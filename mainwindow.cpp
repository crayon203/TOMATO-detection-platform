#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "AnnotWidget.h"
#include "database.h"
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QProcess>
#include <QDebug>
#include <QMessageBox>
#include <QTextCursor>
#include <QPainter>
#include <QPen>
#include <QInputDialog>
#include <QDesktopServices>
#include <QUrl>
#include <QFileInfo>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDateTime>
#include <QProgressBar>
#include <QCoreApplication>
#include <QLibrary>
#include <QDialog>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QPushButton>
#include <algorithm>
#include <cmath>

// ============================================================
//  构造 / 析构
// ============================================================
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    process = new QProcess(this);
    quantProcess = new QProcess(this);
    onnxEngine = new OnnxInference(this);

    // 初始化数据库
    initDatabase();

    // 设置连接
    setupConnections();

    // 初始化类别下拉框
    initCategories();

    // 设置状态栏
    setupStatusBar();

    // 加载历史数据
    loadTrainingHistory();
    loadInferenceHistory();

    // 默认状态
    ui->statusbar->showMessage("就绪 — 请打开图片或加载数据集");
}

MainWindow::~MainWindow()
{
    saveSettings();
    delete ui;
}

// ============================================================
//  初始化
// ============================================================
void MainWindow::initDatabase()
{
    QString dbPath = QCoreApplication::applicationDirPath() + "/tomato.db";
    m_dbReady = DatabaseManager::instance().initialize(dbPath);
    if (m_dbReady) {
        qDebug() << "数据库就绪:" << dbPath;
    }
}

void MainWindow::initCategories()
{
    AnnotWidget *w = qobject_cast<AnnotWidget*>(ui->widgetAnnot);
    if (!w) return;

    // 尝试从数据库加载
    if (m_dbReady) {
        QStringList saved = DatabaseManager::instance().loadCategories();
        if (!saved.isEmpty()) {
            w->setCategories(saved);
        }
    }

    // 填充下拉框
    ui->cbCategory->clear();
    ui->cbCategory->addItems(w->getCategories());
    ui->cbCategory->setCurrentIndex(w->getCurrentCategory());
}

void MainWindow::setupStatusBar()
{
    // 状态栏已由 .ui 创建
    ui->statusbar->showMessage("就绪");
}

void MainWindow::setupConnections()
{
    // ============================
    //  标注工具（保持原有连接）
    // ============================
    connect(ui->btnOpenImage, &QPushButton::clicked, this, &MainWindow::onOpenImage);
    connect(ui->btnSaveYolo, &QPushButton::clicked, this, &MainWindow::onSaveYolo);
    connect(ui->btnClearAnnot, &QPushButton::clicked, this, &MainWindow::onClear);

    // 新增：类别管理
    connect(ui->btnAddCategory, &QPushButton::clicked, this, &MainWindow::onAddCategory);
    connect(ui->btnDelCategory, &QPushButton::clicked, this, &MainWindow::onDelCategory);
    connect(ui->cbCategory, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onCategoryChanged);
    connect(ui->btnDeleteBox, &QPushButton::clicked, this, &MainWindow::onDeleteSelectedBox);

    // 新增：视图操作（已移至工具栏，作用于当前活跃 Tab 的 AnnotWidget）
    connect(ui->btnRotateLeft, &QPushButton::clicked, this, &MainWindow::onRotateLeft);
    connect(ui->btnRotateRight, &QPushButton::clicked, this, &MainWindow::onRotateRight);
    connect(ui->btnResetView, &QPushButton::clicked, this, &MainWindow::onResetView);
    connect(ui->edRotation, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &MainWindow::onRotationAngleChanged);
    connect(ui->btnPanLeft, &QPushButton::clicked, this, &MainWindow::onPanLeft);
    connect(ui->btnPanRight, &QPushButton::clicked, this, &MainWindow::onPanRight);
    connect(ui->btnPanUp, &QPushButton::clicked, this, &MainWindow::onPanUp);
    connect(ui->btnPanDown, &QPushButton::clicked, this, &MainWindow::onPanDown);
    connect(ui->btnZoomIn, &QPushButton::clicked, this, &MainWindow::onZoomIn);
    connect(ui->btnZoomOut, &QPushButton::clicked, this, &MainWindow::onZoomOut);

    // 新增：数据集加载与导航
    connect(ui->btnLoadDataset, &QPushButton::clicked, this, &MainWindow::onLoadDataset);
    connect(ui->btnPrevImage, &QPushButton::clicked, this, &MainWindow::onPrevImage);
    connect(ui->btnNextImage, &QPushButton::clicked, this, &MainWindow::onNextImage);
    connect(ui->btnSaveAllLabels, &QPushButton::clicked, this, &MainWindow::onSaveAllLabels);

    // 新增：AnnotWidget 信号
    AnnotWidget *annotW = qobject_cast<AnnotWidget*>(ui->widgetAnnot);
    if (annotW) {
        connect(annotW, &AnnotWidget::boxesChanged, this, &MainWindow::syncAnnotationTable);
        connect(annotW, &AnnotWidget::boxSelected, this, [this](int idx) {
            ui->tableAnnotations->selectRow(idx);
            updateAnnotationStatus();
        });
        connect(annotW, &AnnotWidget::boxDeselected, this, [this]() {
            ui->tableAnnotations->clearSelection();
            updateAnnotationStatus();
        });
        connect(annotW, &AnnotWidget::statusMessage, this, &MainWindow::updateStatusBar);
        connect(annotW, &AnnotWidget::viewChanged, this, [this](qreal rotation, qreal, QPointF) {
            ui->edRotation->blockSignals(true);
            ui->edRotation->setValue(rotation);
            ui->edRotation->blockSignals(false);
        });
    }

    // 新增：表格交互
    connect(ui->tableAnnotations, &QTableWidget::cellChanged,
            this, &MainWindow::onAnnotationTableCellChanged);

    // ============================
    //  模型训练（保持原有连接）
    // ============================
    connect(ui->btnSelectDataset, &QPushButton::clicked, this, &MainWindow::onSelectDataset);
    connect(ui->btnStartTrain, &QPushButton::clicked, this, &MainWindow::onStartTrain);
    connect(ui->btnStopTrain, &QPushButton::clicked, this, &MainWindow::onStopTrain);

    // 新增：保存训练结果
    connect(ui->btnSaveTrainResult, &QPushButton::clicked, this, &MainWindow::onSaveTrainResult);

    // 新增：数据集划分滑块
    connect(ui->sliderSplit, &QSlider::valueChanged, this, &MainWindow::onSplitSliderChanged);
    connect(ui->spinSplit, QOverload<int>::of(&QSpinBox::valueChanged), ui->sliderSplit, &QSlider::setValue);

    // 训练日志（保持原有）
    connect(process, &QProcess::readyReadStandardOutput, this, [=]() {
        QString output = process->readAllStandardOutput();
        // 检查是否是 JSON 指标输出
        if (output.contains("@@METRICS_JSON@@")) {
            int start = output.indexOf("@@METRICS_JSON@@") + 16;
            int end = output.indexOf("@@END_METRICS@@");
            if (end > start) {
                QString jsonStr = output.mid(start, end - start).trimmed();
                QJsonDocument doc = QJsonDocument::fromJson(jsonStr.toUtf8());
                if (doc.isObject()) {
                    QJsonObject obj = doc.object();
                    m_lastTrainMetrics.clear();
                    for (auto it = obj.begin(); it != obj.end(); ++it) {
                        m_lastTrainMetrics[it.key()] = it.value().toVariant();
                    }

                    // 更新指标显示
                    ui->lblMAP->setText(QString("mAP50: %1").arg(
                        m_lastTrainMetrics.value("map50", "--").toString()));
                    ui->lblMAP95->setText(QString("mAP50-95: %1").arg(
                        m_lastTrainMetrics.value("map50_95", "--").toString()));
                    ui->lblPrecision->setText(QString("Precision: %1").arg(
                        m_lastTrainMetrics.value("precision", "--").toString()));
                    ui->lblRecall->setText(QString("Recall: %1").arg(
                        m_lastTrainMetrics.value("recall", "--").toString()));
                    ui->lblModelPath->setText(QString("模型路径: %1").arg(
                        m_lastTrainMetrics.value("model_path", "--").toString()));
                    ui->lblTrainTime->setText(QString("训练耗时: %1").arg(
                        m_lastTrainMetrics.value("train_time", "--").toString()));
                }
            }
        }

        ui->teTrainLog->append(output);
        ui->teTrainLog->moveCursor(QTextCursor::End);
    });

    connect(process, &QProcess::readyReadStandardError, this, [=]() {
        ui->teTrainLog->append("❌ " + process->readAllStandardError());
        ui->teTrainLog->moveCursor(QTextCursor::End);
    });

    connect(process, &QProcess::finished, this, [=](int, QProcess::ExitStatus) {
        ui->teTrainLog->append("🎉 训练进程结束");
        ui->statusbar->showMessage("训练完成");

        // 训练完成后自动保存到数据库
        if (m_dbReady && !m_lastTrainMetrics.isEmpty()) {
            QVariantMap record;
            record["timestamp"]     = QDateTime::currentDateTime().toString(Qt::ISODate);
            record["dataset_path"]  = ui->edDataDir->text();
            record["epochs"]        = ui->edEpoch->value();
            record["batch_size"]    = ui->edBatch->value();
            record["image_size"]    = ui->edImgsz->value();
            record["learning_rate"] = ui->edLR->value();
            record["optimizer"]     = ui->cbOptimizer->currentText();
            record["device"]        = ui->cbDevice->currentText();
            record["val_split"]     = 1.0 - ui->spinSplit->value() / 100.0;
            record["map50"]         = m_lastTrainMetrics.value("map50");
            record["map50_95"]      = m_lastTrainMetrics.value("map50_95");
            record["precision"]     = m_lastTrainMetrics.value("precision");
            record["recall"]        = m_lastTrainMetrics.value("recall");
            record["model_path"]    = m_lastTrainMetrics.value("model_path");
            record["train_time"]    = m_lastTrainMetrics.value("train_time");

            int id = DatabaseManager::instance().saveTrainingRecord(record);
            if (id > 0) {
                ui->teTrainLog->append(QString("💾 训练结果已自动保存 (ID:%1)").arg(id));
                loadTrainingHistory();
            }
        }
    });

    // ============================
    //  模型量化（保持原有连接）
    // ============================
    connect(ui->btnSelectModel, &QPushButton::clicked, this, &MainWindow::onSelectModel);
    connect(ui->btnStartQuant, &QPushButton::clicked, this, &MainWindow::onStartQuant);
    connect(ui->btnStopQuant, &QPushButton::clicked, this, &MainWindow::onStopQuant);
    connect(quantProcess, &QProcess::readyReadStandardOutput, this, &MainWindow::onQuantOutput);
    connect(quantProcess, &QProcess::readyReadStandardError, this, &MainWindow::onQuantError);
    connect(quantProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &MainWindow::onQuantFinished);

    // ============================
    //  模型推理（保持原有连接）
    // ============================
    connect(ui->btnSelectQuantModel, &QPushButton::clicked, this, &MainWindow::onSelectQuantModel);
    connect(ui->btnOpenInferImage, &QPushButton::clicked, this, &MainWindow::onOpenInferImage);
    connect(ui->btnRunInfer, &QPushButton::clicked, this, &MainWindow::onRunInfer);
    connect(ui->cbInferEngine, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onInferEngineChanged);

    // 批量推理 & 保存结果
    connect(ui->btnBatchInfer, &QPushButton::clicked, this, &MainWindow::onBatchInfer);
    connect(ui->btnSaveInferResult, &QPushButton::clicked, this, &MainWindow::onSaveInferResult);

    // C++ ONNX 推理引擎日志
    connect(onnxEngine, &OnnxInference::logMessage, this, &MainWindow::onCppInferLog);

    // ============================
    //  菜单栏 / 工具栏
    // ============================
    connect(ui->actionExit, &QAction::triggered, this, &QMainWindow::close);
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::onAbout);
    connect(ui->actionOpenImage, &QAction::triggered, this, &MainWindow::onOpenImage);
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::onSaveYolo);
    connect(ui->actionUndo, &QAction::triggered, this, &MainWindow::onUndo);
    connect(ui->actionRedo, &QAction::triggered, this, &MainWindow::onRedo);
    connect(ui->actionZoomIn, &QAction::triggered, this, &MainWindow::onZoomIn);
    connect(ui->actionZoomOut, &QAction::triggered, this, &MainWindow::onZoomOut);
    connect(ui->actionZoomFit, &QAction::triggered, this, &MainWindow::onZoomFit);
    connect(ui->actionRotateLeft, &QAction::triggered, this, &MainWindow::onRotateLeft);
    connect(ui->actionRotateRight, &QAction::triggered, this, &MainWindow::onRotateRight);
    connect(ui->actionResetView, &QAction::triggered, this, &MainWindow::onResetView);
}

