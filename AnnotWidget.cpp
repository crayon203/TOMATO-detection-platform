#include "AnnotWidget.h"
#include <QPainter>
#include <QPen>
#include <QFont>
#include <QMenu>
#include <QAction>
#include <QDir>
#include <QDirIterator>
#include <QFileInfo>
#include <QFile>
#include <QTextStream>
#include <QImage>
#include <QProcess>
#include <QTemporaryFile>
#include <QCoreApplication>
#include <cmath>

static const int HANDLE_SIZE = 8;

AnnotWidget::AnnotWidget(QWidget *parent) : QWidget(parent)
{
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);

    // 初始化默认类别
    m_categories << "tomato" << "green_tomato" << "damaged";
}

// =============================================================
//  图片加载（QImage → QPixmap → Python PIL 回退）
// =============================================================
QPixmap AnnotWidget::loadImageRobust(const QString& filePath)
{
    // 1) 尝试 QImage（内置支持 PNG/BMP/PPM/XBM/XPM）
    QImage img(filePath);
    if (!img.isNull())
        return QPixmap::fromImage(img);

    // 2) 尝试 QPixmap（可能使用原生 Windows API）
    QPixmap pix(filePath);
    if (!pix.isNull())
        return pix;

    // 3) Python PIL 回退 — 支持几乎所有格式（WebP, TIFF, HEIC 等）
    QString pyScript = QCoreApplication::applicationDirPath() + "/convert_img.py";
    if (!QFile::exists(pyScript)) {
        // 回退到源目录
        pyScript = QDir::currentPath() + "/convert_img.py";
    }
    if (!QFile::exists(pyScript)) {
        return QPixmap(); // 彻底失败
    }

    QProcess proc;
    proc.start("python", {pyScript, filePath});
    if (!proc.waitForStarted(5000))
        return QPixmap();
    proc.waitForFinished(10000);

    if (proc.exitCode() != 0)
        return QPixmap();

    QByteArray rawData = proc.readAllStandardOutput();
    if (rawData.size() < 8)
        return QPixmap();

    // 解析: 前 4 字节 = 宽度 (uint32 LE), 接下来 4 字节 = 高度 (uint32 LE)
    quint32 w = *reinterpret_cast<const quint32*>(rawData.constData());
    quint32 h = *reinterpret_cast<const quint32*>(rawData.constData() + 4);
    const uchar* pixels = reinterpret_cast<const uchar*>(rawData.constData() + 8);

    if (w == 0 || h == 0 || w > 20000 || h > 20000)
        return QPixmap();

    QImage pilImage(pixels, w, h, QImage::Format_RGBA8888);
    // 深拷贝（原始数据是临时的 QByteArray）
    QImage deepCopy = pilImage.copy();
    return QPixmap::fromImage(deepCopy);
}

// =============================================================
//  图像设置
// =============================================================
void AnnotWidget::setImage(const QPixmap &pix, const QString &path)
{
    m_pix = pix;
    m_imagePath = path;
    m_boxes.clear();
    m_confidences.clear();
    m_boxCategories.clear();
    m_selectedBox = -1;
    m_mode = Idle;
    m_undoStack.clear();
    m_redoStack.clear();
    m_zoomFactor = 1.0;
    m_rotationAngle = 0.0;
    m_panOffset = QPointF(0, 0);
    update();
}

QPixmap AnnotWidget::getImage() const { return m_pix; }
QString AnnotWidget::getImagePath() const { return m_imagePath; }

QVector<QRect>& AnnotWidget::getBoxes() { return m_boxes; }

void AnnotWidget::clearBoxes()
{
    if (m_boxes.isEmpty()) return;
    pushUndo("清除所有标注框");
    m_boxes.clear();
    m_confidences.clear();
    m_boxCategories.clear();
    m_selectedBox = -1;
    m_mode = Idle;
    update();
    emit boxesChanged();
}

// =============================================================
//  置信度
// =============================================================
QVector<float>& AnnotWidget::getConfidences() { return m_confidences; }

void AnnotWidget::setConfidences(const QVector<float>& confs)
{
    m_confidences = confs;
    update();
}

// =============================================================
//  类别管理
// =============================================================
QStringList& AnnotWidget::getCategories() { return m_categories; }

void AnnotWidget::setCategories(const QStringList& cats)
{
    m_categories = cats;
    if (m_currentCategory >= m_categories.size())
        m_currentCategory = 0;
}

void AnnotWidget::addCategory(const QString& name)
{
    if (!name.isEmpty() && !m_categories.contains(name)) {
        m_categories.append(name);
        emit categoryChanged(m_currentCategory);
    }
}

void AnnotWidget::removeCategory(int index)
{
    if (index >= 0 && index < m_categories.size() && m_categories.size() > 1) {
        m_categories.removeAt(index);
        for (int i = 0; i < m_boxCategories.size(); ++i) {
            if (m_boxCategories[i] == index)
                m_boxCategories[i] = 0;
            else if (m_boxCategories[i] > index)
                m_boxCategories[i]--;
        }
        if (m_currentCategory >= m_categories.size())
            m_currentCategory = m_categories.size() - 1;
        update();
    }
}