// ============================================================
//  类别管理（新增）
// ============================================================
void MainWindow::onAddCategory()
{
    AnnotWidget *w = qobject_cast<AnnotWidget*>(ui->widgetAnnot);
    if (!w) return;

    bool ok;
    QString name = QInputDialog::getText(this, "新建类别", "类别名称:", QLineEdit::Normal, "", &ok);
    if (ok && !name.trimmed().isEmpty()) {
        w->addCategory(name.trimmed());
        ui->cbCategory->blockSignals(true);
        ui->cbCategory->clear();
        ui->cbCategory->addItems(w->getCategories());
        ui->cbCategory->setCurrentIndex(w->getCategories().size() - 1);
        ui->cbCategory->blockSignals(false);

        if (m_dbReady) {
            DatabaseManager::instance().saveCategories(w->getCategories());
        }
        updateStatusBar(QString("已添加类别: %1").arg(name.trimmed()));
    }
}

void MainWindow::onDelCategory()
{
    AnnotWidget *w = qobject_cast<AnnotWidget*>(ui->widgetAnnot);
    if (!w) return;

    int idx = ui->cbCategory->currentIndex();
    if (idx < 0 || w->getCategories().size() <= 1) {
        QMessageBox::warning(this, "警告", "至少保留一个类别");
        return;
    }

    QString catName = w->getCategories()[idx];
    int ret = QMessageBox::question(this, "确认删除",
        QString("确定删除类别 \"%1\" 吗？\n所有该类别标注框将被重置为默认类别。").arg(catName));
    if (ret == QMessageBox::Yes) {
        w->removeCategory(idx);
        ui->cbCategory->blockSignals(true);
        ui->cbCategory->clear();
        ui->cbCategory->addItems(w->getCategories());
        ui->cbCategory->blockSignals(false);

        if (m_dbReady) {
            DatabaseManager::instance().saveCategories(w->getCategories());
        }
        syncAnnotationTable();
        updateStatusBar(QString("已删除类别: %1").arg(catName));
    }
}

void MainWindow::onCategoryChanged(int index)
{
    AnnotWidget *w = qobject_cast<AnnotWidget*>(ui->widgetAnnot);
    if (!w) return;

    // 如果有选中的标注框，则修改该框的类别
    int selBox = w->selectedBoxIndex();
    if (selBox >= 0 && selBox < w->getBoxes().size()) {
        w->setBoxCategory(selBox, index);
        syncAnnotationTable();
        updateStatusBar(QString("已修改框 #%1 类别为: %2")
            .arg(selBox + 1)
            .arg(w->getCategories().value(index, "")));
    } else {
        w->setCurrentCategory(index);
        updateStatusBar(QString("当前类别: %1").arg(w->getCategories().value(index, "")));
    }
}

void MainWindow::onDeleteSelectedBox()
{
    AnnotWidget *w = qobject_cast<AnnotWidget*>(ui->widgetAnnot);
    if (w) {
        w->deleteSelectedBox();
        updateStatusBar("已删除选中标注框");
    }
}

// ============================================================
//  撤销/重做（新增）
// ============================================================
void MainWindow::onUndo()
{
    AnnotWidget *w = currentAnnotWidget();
    if (w) w->undo();
}

void MainWindow::onRedo()
{
    AnnotWidget *w = currentAnnotWidget();
    if (w) w->redo();
}

// ============================================================
//  视图操作（工具栏，作用于当前活跃 Tab 的 AnnotWidget）
// ============================================================
AnnotWidget* MainWindow::currentAnnotWidget()
{
    int idx = ui->tabWidget->currentIndex();
    if (idx == 0) return qobject_cast<AnnotWidget*>(ui->widgetAnnot);
    if (idx == 3) return qobject_cast<AnnotWidget*>(ui->widgetInfer);
    return nullptr;
}

void MainWindow::onZoomIn()
{
    AnnotWidget *w = currentAnnotWidget();
    if (w) w->zoomIn();
}

void MainWindow::onZoomOut()
{
    AnnotWidget *w = currentAnnotWidget();
    if (w) w->zoomOut();
}

void MainWindow::onZoomFit()
{
    AnnotWidget *w = currentAnnotWidget();
    if (w) w->zoomFit();
}

void MainWindow::onRotateLeft()
{
    AnnotWidget *w = currentAnnotWidget();
    if (!w) return;
    w->rotateImage(-15);
    // 同步旋转角度到 UI 控件
    ui->edRotation->blockSignals(true);
    ui->edRotation->setValue(w->rotationAngle());
    ui->edRotation->blockSignals(false);
}

void MainWindow::onRotateRight()
{
    AnnotWidget *w = currentAnnotWidget();
    if (!w) return;
    w->rotateImage(15);
    ui->edRotation->blockSignals(true);
    ui->edRotation->setValue(w->rotationAngle());
    ui->edRotation->blockSignals(false);
}

void MainWindow::onRotationAngleChanged(double degrees)
{
    AnnotWidget *w = currentAnnotWidget();
    if (w) w->setRotationAngle(degrees);
}

void MainWindow::onResetView()
{
    AnnotWidget *w = currentAnnotWidget();
    if (!w) return;
    w->zoomFit();
    // 同步旋转角度到 UI 控件
    ui->edRotation->blockSignals(true);
    ui->edRotation->setValue(0.0);
    ui->edRotation->blockSignals(false);
}

void MainWindow::onPanLeft()
{
    AnnotWidget *w = currentAnnotWidget();
    if (w) w->panBy(40, 0);
}

void MainWindow::onPanRight()
{
    AnnotWidget *w = currentAnnotWidget();
    if (w) w->panBy(-40, 0);
}

void MainWindow::onPanUp()
{
    AnnotWidget *w = currentAnnotWidget();
    if (w) w->panBy(0, 40);
}

void MainWindow::onPanDown()
{
    AnnotWidget *w = currentAnnotWidget();
    if (w) w->panBy(0, -40);
}

// ============================================================
//  数据集加载与导航（新增）
// ============================================================
void MainWindow::onLoadDataset()
{
    QString dir = QFileDialog::getExistingDirectory(this, "选择 YOLO 数据集目录");
    if (dir.isEmpty()) return;

    AnnotWidget *w = qobject_cast<AnnotWidget*>(ui->widgetAnnot);
    if (!w) return;

    w->loadYoloDataset(dir);
    int count = w->datasetImageCount();

    if (count == 0) {
        QMessageBox::information(this, "提示",
            "未在目录中找到图片。\n支持的目录结构: dataset/train/images/, dataset/test/images/ 或 dataset/images/");
        ui->lblDatasetNav->setText("未加载数据集");
        ui->btnPrevImage->setEnabled(false);
        ui->btnNextImage->setEnabled(false);
        return;
    }

    // loadYoloDataset 已自动加载第一张图片
    updateAnnotationStatus();
    updateDatasetNavState();
    syncAnnotationTable();
}