int AnnotWidget::getCurrentCategory() const { return m_currentCategory; }

void AnnotWidget::setCurrentCategory(int index)
{
    if (index >= 0 && index < m_categories.size()) {
        m_currentCategory = index;
        emit categoryChanged(index);
    }
}

QVector<int>& AnnotWidget::getBoxCategories() { return m_boxCategories; }

void AnnotWidget::setBoxCategory(int boxIndex, int categoryIndex)
{
    if (boxIndex >= 0 && boxIndex < m_boxes.size() && categoryIndex < m_categories.size()) {
        pushUndo("修改框类别");
        m_boxCategories[boxIndex] = categoryIndex;
        update();
        emit boxesChanged();
    }
}

// =============================================================
//  框选中 & 删除
// =============================================================
int AnnotWidget::selectedBoxIndex() const { return m_selectedBox; }

void AnnotWidget::selectBox(int index)
{
    if (index >= 0 && index < m_boxes.size()) {
        m_selectedBox = index;
        update();
        emit boxSelected(index);
    } else {
        m_selectedBox = -1;
        update();
        emit boxDeselected();
    }
}

void AnnotWidget::deleteSelectedBox()
{
    if (m_selectedBox >= 0 && m_selectedBox < m_boxes.size()) {
        pushUndo("删除标注框");
        m_boxes.removeAt(m_selectedBox);
        if (m_selectedBox < m_boxCategories.size())
            m_boxCategories.removeAt(m_selectedBox);
        if (m_selectedBox < m_confidences.size())
            m_confidences.removeAt(m_selectedBox);
        m_selectedBox = -1;
        update();
        emit boxesChanged();
    }
}

// =============================================================
//  撤销/重做
// =============================================================
void AnnotWidget::pushUndo(const QString& desc)
{
    UndoAction action;
    action.description = desc;
    action.boxes = m_boxes;
    action.categories = m_boxCategories;
    action.confidences = m_confidences;

    m_undoStack.push(action);
    if (m_undoStack.size() > m_undoLimit)
        m_undoStack.removeFirst();
    m_redoStack.clear();
}

void AnnotWidget::undo()
{
    if (m_undoStack.isEmpty()) return;

    UndoAction redoAction;
    redoAction.description = m_undoStack.top().description;
    redoAction.boxes = m_boxes;
    redoAction.categories = m_boxCategories;
    redoAction.confidences = m_confidences;
    m_redoStack.push(redoAction);

    UndoAction action = m_undoStack.pop();
    m_boxes = action.boxes;
    m_boxCategories = action.categories;
    m_confidences = action.confidences;
    m_selectedBox = -1;
    update();
    emit boxesChanged();
    emit statusMessage("撤销: " + action.description);
}

void AnnotWidget::redo()
{
    if (m_redoStack.isEmpty()) return;

    UndoAction undoAction;
    undoAction.description = m_redoStack.top().description;
    undoAction.boxes = m_boxes;
    undoAction.categories = m_boxCategories;
    undoAction.confidences = m_confidences;
    m_undoStack.push(undoAction);

    UndoAction action = m_redoStack.pop();
    m_boxes = action.boxes;
    m_boxCategories = action.categories;
    m_confidences = action.confidences;
    m_selectedBox = -1;
    update();
    emit boxesChanged();
    emit statusMessage("重做: " + action.description);
}

bool AnnotWidget::canUndo() const { return !m_undoStack.isEmpty(); }
bool AnnotWidget::canRedo() const { return !m_redoStack.isEmpty(); }

// =============================================================
//  图像视图操作
// =============================================================
qreal AnnotWidget::zoomFactor() const { return m_zoomFactor; }
qreal AnnotWidget::rotationAngle() const { return m_rotationAngle; }

void AnnotWidget::zoomIn()
{
    m_zoomFactor = qMin(m_zoomFactor * 1.2, 10.0);
    update();
    emit viewChanged(m_rotationAngle, m_zoomFactor, m_panOffset);
}

void AnnotWidget::zoomOut()
{
    m_zoomFactor = qMax(m_zoomFactor / 1.2, 0.1);
    update();
    emit viewChanged(m_rotationAngle, m_zoomFactor, m_panOffset);
}

void AnnotWidget::zoomFit()
{
    m_zoomFactor = 1.0;
    m_panOffset = QPointF(0, 0);
    m_rotationAngle = 0.0;
    update();
    emit viewChanged(m_rotationAngle, m_zoomFactor, m_panOffset);
}

void AnnotWidget::rotateImage(qreal angle)
{
    setRotationAngle(m_rotationAngle + angle);
}

void AnnotWidget::setRotationAngle(qreal degrees)
{
    // 归一化到 [0, 360)
    m_rotationAngle = std::fmod(degrees, 360.0);
    if (m_rotationAngle < 0)
        m_rotationAngle += 360.0;
    update();
    emit viewChanged(m_rotationAngle, m_zoomFactor, m_panOffset);
}

void AnnotWidget::panBy(qreal dx, qreal dy)
{
    m_panOffset += QPointF(dx, dy);
    update();
    emit viewChanged(m_rotationAngle, m_zoomFactor, m_panOffset);
}

void AnnotWidget::resetView() { zoomFit(); }

// =============================================================
//  数据集加载
// =============================================================
void AnnotWidget::loadYoloDataset(const QString& dirPath)
{
    m_datasetImages.clear();
    m_datasetLabeled.clear();
    m_datasetIndex = -1;

    QDir dir(dirPath);

    // 优先级搜索：train/images → test/images → valid/images → images → 全部递归
    QStringList imageDirs;
    imageDirs << dirPath + "/train/images"
              << dirPath + "/test/images"
              << dirPath + "/valid/images"
              << dirPath + "/images";

    QStringList filters;
    filters << "*.jpg" << "*.jpeg" << "*.png" << "*.bmp" << "*.tiff" << "*.tif" << "*.webp";

    bool foundAny = false;
    for (const QString& imgDir : imageDirs) {
        if (QDir(imgDir).exists()) {
            QFileInfoList files = QDir(imgDir).entryInfoList(filters, QDir::Files);
            for (const QFileInfo& fi : files)
                m_datasetImages.append(fi.absoluteFilePath());
            foundAny = true;
        }
    }

    // 递归扫描整个目录
    if (!foundAny) {
        QDirIterator it(dirPath, filters, QDir::Files, QDirIterator::Subdirectories);
        while (it.hasNext())
            m_datasetImages.append(it.next());
    }

    m_datasetImages.sort();

    // 构建标注状态列表
    for (const QString& imgPath : m_datasetImages) {
        m_datasetLabeled.append(hasLabelFile(imgPath));
    }

    emit statusMessage(QString("加载数据集: %1 张图片").arg(m_datasetImages.size()));

    // 自动加载第一张图片
    if (!m_datasetImages.isEmpty())
        navigateToImage(0);
}

bool AnnotWidget::hasLabelFile(const QString& imgPath) const
{
    QString labelPath = imgPath;
    labelPath.replace("/images/", "/labels/");
    labelPath.replace("\\images\\", "\\labels\\");

    // 如果没有 images 子目录，labels 可能在图片同目录
    if (labelPath == imgPath) {
        for (const QString& ext : {".jpg", ".jpeg", ".png", ".bmp", ".tiff", ".tif", ".webp",
                                    ".JPG", ".JPEG", ".PNG", ".BMP", ".TIFF", ".TIF", ".WEBP"}) {
            if (labelPath.endsWith(ext)) {
                labelPath = labelPath.left(labelPath.length() - ext.length()) + ".txt";
                break;
            }
        }
        return QFile::exists(labelPath);
    }

    for (const QString& ext : {".jpg", ".jpeg", ".png", ".bmp", ".tiff", ".tif", ".webp",
                                ".JPG", ".JPEG", ".PNG", ".BMP", ".TIFF", ".TIF", ".WEBP"}) {
        if (labelPath.endsWith(ext)) {
            labelPath = labelPath.left(labelPath.length() - ext.length()) + ".txt";
            break;
        }
    }
    return QFile::exists(labelPath);
}

QStringList AnnotWidget::getDatasetImages() const { return m_datasetImages; }
int AnnotWidget::currentDatasetIndex() const { return m_datasetIndex; }
int AnnotWidget::datasetImageCount() const { return m_datasetImages.size(); }
bool AnnotWidget::hasPrevImage() const { return m_datasetIndex > 0; }
bool AnnotWidget::hasNextImage() const { return m_datasetIndex >= 0 && m_datasetIndex < m_datasetImages.size() - 1; }

int AnnotWidget::labeledImageCount() const
{
    int count = 0;
    for (bool b : m_datasetLabeled) if (b) count++;
    return count;
}

bool AnnotWidget::currentImageHasLabel() const
{
    if (m_datasetIndex < 0 || m_datasetIndex >= m_datasetLabeled.size()) return false;
    return m_datasetLabeled[m_datasetIndex];
}