void MainWindow::onPrevImage()
{
    AnnotWidget *w = qobject_cast<AnnotWidget*>(ui->widgetAnnot);
    if (!w || !w->hasPrevImage()) return;

    // 保存当前图片的标注（自动保存到 labels 目录）
    autoSaveCurrentLabels(w);

    int newIdx = w->currentDatasetIndex() - 1;
    w->navigateToImage(newIdx);
    updateAnnotationStatus();
    updateDatasetNavState();
    syncAnnotationTable();
}

void MainWindow::onNextImage()
{
    AnnotWidget *w = qobject_cast<AnnotWidget*>(ui->widgetAnnot);
    if (!w || !w->hasNextImage()) return;

    // 保存当前图片的标注
    autoSaveCurrentLabels(w);

    int newIdx = w->currentDatasetIndex() + 1;
    w->navigateToImage(newIdx);
    updateAnnotationStatus();
    updateDatasetNavState();
    syncAnnotationTable();
}

void MainWindow::updateDatasetNavState()
{
    AnnotWidget *w = qobject_cast<AnnotWidget*>(ui->widgetAnnot);
    if (!w) return;

    int count = w->datasetImageCount();
    int current = w->currentDatasetIndex();

    if (count > 0) {
        int labeled = w->labeledImageCount();
        bool curLabeled = w->currentImageHasLabel();
        ui->lblDatasetNav->setText(
            QString("图片 %1 / %2  [已标注: %3/%4]%5")
                .arg(current + 1)
                .arg(count)
                .arg(labeled)
                .arg(count)
                .arg(curLabeled ? " ✅" : " ⚠️ 未标注"));
        ui->btnPrevImage->setEnabled(w->hasPrevImage());
        ui->btnNextImage->setEnabled(w->hasNextImage());
    } else {
        ui->lblDatasetNav->setText("未加载数据集");
        ui->btnPrevImage->setEnabled(false);
        ui->btnNextImage->setEnabled(false);
    }
}

void MainWindow::autoSaveCurrentLabels(AnnotWidget *w)
{
    if (!w || w->getBoxes().isEmpty()) return;

    QString imgPath = w->getImagePath();
    if (imgPath.isEmpty()) return;

    // 生成对应的 labels 路径
    QString labelPath = imgPath;
    labelPath.replace("/images/", "/labels/");
    labelPath.replace("\\images\\", "\\labels\\");
    for (const QString& ext : {".jpg", ".jpeg", ".png", ".bmp", ".tiff", ".tif", ".webp",
                                ".JPG", ".JPEG", ".PNG", ".BMP", ".TIFF", ".TIF", ".WEBP"}) {
        if (labelPath.endsWith(ext)) {
            labelPath = labelPath.left(labelPath.length() - ext.length()) + ".txt";
            break;
        }
    }

    // 确保 labels 目录存在
    QFileInfo fi(labelPath);
    QDir().mkpath(fi.absolutePath());

    QFile f(labelPath);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text)) return;

    QTextStream out(&f);
    const QVector<QRect>& boxes = w->getBoxes();
    const QVector<int>& cats = w->getBoxCategories();
    int ww = w->getImage().width();
    int hh = w->getImage().height();

    for (int i = 0; i < boxes.size(); ++i) {
        const QRect& r = boxes[i];
        double cx = (r.x() + r.width() / 2.0) / ww;
        double cy = (r.y() + r.height() / 2.0) / hh;
        double bw = r.width() * 1.0 / ww;
        double bh = r.height() * 1.0 / hh;
        int classId = (i < cats.size()) ? cats[i] : 0;
        out << classId << " " << cx << " " << cy << " " << bw << " " << bh << "\n";
    }
    f.close();
}

void MainWindow::onSaveAllLabels()
{
    AnnotWidget *w = qobject_cast<AnnotWidget*>(ui->widgetAnnot);
    if (!w) return;

    QStringList allImages = w->getDatasetImages();
    if (allImages.isEmpty()) {
        QMessageBox::warning(this, "警告", "请先加载数据集");
        return;
    }

    // 先保存当前图片
    autoSaveCurrentLabels(w);

    int savedCount = 0;
    // 遍历所有数据集图片，检查是否有对应的标注文件
    for (const QString& imgPath : allImages) {
        QString labelPath = imgPath;
        labelPath.replace("/images/", "/labels/");
        labelPath.replace("\\images\\", "\\labels\\");
        for (const QString& ext : {".jpg", ".jpeg", ".png", ".bmp", ".tiff", ".tif", ".webp",
                                    ".JPG", ".JPEG", ".PNG", ".BMP", ".TIFF", ".TIF", ".WEBP"}) {
            if (labelPath.endsWith(ext)) {
                labelPath = labelPath.left(labelPath.length() - ext.length()) + ".txt";
                break;
            }
        }
        if (QFile::exists(labelPath)) savedCount++;
    }

    QMessageBox::information(this, "批量保存",
        QString("数据集共 %1 张图片\n已有标注文件: %2 个\n\n"
                "浏览图片时标注自动保存至 labels/ 目录。")
            .arg(allImages.size()).arg(savedCount));
}

// ============================================================
//  标注表格同步（新增）
// ============================================================
void MainWindow::syncAnnotationTable()
{
    AnnotWidget *w = qobject_cast<AnnotWidget*>(ui->widgetAnnot);
    if (!w) return;

    QTableWidget *t = ui->tableAnnotations;
    const QVector<QRect>& boxes = w->getBoxes();
    const QVector<int>& cats = w->getBoxCategories();
    const QStringList& catNames = w->getCategories();

    t->blockSignals(true);
    t->setRowCount(boxes.size());

    for (int i = 0; i < boxes.size(); ++i) {
        const QRect& r = boxes[i];
        int catIdx = (i < cats.size()) ? cats[i] : 0;

        t->setItem(i, 0, new QTableWidgetItem(QString::number(i + 1)));
        t->setItem(i, 1, new QTableWidgetItem(catNames.value(catIdx, "unknown")));
        t->setItem(i, 2, new QTableWidgetItem(QString::number(r.x())));
        t->setItem(i, 3, new QTableWidgetItem(QString::number(r.y())));
        t->setItem(i, 4, new QTableWidgetItem(QString::number(r.width())));
        t->setItem(i, 5, new QTableWidgetItem(QString::number(r.height())));
    }

    t->resizeColumnsToContents();
    t->blockSignals(false);
    updateAnnotationStatus();
}

void MainWindow::onAnnotationTableCellChanged(int row, int col)
{
    AnnotWidget *w = qobject_cast<AnnotWidget*>(ui->widgetAnnot);
    if (!w) return;

    QTableWidgetItem *item = ui->tableAnnotations->item(row, col);
    if (!item) return;

    bool ok;
    int val = item->text().toInt(&ok);
    if (!ok) return;

    QVector<QRect>& boxes = w->getBoxes();
    if (row >= boxes.size()) return;

    QRect r = boxes[row];
    switch (col) {
    case 2: r.setX(val); break;
    case 3: r.setY(val); break;
    case 4: r.setWidth(qMax(1, val)); break;
    case 5: r.setHeight(qMax(1, val)); break;
    }
    boxes[row] = r;
    w->update();
}

// ============================================================
//  标注工具（保持原有逻辑 + 增强）
// ============================================================
void MainWindow::onOpenImage()
{
    QString file = QFileDialog::getOpenFileName(this, "选择图片", "",
        "图像文件 (*.jpg *.jpeg *.jpe *.png *.bmp *.tiff *.tif *.webp);;所有文件 (*)");
    if (file.isEmpty()) return;

    QPixmap img = AnnotWidget::loadImageRobust(file);
    if (img.isNull()) {
        QMessageBox::warning(this, "错误", "无法加载图片: " + file);
        return;
    }

    AnnotWidget *w = qobject_cast<AnnotWidget*>(ui->widgetAnnot);
    if (w) {
        w->setImage(img, file);
        syncAnnotationTable();
    }

    QFileInfo fi(file);
    updateStatusBar(QString("已加载: %1 (%2x%3)")
        .arg(fi.fileName())
        .arg(img.width())
        .arg(img.height()));
}

void MainWindow::onSaveYolo()
{
    AnnotWidget *w = qobject_cast<AnnotWidget*>(ui->widgetAnnot);
    if (!w || w->getImage().isNull() || w->getBoxes().isEmpty()) {
        QMessageBox::warning(this, "", "请先标注！");
        return;
    }

    QString path = QFileDialog::getSaveFileName(this, "保存YOLO标注", "labels.txt",
        "标注文件 (*.txt);;所有文件 (*)");
    if (path.isEmpty()) return;

    QFile f(path);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text)) return;

    QTextStream out(&f);
    QPixmap img = w->getImage();
    int ww = img.width();
    int hh = img.height();
    const QVector<QRect>& boxes = w->getBoxes();
    const QVector<int>& cats = w->getBoxCategories();

    for (int i = 0; i < boxes.size(); ++i) {
        const QRect& r = boxes[i];
        double cx = (r.x() + r.width() / 2.0) / ww;
        double cy = (r.y() + r.height() / 2.0) / hh;
        double bw = r.width() * 1.0 / ww;
        double bh = r.height() * 1.0 / hh;
        int classId = (i < cats.size()) ? cats[i] : 0;
        out << classId << " " << cx << " " << cy << " " << bw << " " << bh << "\n";
    }

    f.close();

    // 保存类别信息到数据库
    if (m_dbReady) {
        DatabaseManager::instance().saveCategories(w->getCategories());
    }

    QMessageBox::information(this, "", "✅ 保存成功！\n" + path);
    updateStatusBar("标注已保存: " + path);
}