void AnnotWidget::navigateToImage(int index)
{
    if (index < 0 || index >= m_datasetImages.size()) return;

    QString path = m_datasetImages[index];
    QPixmap pix = loadImageRobust(path);
    if (pix.isNull()) {
        emit statusMessage(QString("无法加载: %1").arg(path));
        return;
    }

    // 加载新图片数据（不重置标注栈）
    m_pix = pix;
    m_imagePath = path;
    m_datasetIndex = index;
    m_boxes.clear();
    m_confidences.clear();
    m_boxCategories.clear();
    m_selectedBox = -1;
    m_mode = Idle;
    m_zoomFactor = 1.0;
    m_rotationAngle = 0.0;
    m_panOffset = QPointF(0, 0);

    // 清空仅当前图片的 undo/redo（不影响全局数据集状态）
    m_undoStack.clear();
    m_redoStack.clear();

    // 加载标注
    QString labelPath = findLabelPath(path);
    if (QFile::exists(labelPath)) {
        QFile f(labelPath);
        if (f.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&f);
            while (!in.atEnd()) {
                QString line = in.readLine().trimmed();
                if (line.isEmpty()) continue;
                QStringList parts = line.split(" ", Qt::SkipEmptyParts);
                if (parts.size() < 5) continue;
                int classId = parts[0].toInt();
                double cx = parts[1].toDouble();
                double cy = parts[2].toDouble();
                double bw = parts[3].toDouble();
                double bh = parts[4].toDouble();
                int w = bw * pix.width();
                int h = bh * pix.height();
                int x = cx * pix.width() - w / 2;
                int y = cy * pix.height() - h / 2;
                m_boxes.append(QRect(qMax(0, x), qMax(0, y), w, h));
                m_boxCategories.append(classId);
                m_confidences.append(0.0f);
            }
            f.close();
            emit statusMessage(QString("已有标注: %1 个框").arg(m_boxes.size()));
        }
    } else {
        emit statusMessage(QString("无标注文件"));
    }

    update();

    QFileInfo fi(path);
    emit boxesChanged();
}

QString AnnotWidget::findLabelPath(const QString& imgPath) const
{
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
    return labelPath;
}

// =============================================================
//  辅助计算函数
// =============================================================
QRect AnnotWidget::getImageFitRect() const
{
    if (m_pix.isNull()) return QRect();

    QSize widgetSize = rect().size();
    QPixmap scaled = m_pix.scaled(widgetSize * m_zoomFactor,
                                   Qt::KeepAspectRatio,
                                   Qt::SmoothTransformation);
    QRect fitRect = scaled.rect();
    fitRect.moveCenter(rect().center());
    fitRect.translate(m_panOffset.toPoint());
    return fitRect;
}

QRect AnnotWidget::imageToWidget(const QRect& r) const
{
    QRect fitRect = getImageFitRect();
    if (fitRect.isEmpty()) return QRect();
    double scaleW = (double)fitRect.width() / m_pix.width();
    double scaleH = (double)fitRect.height() / m_pix.height();
    return QRect(
        fitRect.x() + r.x() * scaleW,
        fitRect.y() + r.y() * scaleH,
        r.width() * scaleW,
        r.height() * scaleH);
}

QRect AnnotWidget::widgetToImage(const QRect& r) const
{
    QRect fitRect = getImageFitRect();
    if (fitRect.isEmpty()) return QRect();
    double scaleW = (double)m_pix.width() / fitRect.width();
    double scaleH = (double)m_pix.height() / fitRect.height();
    int x = qMax(0, qMin((int)((r.x() - fitRect.x()) * scaleW), m_pix.width() - 1));
    int y = qMax(0, qMin((int)((r.y() - fitRect.y()) * scaleH), m_pix.height() - 1));
    int w = qMax(1, qMin((int)(r.width() * scaleW), m_pix.width() - x));
    int h = qMax(1, qMin((int)(r.height() * scaleH), m_pix.height() - y));
    return QRect(x, y, w, h);
}

QRect AnnotWidget::getScaledBoxRect(const QRect& origBox) const
{
    QRect fitRect = getImageFitRect();
    if (fitRect.isEmpty()) return QRect();
    double scaleW = (double)fitRect.width() / m_pix.width();
    double scaleH = (double)fitRect.height() / m_pix.height();
    return QRect(
        fitRect.x() + origBox.x() * scaleW,
        fitRect.y() + origBox.y() * scaleH,
        origBox.width() * scaleW,
        origBox.height() * scaleH);
}

int AnnotWidget::boxAtPoint(const QPoint& pos) const
{
    // 从上层（最后画的）开始检查，这样重叠时优先选中上层框
    for (int i = m_boxes.size() - 1; i >= 0; --i) {
        QRect widgetRect = getScaledBoxRect(m_boxes[i]);
        // 扩大 5 像素便于选中
        QRect hitRect = widgetRect.adjusted(-5, -5, 5, 5);
        if (hitRect.contains(pos))
            return i;
    }
    return -1;
}

AnnotWidget::ResizeHandle AnnotWidget::hitTest(const QPoint& pos) const
{
    if (m_selectedBox < 0 || m_selectedBox >= m_boxes.size())
        return None;

    QRect boxRect = getScaledBoxRect(m_boxes[m_selectedBox]);

    // 四角
    if (QRect(boxRect.topLeft()     - QPoint(HANDLE_SIZE/2, HANDLE_SIZE/2),
              QSize(HANDLE_SIZE, HANDLE_SIZE)).contains(pos)) return TopLeft;
    if (QRect(boxRect.topRight()    - QPoint(HANDLE_SIZE/2, HANDLE_SIZE/2),
              QSize(HANDLE_SIZE, HANDLE_SIZE)).contains(pos)) return TopRight;
    if (QRect(boxRect.bottomLeft()  - QPoint(HANDLE_SIZE/2, HANDLE_SIZE/2),
              QSize(HANDLE_SIZE, HANDLE_SIZE)).contains(pos)) return BottomLeft;
    if (QRect(boxRect.bottomRight() - QPoint(HANDLE_SIZE/2, HANDLE_SIZE/2),
              QSize(HANDLE_SIZE, HANDLE_SIZE)).contains(pos)) return BottomRight;

    // 四边中点
    QPoint topMid((boxRect.left()+boxRect.right())/2, boxRect.top());
    QPoint botMid((boxRect.left()+boxRect.right())/2, boxRect.bottom());
    QPoint leftMid(boxRect.left(), (boxRect.top()+boxRect.bottom())/2);
    QPoint rightMid(boxRect.right(), (boxRect.top()+boxRect.bottom())/2);

    if (QRect(topMid  - QPoint(HANDLE_SIZE/2, HANDLE_SIZE/2), QSize(HANDLE_SIZE, HANDLE_SIZE)).contains(pos)) return Top;
    if (QRect(botMid  - QPoint(HANDLE_SIZE/2, HANDLE_SIZE/2), QSize(HANDLE_SIZE, HANDLE_SIZE)).contains(pos)) return Bottom;
    if (QRect(leftMid  - QPoint(HANDLE_SIZE/2, HANDLE_SIZE/2), QSize(HANDLE_SIZE, HANDLE_SIZE)).contains(pos)) return Left;
    if (QRect(rightMid - QPoint(HANDLE_SIZE/2, HANDLE_SIZE/2), QSize(HANDLE_SIZE, HANDLE_SIZE)).contains(pos)) return Right;

    return None;
}

QRect AnnotWidget::getHandleRect(const QRect& boxRect, ResizeHandle handle) const
{
    QPoint pt;
    switch (handle) {
    case TopLeft:     pt = boxRect.topLeft(); break;
    case Top:         pt = QPoint((boxRect.left()+boxRect.right())/2, boxRect.top()); break;
    case TopRight:    pt = boxRect.topRight(); break;
    case Left:        pt = QPoint(boxRect.left(), (boxRect.top()+boxRect.bottom())/2); break;
    case Right:       pt = QPoint(boxRect.right(), (boxRect.top()+boxRect.bottom())/2); break;
    case BottomLeft:  pt = boxRect.bottomLeft(); break;
    case Bottom:      pt = QPoint((boxRect.left()+boxRect.right())/2, boxRect.bottom()); break;
    case BottomRight: pt = boxRect.bottomRight(); break;
    default: return QRect();
    }
    return QRect(pt.x() - HANDLE_SIZE/2, pt.y() - HANDLE_SIZE/2, HANDLE_SIZE, HANDLE_SIZE);
}