void MainWindow::onClear()
{
    AnnotWidget *w = qobject_cast<AnnotWidget*>(ui->widgetAnnot);
    if (w) {
        w->clearBoxes();
        syncAnnotationTable();
        updateStatusBar("已清除所有标注框");
    }
}

// ============================================================
//  模型训练（保持原有逻辑 + 增强参数）
// ============================================================
void MainWindow::onSelectDataset()
{
    QString dir = QFileDialog::getExistingDirectory(this);
    if (!dir.isEmpty()) ui->edDataDir->setText(dir);
}

void MainWindow::onSplitSliderChanged(int value)
{
    ui->spinSplit->setValue(value);
}

void MainWindow::onStartTrain()
{
    QString dir = ui->edDataDir->text();
    if (dir.isEmpty() || !QFile::exists(dir + "/data.yaml")) {
        QMessageBox::warning(this, "", "数据集错误 — 请选择包含 data.yaml 的数据集目录");
        return;
    }

    ui->teTrainLog->clear();
    m_lastTrainMetrics.clear();
    ui->lblMAP->setText("mAP50: --");
    ui->lblMAP95->setText("mAP50-95: --");
    ui->lblPrecision->setText("Precision: --");
    ui->lblRecall->setText("Recall: --");
    ui->lblModelPath->setText("模型路径: --");
    ui->lblTrainTime->setText("训练耗时: --");

    QString pyScript = QCoreApplication::applicationDirPath() + "/train.py";
    if (!QFile::exists(pyScript)) {
        pyScript = "D:/Learn/QtInstaller/Project/tomatos/train.py";
    }
    if (!QFile::exists(pyScript)) {
        pyScript = QDir::currentPath() + "/train.py";
    }

    QStringList args;
    args << pyScript
         << "--data" << dir + "/data.yaml"
         << "--epochs" << QString::number(ui->edEpoch->value())
         << "--batch" << QString::number(ui->edBatch->value())
         << "--imgsz" << QString::number(ui->edImgsz->value())
         << "--device" << ui->cbDevice->currentText()
         << "--lr" << QString::number(ui->edLR->value())
         << "--optimizer" << ui->cbOptimizer->currentText()
         << "--weight_decay" << QString::number(ui->edWD->value())
         << "--val_split" << QString::number(1.0 - ui->spinSplit->value() / 100.0, 'f', 2);

    process->start("python", args);
    ui->statusbar->showMessage("训练中...");
    ui->teTrainLog->append("🚀 训练进程已启动");
}

void MainWindow::onStopTrain()
{
    if (process->state() == QProcess::Running) {
        process->terminate();
        ui->teTrainLog->append("🛑 训练已停止");
        ui->statusbar->showMessage("训练已停止");
    }
}

void MainWindow::onSaveTrainResult()
{
    if (!m_dbReady) {
        QMessageBox::warning(this, "错误", "数据库未就绪");
        return;
    }
    if (m_lastTrainMetrics.isEmpty()) {
        QMessageBox::information(this, "提示", "没有可保存的训练结果");
        return;
    }

    QVariantMap record;
    record["timestamp"] = QDateTime::currentDateTime().toString(Qt::ISODate);
    record["dataset_path"] = ui->edDataDir->text();
    record["epochs"] = ui->edEpoch->value();
    record["batch_size"] = ui->edBatch->value();
    record["image_size"] = ui->edImgsz->value();
    record["learning_rate"] = ui->edLR->value();
    record["optimizer"] = ui->cbOptimizer->currentText();
    record["device"] = ui->cbDevice->currentText();
    record["val_split"] = 1.0 - ui->spinSplit->value() / 100.0;
    record["map50"] = m_lastTrainMetrics.value("map50");
    record["map50_95"] = m_lastTrainMetrics.value("map50_95");
    record["precision"] = m_lastTrainMetrics.value("precision");
    record["recall"] = m_lastTrainMetrics.value("recall");
    record["model_path"] = m_lastTrainMetrics.value("model_path");
    record["train_time"] = m_lastTrainMetrics.value("train_time");

    int id = DatabaseManager::instance().saveTrainingRecord(record);
    if (id > 0) {
        QMessageBox::information(this, "", QString("✅ 训练结果已保存到数据库 (ID: %1)").arg(id));
        loadTrainingHistory();
    } else {
        QMessageBox::warning(this, "", "保存失败");
    }
}

void MainWindow::loadTrainingHistory()
{
    if (!m_dbReady) return;

    QVector<QVariantMap> records = DatabaseManager::instance().getAllTrainingRecords();
    QTableWidget *t = ui->tableTrainHistory;
    t->setRowCount(records.size());

    for (int i = 0; i < records.size(); ++i) {
        const QVariantMap& r = records[i];
        t->setItem(i, 0, new QTableWidgetItem(r.value("id").toString()));
        t->setItem(i, 1, new QTableWidgetItem(r.value("timestamp").toString()));
        t->setItem(i, 2, new QTableWidgetItem(r.value("dataset_path").toString()));
        t->setItem(i, 3, new QTableWidgetItem(r.value("map50").toString()));
        t->setItem(i, 4, new QTableWidgetItem(r.value("map50_95").toString()));
        t->setItem(i, 5, new QTableWidgetItem(r.value("precision").toString()));
        t->setItem(i, 6, new QTableWidgetItem(r.value("recall").toString()));
        t->setItem(i, 7, new QTableWidgetItem(r.value("model_path").toString()));
        t->setItem(i, 8, new QTableWidgetItem(r.value("train_time").toString()));
    }
    t->resizeColumnsToContents();
}

// ============================================================
//  模型量化（保持原有逻辑 + 增强参数）
// ============================================================
void MainWindow::onSelectModel()
{
    QString f = QFileDialog::getOpenFileName(this, "选择模型", "",
        "模型文件 (*.pt *.onnx);;所有文件 (*)");
    if (!f.isEmpty()) ui->edModelPath->setText(f);
}

void MainWindow::onStartQuant()
{
    QString m = ui->edModelPath->text();
    if (m.isEmpty()) {
        QMessageBox::warning(this, "", "请选择模型文件");
        return;
    }

    QString pyScript = QCoreApplication::applicationDirPath() + "/quantize.py";
    if (!QFile::exists(pyScript)) {
        pyScript = "D:/Learn/QtInstaller/Project/tomatos/quantize.py";
    }
    if (!QFile::exists(pyScript)) {
        pyScript = QDir::currentPath() + "/quantize.py";
    }

    QStringList args;
    args << pyScript
         << "--model" << m
         << "--quant" << ui->cbQuantType->currentText()
         << "--scheme" << ui->cbQuantScheme->currentText().left(3)  // PTQ or QAT
         << "--pathway" << ui->cbQuantPathway->currentText();

    quantProcess->start("python", args);
    ui->statusbar->showMessage("量化中...");
}

void MainWindow::onStopQuant()
{
    if (quantProcess->state() == QProcess::Running) {
        quantProcess->terminate();
        ui->statusbar->showMessage("量化已停止");
    }
}

void MainWindow::onQuantOutput()
{
    ui->teQuantLog->append(quantProcess->readAllStandardOutput());
}

void MainWindow::onQuantError()
{
    ui->teQuantLog->append("❌ " + quantProcess->readAllStandardError());
}

void MainWindow::onQuantFinished()
{
    QMessageBox::information(this, "", "✅ 量化完成！");
    ui->statusbar->showMessage("量化完成");
}

// ============================================================
//  模型推理（保持原有逻辑 + 增强参数和入库）
// ============================================================
void MainWindow::onSelectQuantModel()
{
    // 根据推理引擎调整默认文件过滤
    int engine = ui->cbInferEngine->currentIndex();
    QString filter;
    if (engine == 0)
        filter = "训练模型 (*.pt);;ONNX模型 (*.onnx);;所有文件 (*)";
    else
        filter = "ONNX模型 (*.onnx);;训练模型 (*.pt);;所有文件 (*)";

    QString f = QFileDialog::getOpenFileName(this, "选择模型", "", filter);
    if (!f.isEmpty()) {
        ui->edQuantModelPath->setText(f);
        // 不主动加载模型 — 点击"开始推理"时才加载
    }
}

void MainWindow::onInferEngineChanged(int index)
{
    ui->edQuantModelPath->clear();
    ui->widgetInfer->clearBoxes();
    ui->tableInferResults->setRowCount(0);

    if (index == 0) {
        // Python 模式
        updateStatusBar("推理引擎: Python 加载训练模型 (.pt)");
        ui->label_9->setText("模型路径 (.pt):");
    } else {
        // C++ 模式
        updateStatusBar("推理引擎: C++ 加载量化模型 (.onnx)");
        ui->label_9->setText("模型路径 (.onnx):");

        if (!OnnxInference::isOnnxRuntimeAvailable()) {
            updateStatusBar("⚠️ 未启用 ONNX Runtime — C++ 推理不可用");
        }
    }
}

void MainWindow::onCppInferLog(const QString& msg)
{
    // C++ 推理日志只显示在状态栏，不污染训练日志区
    ui->statusbar->showMessage("[ONNX] " + msg, 5000);
}

void MainWindow::onOpenInferImage()
{
    QString f = QFileDialog::getOpenFileName(this, "选择推理图片", "",
        "图像文件 (*.jpg *.jpeg *.jpe *.png *.bmp *.tiff *.tif *.webp);;所有文件 (*)");
    if (f.isEmpty()) return;

    QPixmap img = AnnotWidget::loadImageRobust(f);
    if (img.isNull()) {
        QMessageBox::warning(this, "错误", "无法加载图片");
        return;
    }

    AnnotWidget *w = qobject_cast<AnnotWidget*>(ui->widgetInfer);
    if (w) {
        w->setImage(img, f);
        ui->statusbar->showMessage("推理图片已加载: " + f);
    }
}