// =============================================================
//  绘制（修复：标签背景不挡其他框）
// =============================================================
void AnnotWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.fillRect(rect(), QColor(50, 50, 50));

    if (m_pix.isNull()) return;

    QRect fitRect = getImageFitRect();
    QPixmap scaled = m_pix.scaled(fitRect.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);

    if (qFuzzyIsNull(m_rotationAngle)) {
        p.drawPixmap(fitRect, scaled);
    } else {
        p.save();
        p.translate(fitRect.center());
        p.rotate(m_rotationAngle);
        p.translate(-fitRect.center());
        p.drawPixmap(fitRect, scaled);
        p.restore();
    }

    double scaleW = (double)fitRect.width() / m_pix.width();
    double scaleH = (double)fitRect.height() / m_pix.height();

    // ============ 第一轮：绘制所有框线（先不画标签）============
    struct LabelInfo {
        int x, y, w, h;        // label rect
        QString text;
        int boxIndex;
    };
    QVector<LabelInfo> labels;

    for (int i = 0; i < m_boxes.size(); ++i) {
        const QRect& r = m_boxes[i];
        int bx = fitRect.x() + r.x() * scaleW;
        int by = fitRect.y() + r.y() * scaleH;
        int bw = r.width() * scaleW;
        int bh = r.height() * scaleH;

        // 选中框用不同颜色
        if (i == m_selectedBox) {
            p.setPen(QPen(QColor(0, 255, 255), 3, Qt::DashLine));
            p.setBrush(Qt::NoBrush);
        } else {
            int catIdx = (i < m_boxCategories.size()) ? m_boxCategories[i] : 0;
            QColor colors[] = {Qt::red, QColor(0,200,0), Qt::cyan, Qt::magenta,
                              QColor(0,100,255), QColor(200,200,0), QColor(255,140,0)};
            p.setPen(QPen(colors[catIdx % 7], 2.5));
            p.setBrush(Qt::NoBrush);
        }
        p.drawRect(bx, by, bw, bh);

        // 收集标签信息
        if (i < m_boxCategories.size() && m_boxCategories[i] < m_categories.size()) {
            LabelInfo lbl;
            lbl.text = m_categories[m_boxCategories[i]];
            lbl.x = bx;
            lbl.y = by - 16;
            lbl.w = p.fontMetrics().horizontalAdvance(lbl.text) + 8;
            lbl.h = 15;
            lbl.boxIndex = i;
            // 边界检查：标签不超出图像区域
            if (lbl.y < fitRect.top()) lbl.y = by + bh + 2;
            if (lbl.x + lbl.w > fitRect.right()) lbl.x = fitRect.right() - lbl.w;
            if (lbl.x < fitRect.left()) lbl.x = fitRect.left();
            labels.append(lbl);
        }

        // 收集置信度标签
        if (i < m_confidences.size() && m_confidences[i] > 0.001f) {
            LabelInfo clbl;
            clbl.text = QString::number(m_confidences[i], 'f', 2);
            clbl.x = bx;
            clbl.y = by + bh + 1;
            clbl.w = p.fontMetrics().horizontalAdvance(clbl.text) + 6;
            clbl.h = 15;
            clbl.boxIndex = i;
            if (clbl.y + clbl.h > fitRect.bottom()) clbl.y = by - 16;
            if (clbl.x + clbl.w > fitRect.right()) clbl.x = fitRect.right() - clbl.w;
            if (clbl.x < fitRect.left()) clbl.x = fitRect.left();
            labels.append(clbl);
        }
    }

    // ============ 第二轮：绘制所有标签（在所有框线画完后）============
    p.setFont(QFont("Arial", 9, QFont::Bold));
    for (const LabelInfo& lbl : labels) {
        p.fillRect(lbl.x, lbl.y, lbl.w, lbl.h, QColor(0, 0, 0, 180));
        p.setPen(Qt::white);
        p.drawText(lbl.x + 3, lbl.y, lbl.w - 4, lbl.h, Qt::AlignVCenter, lbl.text);
        p.setPen(Qt::NoPen);
    }

    // ============ 第三轮：绘制选中框手柄（在最上层）============
    if (m_selectedBox >= 0 && m_selectedBox < m_boxes.size()) {
        const QRect& selR = m_boxes[m_selectedBox];
        int bx = fitRect.x() + selR.x() * scaleW;
        int by = fitRect.y() + selR.y() * scaleH;
        int bw = selR.width() * scaleW;
        int bh = selR.height() * scaleH;
        QRect boxRect(bx, by, bw, bh);

        for (int h = TopLeft; h <= BottomRight; ++h) {
            QRect hRect = getHandleRect(boxRect, (ResizeHandle)h);
            p.setPen(QPen(QColor(0,255,255), 1));
            p.setBrush(QColor(100,255,255));
            p.drawRect(hRect);
        }
    }

    // 正在绘制新框
    if (m_mode == Drawing) {
        p.setPen(QPen(Qt::cyan, 2, Qt::DashLine));
        p.setBrush(Qt::NoBrush);
        QRect drawRect(m_startPoint, m_endPoint);
        p.drawRect(drawRect.normalized());
    }
}

// =============================================================
//  鼠标事件 — 状态机
// =============================================================
void AnnotWidget::mousePressEvent(QMouseEvent *event)
{
    if (m_pix.isNull()) return;
    setFocus();

    QPoint pos = event->pos();

    // 中键或 Ctrl+左键 → 平移
    if (event->button() == Qt::MiddleButton ||
        (event->button() == Qt::LeftButton && event->modifiers() & Qt::ControlModifier)) {
        m_mode = Panning;
        m_startPoint = pos;
        setCursor(Qt::ClosedHandCursor);
        return;
    }

    if (event->button() == Qt::LeftButton) {
        // Shift+左键 → 强制新建标注框（绕过现有框选中，解决大框内小目标标注问题）
        bool forceNew = (event->modifiers() & Qt::ShiftModifier);

        if (!forceNew) {
            // 1. 检查缩放手柄
            ResizeHandle handle = hitTest(pos);
            if (handle != None) {
                pushUndo("调整标注框大小");  // 在修改前保存原始状态
                m_mode = Resizing;
                m_activeHandle = handle;
                m_interactingIndex = m_selectedBox;
                m_originalBox = m_boxes[m_selectedBox];
                m_startPoint = pos;
                return;
            }

            // 2. 检查是否点击了框内部 → 移动模式
            int idx = boxAtPoint(pos);
            if (idx >= 0) {
                selectBox(idx);
                pushUndo("移动标注框");  // 在修改前保存原始状态
                m_mode = Moving;
                m_interactingIndex = idx;
                m_originalBox = m_boxes[idx];
                m_startPoint = pos;
                return;
            }
        }

        // 3. 创建新框（空白区域或 Shift+点击）
        selectBox(-1);
        m_mode = Drawing;
        m_startPoint = pos;
        m_endPoint = pos;
    }
}