void MainWindow::onRunInfer()
{
    QString model = ui->edQuantModelPath->text();
    AnnotWidget *w = qobject_cast<AnnotWidget*>(ui->widgetInfer);

    if (model.isEmpty() || !w || w->getImage().isNull()) {
        QMessageBox::warning(this, "", "请选择模型与图片");
        return;
    }

    int engine = ui->cbInferEngine->currentIndex();

    if (engine == 0) {
        // ======== Python 推理：加载训练原始模型 (.pt) ========
        runPythonInference(model, w);
    } else {
        // ======== C++ 推理：加载量化 ONNX 模型 (.onnx) ========
        runCppInference();
    }
}

void MainWindow::runPythonInference(const QString& model, AnnotWidget *w)
{
    QString pyScript = QCoreApplication::applicationDirPath() + "/infer.py";
    if (!QFile::exists(pyScript)) {
        pyScript = "D:/Learn/QtInstaller/Project/tomatos/infer.py";
    }
    if (!QFile::exists(pyScript)) {
        pyScript = QDir::currentPath() + "/infer.py";
    }

    ui->statusbar->showMessage("Python 推理中...");

    // 查找 Python 可执行文件
    QString pythonExe;
    auto tryPython = [&](const QString& name) -> bool {
        QProcess t;
        t.start(name, {"--version"});
        if (!t.waitForStarted(3000)) return false;
        t.waitForFinished(5000);
        if (t.exitCode() == 0) { pythonExe = name; return true; }
        return false;
    };
    // 按优先级尝试：系统 PATH → 常见 Anaconda 路径
    QStringList candidates = {
        "python", "python3", "py",
        "D:/Learn/Anaconda3/python.exe",
        "C:/Users/" + qgetenv("USERNAME") + "/anaconda3/python.exe",
        "C:/Users/" + qgetenv("USERNAME") + "/miniconda3/python.exe",
        "C:/ProgramData/anaconda3/python.exe",
        "C:/ProgramData/miniconda3/python.exe",
    };
    bool found = false;
    for (const QString& c : candidates) {
        if (tryPython(c)) { found = true; break; }
    }
    if (!found) {
        QMessageBox::warning(this, "推理失败",
            "找不到 Python。\n\n请安装 Python 或将其添加到系统 PATH。\n"
            "Anaconda 用户请打开 Anaconda Prompt 启动本程序。");
        return;
    }

    QProcess p;
    QStringList args;
    args << pyScript
         << "--model" << model
         << "--img" << w->getImagePath()
         << "--conf" << QString::number(ui->edConfThresh->value())
         << "--iou" << QString::number(ui->edIoUThresh->value());

    // 把标注工具里定义的类别名传给推理脚本
    AnnotWidget *annotW = qobject_cast<AnnotWidget*>(ui->widgetAnnot);
    if (annotW && !annotW->getCategories().isEmpty()) {
        args << "--names" << annotW->getCategories().join(",");
    }

    p.start(pythonExe, args);

    if (!p.waitForStarted(5000)) {
        QMessageBox::warning(this, "推理失败",
            QString("无法启动 Python。\n\n"
                    "请检查:\n"
                    "1. Python 是否已安装\n"
                    "2. 脚本是否存在: %1\n"
                    "3. 模型文件是否存在: %2")
                .arg(pyScript).arg(model));
        return;
    }

    p.waitForFinished(120000);

    QString out = p.readAllStandardOutput().trimmed();
    QString err = p.readAllStandardError().trimmed();

    // 解析检测行
    QVector<InferDetection> detections;

    w->clearBoxes();
    w->getConfidences().clear();

    QStringList lines = out.split("\n", Qt::SkipEmptyParts);
    for (const QString& line : lines) {
        if (line.startsWith("@@") || line.startsWith("===")) continue;
        if (line.startsWith("{") || line.startsWith("}")) continue;
        if (line.startsWith("⚠️")) continue;
        if (line.trimmed().isEmpty()) continue;

        QStringList parts = line.split(" ", Qt::SkipEmptyParts);
        if (parts.size() < 5) continue;

        bool ok1, ok2, ok3, ok4;
        int bx = parts[0].toInt(&ok1);
        int by = parts[1].toInt(&ok2);
        int bw = parts[2].toInt(&ok3);
        int bh = parts[3].toInt(&ok4);
        float conf = parts[4].toFloat();

        if (!ok1 || !ok2 || !ok3 || !ok4) continue;
        if (bw <= 0 || bh <= 0) continue;
        if (conf < 0.001 || conf > 1.0) continue;

        InferDetection d;
        d.x = bx; d.y = by; d.w = bw; d.h = bh;
        d.conf = conf;
        d.className = parts.size() >= 7 ? parts[6] : "object";

        // 把类别名映射到标注工具的类别索引，才能正确显示标签
        int catIdx = annotW ? annotW->getCategories().indexOf(d.className) : -1;
        if (catIdx < 0) catIdx = 0;  // 找不到则归为第一个类别

        w->getBoxes().append(QRect(d.x, d.y, d.w, d.h));
        w->getConfidences().append(d.conf);
        w->getBoxCategories().append(catIdx);
        detections.append(d);
    }

    // 如果未检测到目标，显示详细信息帮助排查
    if (detections.isEmpty()) {
        QString diag = "⚠️ 未检测到任何目标\n\n"
                       "==== Python 命令 ====\n" + pythonExe + " " + args.join(" ") +
                       "\n\n==== stdout ====\n" + (out.isEmpty() ? "(空)" : out) +
                       "\n\n==== stderr ====\n" + (err.isEmpty() ? "(空)" : err);
        QDialog dlg(this);
        dlg.setWindowTitle("推理诊断");
        dlg.resize(700, 400);
        QVBoxLayout* lay = new QVBoxLayout(&dlg);
        QLabel* lbl = new QLabel("未检测到任何目标，以下是详细信息（可选中复制）：");
        QTextEdit* te = new QTextEdit();
        te->setReadOnly(true);
        te->setPlainText(diag);
        te->setFont(QFont("Consolas", 10));
        QPushButton* btn = new QPushButton("关闭");
        connect(btn, &QPushButton::clicked, &dlg, &QDialog::accept);
        lay->addWidget(lbl);
        lay->addWidget(te);
        lay->addWidget(btn);
        dlg.exec();
        return;
    }

    w->update();
    syncInferTable();

    // 入库 & 保存图片
    finishInference(detections, model, w->getImagePath());
}

void MainWindow::runCppInference()
{
    AnnotWidget *w = qobject_cast<AnnotWidget*>(ui->widgetInfer);
    AnnotWidget *annotW = qobject_cast<AnnotWidget*>(ui->widgetAnnot);
    if (!w) return;

    QString modelPath = ui->edQuantModelPath->text();

    // 先尝试原生 C++ ONNX Runtime
    if (OnnxInference::isOnnxRuntimeAvailable()) {
        if (onnxEngine->isLoaded() || onnxEngine->loadModel(modelPath)) {
            ui->statusbar->showMessage("C++ ONNX Runtime 推理中...");

            QImage image = w->getImage().toImage().convertToFormat(QImage::Format_RGB888);
            float conf = static_cast<float>(ui->edConfThresh->value());
            float iou  = static_cast<float>(ui->edIoUThresh->value());

            QVector<DetectionResult> results = onnxEngine->detect(image, conf, iou);

            w->clearBoxes();
            w->getConfidences().clear();
            QVector<InferDetection> detections;
            for (const DetectionResult& r : results) {
                w->getBoxes().append(r.box);
                w->getConfidences().append(r.confidence);
                InferDetection d;
                d.x = r.box.x(); d.y = r.box.y();
                d.w = r.box.width(); d.h = r.box.height();
                d.conf = r.confidence;
                d.className = r.className;
                detections.append(d);

                // 映射类别名到索引
                int catIdx = annotW ? annotW->getCategories().indexOf(r.className) : -1;
                if (catIdx < 0) catIdx = 0;
                w->getBoxCategories().append(catIdx);
            }
            w->update();
            syncInferTable();
            ui->statusbar->showMessage(
                QString("C++ ONNX 推理完成 — %1 个目标").arg(detections.size()));
            finishInference(detections, modelPath, w->getImagePath());
            return;
        }
    }

    // ONNX Runtime 不可用 → 静默回退 Python（ultralytics 可以加载 .onnx）
    ui->statusbar->showMessage("ONNX Runtime 未安装，自动使用 Python 推理...");
    runPythonInference(modelPath, w);
}

void MainWindow::finishInference(QVector<InferDetection>& detections,
                                  const QString& modelPath,
                                  const QString& imagePath)
{
    AnnotWidget *w = qobject_cast<AnnotWidget*>(ui->widgetInfer);

    if (!detections.isEmpty()) {
        float avgConf = 0;
        for (auto& d : detections) avgConf += d.conf;
        avgConf /= detections.size();

        QVariantMap record;
        record["model_path"] = modelPath;
        record["image_path"] = imagePath;
        record["image_name"] = QFileInfo(imagePath).fileName();
        record["conf_threshold"] = ui->edConfThresh->value();
        record["iou_threshold"] = ui->edIoUThresh->value();
        record["detection_count"] = detections.size();
        record["avg_confidence"] = avgConf;
        record["result_image_path"] = "";

        if (m_dbReady) {
            m_currentInferId = DatabaseManager::instance().saveInferenceRecord(record);
            for (const InferDetection& d : detections) {
                QVariantMap det;
                det["class_name"] = d.className;
                det["x"] = d.x; det["y"] = d.y;
                det["width"] = d.w; det["height"] = d.h;
                det["confidence"] = d.conf;
                DatabaseManager::instance().saveDetectionDetail(m_currentInferId, det);
            }
        }

        QMessageBox::information(this, "",
            QString("✅ 推理完成！检测到 %1 个目标\n平均置信度: %2")
                .arg(detections.size())
                .arg(avgConf, 0, 'f', 3));
    } else {
        QMessageBox::information(this, "", "⚠️ 推理完成，未检测到目标");
    }

    // 保存带检测框图片
    if (!detections.isEmpty() && w) {
        QPixmap img = w->getImage();
        QPainter pp(&img);
        pp.setPen(QPen(Qt::red, 3));
        pp.setFont(QFont("Arial", 11, QFont::Bold));
        for (const InferDetection& d : detections) {
            pp.drawRect(QRect(d.x, d.y, d.w, d.h));
            pp.drawText(d.x, d.y - 5,
                        QString("%1 %2").arg(d.className).arg(d.conf, 0, 'f', 2));
        }
        img.save("result.jpg");
    }

    ui->statusbar->showMessage(
        QString("推理完成 — 检测到 %1 个目标").arg(detections.size()));
}

void MainWindow::onBatchInfer()
{
    QString dir = QFileDialog::getExistingDirectory(this, "选择包含图片的文件夹");
    if (dir.isEmpty()) return;

    QString model = ui->edQuantModelPath->text();
    if (model.isEmpty()) {
        QMessageBox::warning(this, "", "请先选择模型");
        return;
    }

    QString pyScript = QCoreApplication::applicationDirPath() + "/infer.py";
    if (!QFile::exists(pyScript)) {
        pyScript = "D:/Learn/QtInstaller/Project/tomatos/infer.py";
    }
    if (!QFile::exists(pyScript)) {
        pyScript = QDir::currentPath() + "/infer.py";
    }

    QProcess p;
    QStringList args;
    args << pyScript
         << "--model" << model
         << "--img" << dir
         << "--batch"
         << "--conf" << QString::number(ui->edConfThresh->value())
         << "--iou" << QString::number(ui->edIoUThresh->value());

    AnnotWidget *annotW = qobject_cast<AnnotWidget*>(ui->widgetAnnot);
    if (annotW && !annotW->getCategories().isEmpty()) {
        args << "--names" << annotW->getCategories().join(",");
    }

    p.start("python", args);
    p.waitForFinished(300000);  // 5分钟超时（批量可能很长时间）

    QString out = p.readAllStandardOutput();
    ui->statusbar->showMessage("批量推理完成");
    QMessageBox::information(this, "批量推理", out);
}

void MainWindow::onSaveInferResult()
{
    if (!m_dbReady || m_currentInferId < 0) {
        QMessageBox::warning(this, "", "请先执行推理");
        return;
    }

    loadInferenceHistory();
    QMessageBox::information(this, "", "✅ 推理结果已保存到数据库");
}

void MainWindow::syncInferTable()
{
    AnnotWidget *w = qobject_cast<AnnotWidget*>(ui->widgetInfer);
    if (!w) return;

    QTableWidget *t = ui->tableInferResults;
    const QVector<QRect>& boxes = w->getBoxes();
    const QVector<float>& confs = w->getConfidences();

    t->setRowCount(boxes.size());
    // 确保有置信度列
    if (t->columnCount() < 7) {
        t->setColumnCount(7);
        t->setHorizontalHeaderItem(6, new QTableWidgetItem("置信度"));
    }

    for (int i = 0; i < boxes.size(); ++i) {
        const QRect& r = boxes[i];
        t->setItem(i, 0, new QTableWidgetItem(QString::number(i + 1)));
        // 使用 m_boxCategories 获取真实类别名
        AnnotWidget *annotW = qobject_cast<AnnotWidget*>(ui->widgetAnnot);
        const QVector<int>& boxCats = w->getBoxCategories();
        QString catName = "object";
        if (annotW && i < boxCats.size() && boxCats[i] < annotW->getCategories().size())
            catName = annotW->getCategories()[boxCats[i]];
        t->setItem(i, 1, new QTableWidgetItem(catName));
        t->setItem(i, 2, new QTableWidgetItem(QString::number(r.x())));
        t->setItem(i, 3, new QTableWidgetItem(QString::number(r.y())));
        t->setItem(i, 4, new QTableWidgetItem(QString::number(r.width())));
        t->setItem(i, 5, new QTableWidgetItem(QString::number(r.height())));
        if (i < confs.size()) {
            t->setItem(i, 6, new QTableWidgetItem(QString::number(confs[i], 'f', 4)));
        }
    }
    t->resizeColumnsToContents();
}

void MainWindow::loadInferenceHistory()
{
    if (!m_dbReady) return;

    QVector<QVariantMap> records = DatabaseManager::instance().getAllInferenceRecords();
    QTableWidget *t = ui->tableInferHistory;
    t->setRowCount(records.size());

    for (int i = 0; i < records.size(); ++i) {
        const QVariantMap& r = records[i];
        t->setItem(i, 0, new QTableWidgetItem(r.value("id").toString()));
        t->setItem(i, 1, new QTableWidgetItem(r.value("timestamp").toString()));
        t->setItem(i, 2, new QTableWidgetItem(r.value("model_path").toString()));
        t->setItem(i, 3, new QTableWidgetItem(r.value("image_name").toString()));
        t->setItem(i, 4, new QTableWidgetItem(r.value("detection_count").toString()));
        t->setItem(i, 5, new QTableWidgetItem(r.value("avg_confidence").toString()));
    }
    t->resizeColumnsToContents();
}

// ============================================================
//  通用
// ============================================================
void MainWindow::onAbout()
{
    QMessageBox::about(this, "关于 TOMATO",
        "<h2>TOMATO 番茄检测平台 v2.0</h2>"
        "<p>基于 QT C++ + YOLOv8 的桌面标注训练推理一体化工具</p>"
        "<p>功能：图像标注 | 模型训练 | 模型量化 | 图像推理 | 数据管理</p>");
}

void MainWindow::updateStatusBar(const QString& msg)
{
    ui->statusbar->showMessage(msg);
}

void MainWindow::updateAnnotationStatus()
{
    AnnotWidget *w = qobject_cast<AnnotWidget*>(ui->widgetAnnot);
    if (!w || w->getImage().isNull()) {
        if (w && w->datasetImageCount() == 0) {
            ui->lblDatasetNav->setText("未加载数据集");
        }
        return;
    }

    QFileInfo fi(w->getImagePath());
    QString info = QString("图片: %1 | 尺寸: %2x%3 | 标注框: %4 | 缩放: %5%")
            .arg(fi.fileName())
            .arg(w->getImage().width())
            .arg(w->getImage().height())
            .arg(w->getBoxes().size())
            .arg((int)(w->zoomFactor() * 100));

    // 如果加载了数据集，显示进度
    if (w->datasetImageCount() > 0) {
        info += QString(" | 数据集: %1/%2")
            .arg(w->currentDatasetIndex() + 1)
            .arg(w->datasetImageCount());
    }

    ui->statusbar->showMessage(info);
}

void MainWindow::applySettings()
{
    // 可用于加载上次保存的设置
}

void MainWindow::saveSettings()
{
    AnnotWidget *w = qobject_cast<AnnotWidget*>(ui->widgetAnnot);
    if (w && m_dbReady) {
        DatabaseManager::instance().saveCategories(w->getCategories());
    }
}

// ##############################################################################
//  ONNX Runtime C++ 推理引擎 — 运行时动态加载（不需要编译时 SDK）
// ##############################################################################

// ---- C API 基础类型 ----
typedef int32_t  OrtErrorCode;
typedef uint32_t ONNXTensorElementDataType;
#define ONNX_TENSOR_ELEMENT_DATA_TYPE_FLOAT  1

struct OrtEnv; struct OrtSession; struct OrtMemoryInfo; struct OrtAllocator;
struct OrtValue; struct OrtRunOptions; struct OrtStatus;
struct OrtTypeInfo; struct OrtTensorTypeAndShapeInfo;
typedef struct OrtSessionOptions OrtSessionOptions;