void AnnotWidget::mouseMoveEvent(QMouseEvent *event)
{
    QPoint pos = event->pos();

    if (m_mode == Drawing) {
        m_endPoint = pos;
        update();
    }
    else if (m_mode == Moving && m_interactingIndex >= 0) {
        QRect fitRect = getImageFitRect();
        if (fitRect.isEmpty()) return;
        double scaleW = (double)m_pix.width() / fitRect.width();
        double scaleH = (double)m_pix.height() / fitRect.height();
        QPoint delta = pos - m_startPoint;

        QRect newBox = m_originalBox;
        newBox.translate(delta.x() * scaleW, delta.y() * scaleH);

        // 边界检查
        if (newBox.left() < 0) newBox.moveLeft(0);
        if (newBox.top() < 0) newBox.moveTop(0);
        if (newBox.right() > m_pix.width()) newBox.moveRight(m_pix.width());
        if (newBox.bottom() > m_pix.height()) newBox.moveBottom(m_pix.height());

        m_boxes[m_interactingIndex] = newBox;
        update();
    }
    else if (m_mode == Resizing && m_interactingIndex >= 0) {
        QRect fitRect = getImageFitRect();
        if (fitRect.isEmpty()) return;
        double scaleW = (double)m_pix.width() / fitRect.width();
        double scaleH = (double)m_pix.height() / fitRect.height();
        QPoint delta = pos - m_startPoint;

        int dx = qRound(delta.x() * scaleW);
        int dy = qRound(delta.y() * scaleH);
        QRect newBox = m_originalBox;

        switch (m_activeHandle) {
        case TopLeft:
            newBox.setLeft(qMin(newBox.left() + dx, newBox.right() - 10));
            newBox.setTop(qMin(newBox.top() + dy, newBox.bottom() - 10));
            break;
        case Top:
            newBox.setTop(qMin(newBox.top() + dy, newBox.bottom() - 10));
            break;
        case TopRight:
            newBox.setRight(qMax(newBox.right() + dx, newBox.left() + 10));
            newBox.setTop(qMin(newBox.top() + dy, newBox.bottom() - 10));
            break;
        case Left:
            newBox.setLeft(qMin(newBox.left() + dx, newBox.right() - 10));
            break;
        case Right:
            newBox.setRight(qMax(newBox.right() + dx, newBox.left() + 10));
            break;
        case BottomLeft:
            newBox.setLeft(qMin(newBox.left() + dx, newBox.right() - 10));
            newBox.setBottom(qMax(newBox.bottom() + dy, newBox.top() + 10));
            break;
        case Bottom:
            newBox.setBottom(qMax(newBox.bottom() + dy, newBox.top() + 10));
            break;
        case BottomRight:
            newBox.setRight(qMax(newBox.right() + dx, newBox.left() + 10));
            newBox.setBottom(qMax(newBox.bottom() + dy, newBox.top() + 10));
            break;
        default: break;
        }

        newBox.setLeft(qMax(0, newBox.left()));
        newBox.setTop(qMax(0, newBox.top()));
        newBox.setRight(qMin(m_pix.width(), newBox.right()));
        newBox.setBottom(qMin(m_pix.height(), newBox.bottom()));

        m_boxes[m_interactingIndex] = newBox;
        update();
    }
    else if (m_mode == Panning) {
        QPoint delta = pos - m_startPoint;
        m_panOffset += delta;
        m_startPoint = pos;
        update();
    }
    else if (m_mode == Idle) {
        ResizeHandle h = hitTest(pos);
        if (h != None) {
            switch (h) {
            case TopLeft: case BottomRight: setCursor(Qt::SizeFDiagCursor); break;
            case TopRight: case BottomLeft: setCursor(Qt::SizeBDiagCursor); break;
            case Top: case Bottom: setCursor(Qt::SizeVerCursor); break;
            case Left: case Right: setCursor(Qt::SizeHorCursor); break;
            default: break;
            }
        } else if (boxAtPoint(pos) >= 0) {
            setCursor(Qt::SizeAllCursor);
        } else {
            setCursor(Qt::CrossCursor);
        }
    }
}

void AnnotWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MiddleButton ||
        (event->button() == Qt::LeftButton && m_mode == Panning)) {
        m_mode = Idle;
        setCursor(Qt::CrossCursor);
        return;
    }

    if (m_mode == Drawing) {
        m_mode = Idle;

        QRect fitRect = getImageFitRect();
        if (fitRect.isEmpty()) { update(); return; }
        double scaleW = (double)m_pix.width() / fitRect.width();
        double scaleH = (double)m_pix.height() / fitRect.height();

        QRect drawRect = QRect(m_startPoint, m_endPoint).normalized();

        // 最小 3 像素容忍
        if (drawRect.width() < 3 && drawRect.height() < 3) return;

        int x = qRound((drawRect.x() - fitRect.x()) * scaleW);
        int y = qRound((drawRect.y() - fitRect.y()) * scaleH);
        int w = qRound(drawRect.width() * scaleW);
        int h = qRound(drawRect.height() * scaleH);

        x = qMax(0, qMin(x, m_pix.width() - 1));
        y = qMax(0, qMin(y, m_pix.height() - 1));
        w = qMax(1, qMin(w, m_pix.width() - x));
        h = qMax(1, qMin(h, m_pix.height() - y));

        pushUndo("创建标注框");
        m_boxes.append(QRect(x, y, w, h));
        m_boxCategories.append(m_currentCategory);
        m_confidences.append(0.0f);
        m_selectedBox = m_boxes.size() - 1;
        update();
        emit boxesChanged();
        emit boxSelected(m_selectedBox);
    }
    else if (m_mode == Moving && m_interactingIndex >= 0) {
        if (m_boxes[m_interactingIndex] == m_originalBox) {
            // 没有实际移动，移除 mousePress 时压入的撤销条目
            if (!m_undoStack.isEmpty()) m_undoStack.pop();
        }
        m_mode = Idle;
        m_interactingIndex = -1;
        update();
        emit boxesChanged();
    }
    else if (m_mode == Resizing && m_interactingIndex >= 0) {
        if (m_boxes[m_interactingIndex] == m_originalBox) {
            // 没有实际调整大小，移除 mousePress 时压入的撤销条目
            if (!m_undoStack.isEmpty()) m_undoStack.pop();
        }
        m_mode = Idle;
        m_activeHandle = None;
        m_interactingIndex = -1;
        update();
        emit boxesChanged();
    }
}