typedef OrtStatus* (*FN_CreateRunOptions)(OrtRunOptions** out);
typedef void       (*FN_ReleaseRunOptions)(OrtRunOptions*);
typedef OrtErrorCode (*OrtGetErrorCode)(const OrtStatus* status);
typedef const char*  (*OrtGetErrorMessage)(const OrtStatus* status);
typedef void         (*OrtReleaseStatus)(OrtStatus*);
typedef OrtStatus* (*OrtCreateEnv)(uint32_t logLevel, const char* logid, OrtEnv** out);
typedef void       (*OrtReleaseEnv)(OrtEnv* env);
typedef OrtStatus* (*OrtCreateCpuMemoryInfo)(int device, int memType, OrtMemoryInfo** out);
typedef void       (*OrtReleaseMemoryInfo)(OrtMemoryInfo* info);
typedef OrtStatus* (*OrtCreateSessionOptions)(OrtSessionOptions** out);
typedef void       (*OrtReleaseSessionOptions)(OrtSessionOptions*);
typedef OrtStatus* (*OrtSetSessionGraphOptimizationLevel)(OrtSessionOptions*, int);
typedef OrtStatus* (*OrtSetIntraOpNumThreads)(OrtSessionOptions*, int);
typedef OrtStatus* (*OrtCreateSession)(OrtEnv*, const char* modelPath, OrtSessionOptions*, OrtSession** out);
typedef void       (*OrtReleaseSession)(OrtSession*);
typedef OrtStatus* (*OrtSessionGetInputCount)(const OrtSession*, size_t* out);
typedef OrtStatus* (*OrtSessionGetOutputCount)(const OrtSession*, size_t* out);
typedef OrtStatus* (*OrtSessionGetInputName)(const OrtSession*, size_t, OrtAllocator*, char** out);
typedef OrtStatus* (*OrtSessionGetOutputName)(const OrtSession*, size_t, OrtAllocator*, char** out);
typedef OrtStatus* (*OrtSessionGetInputTypeInfo)(const OrtSession*, size_t, OrtTypeInfo** out);
typedef void       (*OrtReleaseTypeInfo)(OrtTypeInfo*);
typedef OrtStatus* (*OrtCastTypeInfoToTensorInfo)(const OrtTypeInfo*, const OrtTensorTypeAndShapeInfo** out);
typedef OrtStatus* (*OrtGetTensorElementType)(const OrtTensorTypeAndShapeInfo*, ONNXTensorElementDataType* out);
typedef OrtStatus* (*OrtGetDimensionsCount)(const OrtTensorTypeAndShapeInfo*, size_t* out);
typedef OrtStatus* (*OrtGetDimensions)(const OrtTensorTypeAndShapeInfo*, int64_t* dims, size_t dimCount);
typedef void       (*OrtReleaseTensorTypeAndShapeInfo)(OrtTensorTypeAndShapeInfo*);
typedef OrtStatus* (*OrtCreateTensorWithDataAsOrtValue)(OrtMemoryInfo*, void* data, size_t dataLen, const int64_t* shape, size_t rank, ONNXTensorElementDataType type, OrtValue** out);
typedef OrtStatus* (*OrtGetValueType)(const OrtValue*, int* out);
typedef OrtStatus* (*OrtGetTensorMutableData)(OrtValue*, void** out);
typedef void       (*OrtReleaseValue)(OrtValue*);
typedef OrtStatus* (*OrtGetTensorTypeAndShape)(OrtValue*, OrtTensorTypeAndShapeInfo** out);
typedef OrtStatus* (*OrtRun)(OrtSession*, OrtRunOptions*, const char* const* inputNames, const OrtValue* const* inputs, size_t inputCount, const char* const* outputNames, size_t outputCount, OrtValue** outputs);
typedef OrtStatus* (*OrtGetAllocatorWithDefaultOptions)(OrtAllocator** out);
typedef const void* (*OrtGetApiBase)();

struct OrtFunctions {
    FN_CreateRunOptions               CreateRunOptions = nullptr;
    FN_ReleaseRunOptions              ReleaseRunOptions = nullptr;
    OrtGetErrorMessage                GetErrorMessage = nullptr;
    OrtReleaseStatus                  ReleaseStatus = nullptr;
    OrtCreateEnv                      CreateEnv = nullptr;
    OrtReleaseEnv                     ReleaseEnv = nullptr;
    OrtCreateCpuMemoryInfo            CreateCpuMemoryInfo = nullptr;
    OrtReleaseMemoryInfo              ReleaseMemoryInfo = nullptr;
    OrtCreateSessionOptions           CreateSessionOptions = nullptr;
    OrtReleaseSessionOptions          ReleaseSessionOptions = nullptr;
    OrtSetSessionGraphOptimizationLevel SetSessionGraphOptimizationLevel = nullptr;
    OrtSetIntraOpNumThreads           SetIntraOpNumThreads = nullptr;
    OrtCreateSession                  CreateSession = nullptr;
    OrtReleaseSession                 ReleaseSession = nullptr;
    OrtSessionGetInputCount           GetInputCount = nullptr;
    OrtSessionGetOutputCount          GetOutputCount = nullptr;
    OrtSessionGetInputName            GetInputName = nullptr;
    OrtSessionGetOutputName           GetOutputName = nullptr;
    OrtSessionGetInputTypeInfo        GetInputTypeInfo = nullptr;
    OrtReleaseTypeInfo                ReleaseTypeInfo = nullptr;
    OrtCastTypeInfoToTensorInfo       CastTypeInfoToTensorInfo = nullptr;
    OrtGetTensorElementType           GetTensorElementType = nullptr;
    OrtGetDimensionsCount             GetDimensionsCount = nullptr;
    OrtGetDimensions                  GetDimensions = nullptr;
    OrtReleaseTensorTypeAndShapeInfo  ReleaseTensorTypeAndShapeInfo = nullptr;
    OrtCreateTensorWithDataAsOrtValue  CreateTensorWithDataAsOrtValue = nullptr;
    OrtGetValueType                   GetValueType = nullptr;
    OrtGetTensorMutableData           GetTensorMutableData = nullptr;
    OrtReleaseValue                   ReleaseValue = nullptr;
    OrtGetTensorTypeAndShape          GetTensorTypeAndShape = nullptr;
    OrtRun                            Run = nullptr;
    OrtGetAllocatorWithDefaultOptions GetAllocatorWithDefaultOptions = nullptr;
    bool isValid() const { return CreateEnv && CreateSession && CreateTensorWithDataAsOrtValue && Run; }
};

bool OnnxInference::s_ortAvailable = false;
bool OnnxInference::s_ortChecked   = false;
static QLibrary*     g_ortLib    = nullptr;
static OrtFunctions  g_ort;
static bool          g_ortLoaded = false;

// ---- 搜索 onnxruntime.dll ----
QString OnnxInference::findOrtDll()
{
    QStringList paths;
    paths << QCoreApplication::applicationDirPath() + "/onnxruntime.dll"
          << "C:/Users/" + qgetenv("USERNAME") + "/AppData/Local/Programs/Python/Python312/Lib/site-packages/onnxruntime/capi/onnxruntime.dll"
          << "C:/Users/" + qgetenv("USERNAME") + "/AppData/Local/Programs/Python/Python311/Lib/site-packages/onnxruntime/capi/onnxruntime.dll"
          << "C:/Users/" + qgetenv("USERNAME") + "/miniconda3/Lib/site-packages/onnxruntime/capi/onnxruntime.dll"
          << "C:/Users/" + qgetenv("USERNAME") + "/anaconda3/Lib/site-packages/onnxruntime/capi/onnxruntime.dll"
          << "C:/ProgramData/miniconda3/Lib/site-packages/onnxruntime/capi/onnxruntime.dll"
          << "C:/ProgramData/anaconda3/Lib/site-packages/onnxruntime/capi/onnxruntime.dll"
          << "D:/Learn/Anaconda3/Lib/site-packages/onnxruntime/capi/onnxruntime.dll";
    for (const QString& p : paths)
        if (QFile::exists(p)) return p;
    return "onnxruntime.dll";  // fallback: system PATH
}

// ---- 加载 DLL + 解析函数指针 ----
bool OnnxInference::initOrtLibrary()
{
    if (g_ortLoaded && g_ort.isValid()) return true;

    g_ortLib = new QLibrary(findOrtDll());
    if (!g_ortLib->load()) { delete g_ortLib; g_ortLib = nullptr; return false; }

#define LOAD(fn) g_ort.fn = reinterpret_cast<decltype(g_ort.fn)>(g_ortLib->resolve(#fn))
    LOAD(CreateEnv);          LOAD(CreateSession);
    LOAD(ReleaseEnv);         LOAD(ReleaseSession);
    LOAD(ReleaseStatus);      LOAD(GetErrorMessage);
    LOAD(CreateCpuMemoryInfo);LOAD(ReleaseMemoryInfo);
    LOAD(CreateSessionOptions);LOAD(ReleaseSessionOptions);
    LOAD(SetSessionGraphOptimizationLevel);LOAD(SetIntraOpNumThreads);
    LOAD(CreateRunOptions);   LOAD(ReleaseRunOptions);
    LOAD(Run);                LOAD(GetInputCount);
    LOAD(GetOutputCount);     LOAD(GetInputName);
    LOAD(GetOutputName);      LOAD(GetInputTypeInfo);
    LOAD(ReleaseTypeInfo);    LOAD(CastTypeInfoToTensorInfo);
    LOAD(GetTensorElementType);LOAD(GetDimensionsCount);
    LOAD(GetDimensions);      LOAD(ReleaseTensorTypeAndShapeInfo);
    LOAD(CreateTensorWithDataAsOrtValue);
    LOAD(GetTensorMutableData);LOAD(ReleaseValue);
    LOAD(GetAllocatorWithDefaultOptions);
    g_ort.GetValueType = reinterpret_cast<decltype(g_ort.GetValueType)>(g_ortLib->resolve("GetValueType"));
    g_ort.GetTensorTypeAndShape = reinterpret_cast<decltype(g_ort.GetTensorTypeAndShape)>(g_ortLib->resolve("GetTensorTypeAndShape"));
#undef LOAD

    if (!g_ort.isValid()) { g_ortLib->unload(); delete g_ortLib; g_ortLib = nullptr; return false; }
    g_ortLoaded = true;
    return true;
}

// ---- 构造/析构 ----
OnnxInference::OnnxInference(QObject *parent) : QObject(parent) {
    m_classNames = {"tomato","green_tomato","damaged","ripe","unripe"};
}
OnnxInference::~OnnxInference() {
    if (m_session)   { g_ort.ReleaseSession(reinterpret_cast<OrtSession*>(m_session)); }
    if (m_env)       { g_ort.ReleaseEnv(reinterpret_cast<OrtEnv*>(m_env)); }
    if (m_memoryInfo){ g_ort.ReleaseMemoryInfo(reinterpret_cast<OrtMemoryInfo*>(m_memoryInfo)); }
    m_loaded = false;
}
bool OnnxInference::isLoaded() const { return m_loaded; }
int  OnnxInference::inputWidth()  const { return m_inputW; }
int  OnnxInference::inputHeight() const { return m_inputH; }
QStringList OnnxInference::classNames() const { return m_classNames; }

bool OnnxInference::isOnnxRuntimeAvailable() {
    if (s_ortChecked) return s_ortAvailable;
    s_ortChecked = true;
    s_ortAvailable = initOrtLibrary();
    return s_ortAvailable;
}

// ---- 加载模型 ----
bool OnnxInference::loadModel(const QString& modelPath)
{
    m_loaded = false;
    if (!initOrtLibrary()) { emit logMessage("找不到 onnxruntime.dll，请 pip install onnxruntime"); return false; }
    if (!QFile::exists(modelPath)) { emit logMessage("模型文件不存在: "+modelPath); return false; }

    OrtStatus* st = g_ort.CreateEnv(2, "Tomato", reinterpret_cast<OrtEnv**>(&m_env));
    if (st) { emit logMessage(QString("CreateEnv: %1").arg(g_ort.GetErrorMessage(st))); g_ort.ReleaseStatus(st); return false; }

    OrtSessionOptions* opts = nullptr;
    g_ort.CreateSessionOptions(&opts);
    g_ort.SetSessionGraphOptimizationLevel(opts, 1);
    g_ort.SetIntraOpNumThreads(opts, 4);

    QByteArray pathBytes = modelPath.toUtf8();
    st = g_ort.CreateSession(reinterpret_cast<OrtEnv*>(m_env), pathBytes.constData(), opts, reinterpret_cast<OrtSession**>(&m_session));
    g_ort.ReleaseSessionOptions(opts);
    if (st) { emit logMessage(QString("加载失败: %1").arg(g_ort.GetErrorMessage(st))); g_ort.ReleaseStatus(st); return false; }

    size_t nIn = 0, nOut = 0;
    g_ort.GetInputCount(reinterpret_cast<OrtSession*>(m_session), &nIn);
    g_ort.GetOutputCount(reinterpret_cast<OrtSession*>(m_session), &nOut);
    g_ort.GetAllocatorWithDefaultOptions(reinterpret_cast<OrtAllocator**>(&m_allocator));

    if (nIn > 0) {
        OrtTypeInfo* ti = nullptr;
        g_ort.GetInputTypeInfo(reinterpret_cast<OrtSession*>(m_session), 0, &ti);
        const OrtTensorTypeAndShapeInfo* tsi = nullptr;
        g_ort.CastTypeInfoToTensorInfo(ti, &tsi);
        size_t dc = 0; g_ort.GetDimensionsCount(tsi, &dc);
        if (dc >= 4) { std::vector<int64_t> d(dc); g_ort.GetDimensions(tsi, d.data(), dc); m_inputH = (int)d[2]; m_inputW = (int)d[3]; }
        g_ort.ReleaseTensorTypeAndShapeInfo(const_cast<OrtTensorTypeAndShapeInfo*>(tsi));
        g_ort.ReleaseTypeInfo(ti);
    }
    g_ort.CreateCpuMemoryInfo(0,0,reinterpret_cast<OrtMemoryInfo**>(&m_memoryInfo));
    m_loaded = true;
    emit logMessage(QString("ONNX 模型已加载 (%1x%2)").arg(m_inputW).arg(m_inputH));
    return true;
}

// ---- 预处理 ----
QImage OnnxInference::preprocess(const QImage& image)
{
    int iw = image.width(), ih = image.height();
    float s = qMin((float)m_inputW/iw, (float)m_inputH/ih);
    int nw = (int)(iw*s), nh = (int)(ih*s);
    QImage pad(m_inputW, m_inputH, QImage::Format_RGB888);
    pad.fill(QColor(114,114,114));
    QPainter p(&pad); p.drawImage((m_inputW-nw)/2, (m_inputH-nh)/2, image.scaled(nw,nh,Qt::IgnoreAspectRatio,Qt::SmoothTransformation)); p.end();
    return pad;
}

// ---- 推理 ----
QVector<DetectionResult> OnnxInference::detect(const QImage& image, float ct, float iot)
{
    QVector<DetectionResult> r;
    if (!m_loaded || !g_ortLoaded) return r;

    QImage pp = preprocess(image);
    int ch=3, ts=ch*m_inputH*m_inputW;
    std::vector<float> idata(ts);
    for(int c=0;c<ch;c++) for(int y=0;y<m_inputH;y++) for(int x=0;x<m_inputW;x++) {
        QRgb px=pp.pixel(x,y); float v=0;
        if(c==0)v=qRed(px)/255.f; else if(c==1)v=qGreen(px)/255.f; else v=qBlue(px)/255.f;
        idata[c*m_inputH*m_inputW + y*m_inputW + x] = v;
    }

    std::vector<int64_t> ish={1,3,m_inputH,m_inputW};
    OrtValue* itv=nullptr;
    g_ort.CreateTensorWithDataAsOrtValue(reinterpret_cast<OrtMemoryInfo*>(m_memoryInfo), idata.data(), ts*sizeof(float), ish.data(),4, ONNX_TENSOR_ELEMENT_DATA_TYPE_FLOAT, &itv);

    char* iname=nullptr; g_ort.GetInputName(reinterpret_cast<OrtSession*>(m_session),0,reinterpret_cast<OrtAllocator*>(m_allocator),&iname);
    const char* inames[]={iname};

    size_t no=0; g_ort.GetOutputCount(reinterpret_cast<OrtSession*>(m_session),&no);
    std::vector<const char*> onames(no);
    for(size_t i=0;i<no;i++){char*on=nullptr; g_ort.GetOutputName(reinterpret_cast<OrtSession*>(m_session),i,reinterpret_cast<OrtAllocator*>(m_allocator),&on); onames[i]=on;}

    OrtRunOptions* ro=nullptr; g_ort.CreateRunOptions(&ro);
    std::vector<OrtValue*> outs(no,nullptr);
    const OrtValue* itvs[]={itv};
    OrtStatus* st=g_ort.Run(reinterpret_cast<OrtSession*>(m_session),ro,inames,itvs,1,onames.data(),no,outs.data());
    g_ort.ReleaseRunOptions(ro);
    if(st){g_ort.ReleaseStatus(st);g_ort.ReleaseValue(itv);return r;}

    if(no>0&&outs[0]){
        float* od=nullptr; g_ort.GetTensorMutableData(outs[0],reinterpret_cast<void**>(&od));
        OrtTensorTypeAndShapeInfo* si=nullptr; g_ort.GetTensorTypeAndShape(outs[0],&si);
        size_t dc=0; std::vector<int64_t> os; g_ort.GetDimensionsCount(si,&dc); os.resize(dc); g_ort.GetDimensions(si,os.data(),dc);
        int nc=80, na=8400;
        if(os.size()>=2){nc=(int)os[1]-4; na=(int)os[2];}
        g_ort.ReleaseTensorTypeAndShapeInfo(si);
        r=postprocess(od,na,nc,image.width(),image.height(),ct,iot);
    }
    g_ort.ReleaseValue(itv); for(auto*o:outs)if(o)g_ort.ReleaseValue(o);
    return r;
}

// ---- 后处理 + NMS ----
QVector<DetectionResult> OnnxInference::postprocess(const float* out, int na, int nc, int iw, int ih, float ct, float iot)
{
    QVector<DetectionResult> raw;
    int fl=nc+4;
    float g=qMin((float)m_inputW/iw,(float)m_inputH/ih);
    float px=(m_inputW-iw*g)*.5f, py=(m_inputH-ih*g)*.5f;
    for(int i=0;i<na;i++){
        const float* r=out+i*fl;
        float ms=0;int bc=0;
        for(int c=0;c<nc;c++){float s=r[4+c];if(s>ms){ms=s;bc=c;}}
        if(ms<ct)continue;
        float cx=r[0],cy=r[1],bw=r[2],bh=r[3];
        float x1=(cx-bw*.5f-px)/g, y1=(cy-bh*.5f-py)/g;
        float x2=(cx+bw*.5f-px)/g, y2=(cy+bh*.5f-py)/g;
        x1=qMax(0.f,qMin(x1,(float)iw)); y1=qMax(0.f,qMin(y1,(float)ih));
        x2=qMax(0.f,qMin(x2,(float)iw)); y2=qMax(0.f,qMin(y2,(float)ih));
        if(x2-x1<3||y2-y1<3)continue;
        DetectionResult d; d.box=QRect((int)x1,(int)y1,(int)(x2-x1),(int)(y2-y1)); d.confidence=ms; d.classId=bc;
        d.className=(bc<m_classNames.size())?m_classNames[bc]:QString("cls%1").arg(bc);
        raw.append(d);
    }
    QVector<int> keep=nms(raw,iot);
    QVector<DetectionResult> f;
    for(int k:keep)f.append(raw[k]);
    return f;
}

QVector<int> OnnxInference::nms(const QVector<DetectionResult>& dets, float iot)
{
    QVector<int> keep, idx(dets.size());
    for(int i=0;i<idx.size();i++) idx[i]=i;
    std::sort(idx.begin(),idx.end(),[&](int a,int b){return dets[a].confidence>dets[b].confidence;});
    QVector<bool> sup(dets.size(),false);
    for(int i=0;i<idx.size();i++){int a=idx[i];if(sup[a])continue;keep.append(a);
        for(int j=i+1;j<idx.size();j++){int b=idx[j];if(sup[b]||dets[a].classId!=dets[b].classId)continue;
            if(computeIoU(dets[a].box,dets[b].box)>iot)sup[b]=true;}}
    return keep;
}

float OnnxInference::computeIoU(const QRect& a, const QRect& b) {
    int x1=qMax(a.left(),b.left()), y1=qMax(a.top(),b.top()), x2=qMin(a.right(),b.right()), y2=qMin(a.bottom(),b.bottom());
    int iw=qMax(0,x2-x1), ih=qMax(0,y2-y1);
    float I=iw*ih, A=a.width()*a.height(), B=b.width()*b.height();
    return I/(A+B-I+1e-6f);
}