// =============================================================
//  键盘事件
// =============================================================
void AnnotWidget::keyPressEvent(QKeyEvent *event)
{
    // 注意：撤销/重做由菜单栏 QAction 快捷键 (Ctrl+Z / Ctrl+Y) 统一处理，
    // 不在此处重复处理，避免快捷键双重触发导致重做失效。

    if (event->key() == Qt::Key_Delete || event->key() == Qt::Key_Backspace) {
        deleteSelectedBox();
    }
    else if (event->key() == Qt::Key_Escape) {
        selectBox(-1);
        m_mode = Idle;
        update();
    }
    else if (event->key() == Qt::Key_Plus || event->key() == Qt::Key_Equal) {
        zoomIn();
    }
    else if (event->key() == Qt::Key_Minus) {
        zoomOut();
    }
    // 方向键平移（Shift 加速 4 倍）
    else if (event->key() == Qt::Key_Left) {
        int step = (event->modifiers() & Qt::ShiftModifier) ? 160 : 40;
        panBy(step, 0);
    }
    else if (event->key() == Qt::Key_Right) {
        int step = (event->modifiers() & Qt::ShiftModifier) ? 160 : 40;
        panBy(-step, 0);
    }
    else if (event->key() == Qt::Key_Up) {
        int step = (event->modifiers() & Qt::ShiftModifier) ? 160 : 40;
        panBy(0, step);
    }
    else if (event->key() == Qt::Key_Down) {
        int step = (event->modifiers() & Qt::ShiftModifier) ? 160 : 40;
        panBy(0, -step);
    }
    // 旋转：BracketLeft = 左旋 15°, BracketRight = 右旋 15°
    // Shift 加倍到 45°
    else if (event->key() == Qt::Key_BracketLeft || event->key() == Qt::Key_BraceLeft) {
        qreal step = (event->modifiers() & Qt::ShiftModifier) ? 45.0 : 15.0;
        setRotationAngle(m_rotationAngle - step);
        emit statusMessage(QString("旋转: %1°").arg(m_rotationAngle, 0, 'f', 1));
    }
    else if (event->key() == Qt::Key_BracketRight || event->key() == Qt::Key_BraceRight) {
        qreal step = (event->modifiers() & Qt::ShiftModifier) ? 45.0 : 15.0;
        setRotationAngle(m_rotationAngle + step);
        emit statusMessage(QString("旋转: %1°").arg(m_rotationAngle, 0, 'f', 1));
    }
    else if (event->key() == Qt::Key_0) {
        zoomFit();
    }
    else {
        QWidget::keyPressEvent(event);
    }
}

// =============================================================
//  滚轮缩放
// =============================================================
void AnnotWidget::wheelEvent(QWheelEvent *event)
{
    if (event->modifiers() & Qt::ControlModifier) {
        if (event->angleDelta().y() > 0)
            zoomIn();
        else
            zoomOut();
    } else {
        QWidget::wheelEvent(event);
    }
}

// =============================================================
//  右键菜单
// =============================================================
void AnnotWidget::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);
    int idx = boxAtPoint(event->pos());

    if (idx >= 0) {
        selectBox(idx);
        QAction* delAction = menu.addAction("删除此标注框 (Delete)");
        menu.addSeparator();

        QMenu* catMenu = menu.addMenu("修改类别");
        for (int i = 0; i < m_categories.size(); ++i) {
            QAction* catAct = catMenu->addAction(m_categories[i]);
            catAct->setCheckable(true);
            if (idx < m_boxCategories.size() && m_boxCategories[idx] == i)
                catAct->setChecked(true);
            int catIdx = i;
            connect(catAct, &QAction::triggered, [this, idx, catIdx]() {
                setBoxCategory(idx, catIdx);
            });
        }

        menu.addSeparator();
        QAction* chosen = menu.exec(event->globalPos());
        if (chosen == delAction)
            deleteSelectedBox();
    } else {
        QAction* clearAction = menu.addAction("清除所有标注框");
        if (canUndo()) {
            menu.addSeparator();
            menu.addAction("撤销 (Ctrl+Z)")->setEnabled(false);
        }
        QAction* chosen = menu.exec(event->globalPos());
        if (chosen == clearAction)
            clearBoxes();
    }
}
