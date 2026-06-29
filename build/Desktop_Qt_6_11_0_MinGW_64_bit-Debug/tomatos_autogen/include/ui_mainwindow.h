/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.11.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "AnnotWidget.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionOpenImage;
    QAction *actionSave;
    QAction *actionUndo;
    QAction *actionRedo;
    QAction *actionZoomIn;
    QAction *actionZoomOut;
    QAction *actionZoomFit;
    QAction *actionExit;
    QAction *actionRotateLeft;
    QAction *actionRotateRight;
    QAction *actionResetView;
    QAction *actionAbout;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QTabWidget *tabWidget;
    QWidget *tabAnnot;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *categoryLayout;
    QLabel *lblCategory;
    QComboBox *cbCategory;
    QPushButton *btnAddCategory;
    QPushButton *btnDelCategory;
    QSpacerItem *catSpacer;
    QPushButton *btnOpenImage;
    QPushButton *btnSaveYolo;
    QPushButton *btnClearAnnot;
    QPushButton *btnDeleteBox;
    QSpacerItem *viewSpacer;
    QPushButton *btnPanLeft;
    QPushButton *btnPanUp;
    QPushButton *btnPanDown;
    QPushButton *btnPanRight;
    QSpacerItem *panZoomSpacer;
    QPushButton *btnZoomOut;
    QPushButton *btnZoomIn;
    QSpacerItem *zoomRotSpacer;
    QLabel *lblRotation;
    QDoubleSpinBox *edRotation;
    QPushButton *btnRotateLeft;
    QPushButton *btnRotateRight;
    QSpacerItem *rotResetSpacer;
    QPushButton *btnResetView;
    QPushButton *btnLoadDataset;
    QHBoxLayout *datasetNavLayout;
    QPushButton *btnPrevImage;
    QLabel *lblDatasetNav;
    QPushButton *btnNextImage;
    QPushButton *btnSaveAllLabels;
    AnnotWidget *widgetAnnot;
    QTableWidget *tableAnnotations;
    QWidget *tabTrain;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QLineEdit *edDataDir;
    QPushButton *btnSelectDataset;
    QHBoxLayout *splitLayout;
    QLabel *lblSplit;
    QSlider *sliderSplit;
    QSpinBox *spinSplit;
    QGridLayout *trainParamsGrid;
    QLabel *label_2;
    QSpinBox *edEpoch;
    QLabel *label_3;
    QSpinBox *edBatch;
    QLabel *label_4;
    QSpinBox *edImgsz;
    QLabel *lblLR;
    QDoubleSpinBox *edLR;
    QLabel *lblOptimizer;
    QComboBox *cbOptimizer;
    QLabel *lblWD;
    QDoubleSpinBox *edWD;
    QLabel *lblDevice;
    QComboBox *cbDevice;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *btnStartTrain;
    QPushButton *btnStopTrain;
    QLabel *label_5;
    QTextEdit *teTrainLog;
    QGroupBox *gbMetrics;
    QGridLayout *metricsGrid;
    QLabel *lblMAP;
    QLabel *lblMAP95;
    QLabel *lblPrecision;
    QLabel *lblRecall;
    QLabel *lblModelPath;
    QLabel *lblTrainTime;
    QPushButton *btnSaveTrainResult;
    QWidget *tabQuant;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_6;
    QLineEdit *edModelPath;
    QPushButton *btnSelectModel;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_7;
    QComboBox *cbQuantType;
    QLabel *lblQuantScheme;
    QComboBox *cbQuantScheme;
    QHBoxLayout *quantPathwayLayout;
    QLabel *lblQuantPathway;
    QComboBox *cbQuantPathway;
    QHBoxLayout *horizontalLayout_7;
    QPushButton *btnStartQuant;
    QPushButton *btnStopQuant;
    QLabel *label_8;
    QTextEdit *teQuantLog;
    QWidget *tabInfer;
    QVBoxLayout *verticalLayout_5;
    QHBoxLayout *inferEngineLayout;
    QLabel *lblInferEngine;
    QComboBox *cbInferEngine;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label_9;
    QLineEdit *edQuantModelPath;
    QPushButton *btnSelectQuantModel;
    QHBoxLayout *inferParamLayout;
    QLabel *lblConf;
    QDoubleSpinBox *edConfThresh;
    QLabel *lblIoU;
    QDoubleSpinBox *edIoUThresh;
    QHBoxLayout *horizontalLayout_9;
    QPushButton *btnOpenInferImage;
    QPushButton *btnRunInfer;
    QPushButton *btnBatchInfer;
    QPushButton *btnSaveInferResult;
    AnnotWidget *widgetInfer;
    QTableWidget *tableInferResults;
    QWidget *tabHistory;
    QVBoxLayout *historyLayout;
    QTabWidget *historyTabWidget;
    QWidget *historyTrainTab;
    QVBoxLayout *vboxLayout;
    QTableWidget *tableTrainHistory;
    QWidget *historyInferTab;
    QVBoxLayout *vboxLayout1;
    QTableWidget *tableInferHistory;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuEdit;
    QMenu *menuView;
    QMenu *menuHelp;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1100, 800);
        actionOpenImage = new QAction(MainWindow);
        actionOpenImage->setObjectName("actionOpenImage");
        actionSave = new QAction(MainWindow);
        actionSave->setObjectName("actionSave");
        actionUndo = new QAction(MainWindow);
        actionUndo->setObjectName("actionUndo");
        actionRedo = new QAction(MainWindow);
        actionRedo->setObjectName("actionRedo");
        actionZoomIn = new QAction(MainWindow);
        actionZoomIn->setObjectName("actionZoomIn");
        actionZoomOut = new QAction(MainWindow);
        actionZoomOut->setObjectName("actionZoomOut");
        actionZoomFit = new QAction(MainWindow);
        actionZoomFit->setObjectName("actionZoomFit");
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName("actionExit");
        actionRotateLeft = new QAction(MainWindow);
        actionRotateLeft->setObjectName("actionRotateLeft");
        actionRotateRight = new QAction(MainWindow);
        actionRotateRight->setObjectName("actionRotateRight");
        actionResetView = new QAction(MainWindow);
        actionResetView->setObjectName("actionResetView");
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName("actionAbout");
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName("verticalLayout");
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName("tabWidget");
        tabAnnot = new QWidget();
        tabAnnot->setObjectName("tabAnnot");
        verticalLayout_2 = new QVBoxLayout(tabAnnot);
        verticalLayout_2->setObjectName("verticalLayout_2");
        categoryLayout = new QHBoxLayout();
        categoryLayout->setObjectName("categoryLayout");
        lblCategory = new QLabel(tabAnnot);
        lblCategory->setObjectName("lblCategory");

        categoryLayout->addWidget(lblCategory);

        cbCategory = new QComboBox(tabAnnot);
        cbCategory->setObjectName("cbCategory");

        categoryLayout->addWidget(cbCategory);

        btnAddCategory = new QPushButton(tabAnnot);
        btnAddCategory->setObjectName("btnAddCategory");

        categoryLayout->addWidget(btnAddCategory);

        btnDelCategory = new QPushButton(tabAnnot);
        btnDelCategory->setObjectName("btnDelCategory");

        categoryLayout->addWidget(btnDelCategory);

        catSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        categoryLayout->addItem(catSpacer);

        btnOpenImage = new QPushButton(tabAnnot);
        btnOpenImage->setObjectName("btnOpenImage");

        categoryLayout->addWidget(btnOpenImage);

        btnSaveYolo = new QPushButton(tabAnnot);
        btnSaveYolo->setObjectName("btnSaveYolo");

        categoryLayout->addWidget(btnSaveYolo);

        btnClearAnnot = new QPushButton(tabAnnot);
        btnClearAnnot->setObjectName("btnClearAnnot");

        categoryLayout->addWidget(btnClearAnnot);

        btnDeleteBox = new QPushButton(tabAnnot);
        btnDeleteBox->setObjectName("btnDeleteBox");

        categoryLayout->addWidget(btnDeleteBox);

        viewSpacer = new QSpacerItem(20, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        categoryLayout->addItem(viewSpacer);

        btnPanLeft = new QPushButton(tabAnnot);
        btnPanLeft->setObjectName("btnPanLeft");
        btnPanLeft->setMaximumSize(QSize(32, 16777215));

        categoryLayout->addWidget(btnPanLeft);

        btnPanUp = new QPushButton(tabAnnot);
        btnPanUp->setObjectName("btnPanUp");
        btnPanUp->setMaximumSize(QSize(32, 16777215));

        categoryLayout->addWidget(btnPanUp);

        btnPanDown = new QPushButton(tabAnnot);
        btnPanDown->setObjectName("btnPanDown");
        btnPanDown->setMaximumSize(QSize(32, 16777215));

        categoryLayout->addWidget(btnPanDown);

        btnPanRight = new QPushButton(tabAnnot);
        btnPanRight->setObjectName("btnPanRight");
        btnPanRight->setMaximumSize(QSize(32, 16777215));

        categoryLayout->addWidget(btnPanRight);

        panZoomSpacer = new QSpacerItem(8, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        categoryLayout->addItem(panZoomSpacer);

        btnZoomOut = new QPushButton(tabAnnot);
        btnZoomOut->setObjectName("btnZoomOut");
        btnZoomOut->setMaximumSize(QSize(32, 16777215));

        categoryLayout->addWidget(btnZoomOut);

        btnZoomIn = new QPushButton(tabAnnot);
        btnZoomIn->setObjectName("btnZoomIn");
        btnZoomIn->setMaximumSize(QSize(32, 16777215));

        categoryLayout->addWidget(btnZoomIn);

        zoomRotSpacer = new QSpacerItem(8, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        categoryLayout->addItem(zoomRotSpacer);

        lblRotation = new QLabel(tabAnnot);
        lblRotation->setObjectName("lblRotation");

        categoryLayout->addWidget(lblRotation);

        edRotation = new QDoubleSpinBox(tabAnnot);
        edRotation->setObjectName("edRotation");
        edRotation->setMinimumSize(QSize(70, 0));
        edRotation->setDecimals(1);
        edRotation->setMinimum(0.000000000000000);
        edRotation->setMaximum(359.899999999999977);
        edRotation->setSingleStep(5.000000000000000);
        edRotation->setValue(0.000000000000000);

        categoryLayout->addWidget(edRotation);

        btnRotateLeft = new QPushButton(tabAnnot);
        btnRotateLeft->setObjectName("btnRotateLeft");
        btnRotateLeft->setMaximumSize(QSize(32, 16777215));

        categoryLayout->addWidget(btnRotateLeft);

        btnRotateRight = new QPushButton(tabAnnot);
        btnRotateRight->setObjectName("btnRotateRight");
        btnRotateRight->setMaximumSize(QSize(32, 16777215));

        categoryLayout->addWidget(btnRotateRight);

        rotResetSpacer = new QSpacerItem(4, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        categoryLayout->addItem(rotResetSpacer);

        btnResetView = new QPushButton(tabAnnot);
        btnResetView->setObjectName("btnResetView");

        categoryLayout->addWidget(btnResetView);

        btnLoadDataset = new QPushButton(tabAnnot);
        btnLoadDataset->setObjectName("btnLoadDataset");

        categoryLayout->addWidget(btnLoadDataset);


        verticalLayout_2->addLayout(categoryLayout);

        datasetNavLayout = new QHBoxLayout();
        datasetNavLayout->setObjectName("datasetNavLayout");
        btnPrevImage = new QPushButton(tabAnnot);
        btnPrevImage->setObjectName("btnPrevImage");
        btnPrevImage->setEnabled(false);
        btnPrevImage->setMaximumSize(QSize(90, 16777215));

        datasetNavLayout->addWidget(btnPrevImage);

        lblDatasetNav = new QLabel(tabAnnot);
        lblDatasetNav->setObjectName("lblDatasetNav");
        lblDatasetNav->setAlignment(Qt::AlignmentFlag::AlignCenter);

        datasetNavLayout->addWidget(lblDatasetNav);

        btnNextImage = new QPushButton(tabAnnot);
        btnNextImage->setObjectName("btnNextImage");
        btnNextImage->setEnabled(false);
        btnNextImage->setMaximumSize(QSize(90, 16777215));

        datasetNavLayout->addWidget(btnNextImage);

        btnSaveAllLabels = new QPushButton(tabAnnot);
        btnSaveAllLabels->setObjectName("btnSaveAllLabels");

        datasetNavLayout->addWidget(btnSaveAllLabels);


        verticalLayout_2->addLayout(datasetNavLayout);

        widgetAnnot = new AnnotWidget(tabAnnot);
        widgetAnnot->setObjectName("widgetAnnot");
        widgetAnnot->setMinimumSize(QSize(400, 300));

        verticalLayout_2->addWidget(widgetAnnot);

        tableAnnotations = new QTableWidget(tabAnnot);
        if (tableAnnotations->columnCount() < 6)
            tableAnnotations->setColumnCount(6);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableAnnotations->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableAnnotations->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableAnnotations->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableAnnotations->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableAnnotations->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableAnnotations->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        tableAnnotations->setObjectName("tableAnnotations");
        tableAnnotations->setMaximumSize(QSize(16777215, 150));
        tableAnnotations->setColumnCount(6);

        verticalLayout_2->addWidget(tableAnnotations);

        tabWidget->addTab(tabAnnot, QString());
        tabTrain = new QWidget();
        tabTrain->setObjectName("tabTrain");
        verticalLayout_3 = new QVBoxLayout(tabTrain);
        verticalLayout_3->setObjectName("verticalLayout_3");
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        label = new QLabel(tabTrain);
        label->setObjectName("label");

        horizontalLayout_2->addWidget(label);

        edDataDir = new QLineEdit(tabTrain);
        edDataDir->setObjectName("edDataDir");

        horizontalLayout_2->addWidget(edDataDir);

        btnSelectDataset = new QPushButton(tabTrain);
        btnSelectDataset->setObjectName("btnSelectDataset");

        horizontalLayout_2->addWidget(btnSelectDataset);


        verticalLayout_3->addLayout(horizontalLayout_2);

        splitLayout = new QHBoxLayout();
        splitLayout->setObjectName("splitLayout");
        lblSplit = new QLabel(tabTrain);
        lblSplit->setObjectName("lblSplit");

        splitLayout->addWidget(lblSplit);

        sliderSplit = new QSlider(tabTrain);
        sliderSplit->setObjectName("sliderSplit");
        sliderSplit->setMinimum(50);
        sliderSplit->setMaximum(95);
        sliderSplit->setValue(80);
        sliderSplit->setOrientation(Qt::Orientation::Horizontal);

        splitLayout->addWidget(sliderSplit);

        spinSplit = new QSpinBox(tabTrain);
        spinSplit->setObjectName("spinSplit");
        spinSplit->setMinimum(50);
        spinSplit->setMaximum(95);
        spinSplit->setValue(80);

        splitLayout->addWidget(spinSplit);


        verticalLayout_3->addLayout(splitLayout);

        trainParamsGrid = new QGridLayout();
        trainParamsGrid->setObjectName("trainParamsGrid");
        label_2 = new QLabel(tabTrain);
        label_2->setObjectName("label_2");

        trainParamsGrid->addWidget(label_2, 0, 0, 1, 1);

        edEpoch = new QSpinBox(tabTrain);
        edEpoch->setObjectName("edEpoch");
        edEpoch->setMinimum(1);
        edEpoch->setMaximum(500);
        edEpoch->setValue(10);

        trainParamsGrid->addWidget(edEpoch, 0, 1, 1, 1);

        label_3 = new QLabel(tabTrain);
        label_3->setObjectName("label_3");

        trainParamsGrid->addWidget(label_3, 0, 2, 1, 1);

        edBatch = new QSpinBox(tabTrain);
        edBatch->setObjectName("edBatch");
        edBatch->setMinimum(1);
        edBatch->setMaximum(128);
        edBatch->setValue(2);

        trainParamsGrid->addWidget(edBatch, 0, 3, 1, 1);

        label_4 = new QLabel(tabTrain);
        label_4->setObjectName("label_4");

        trainParamsGrid->addWidget(label_4, 0, 4, 1, 1);

        edImgsz = new QSpinBox(tabTrain);
        edImgsz->setObjectName("edImgsz");
        edImgsz->setMinimum(320);
        edImgsz->setMaximum(1280);
        edImgsz->setSingleStep(32);
        edImgsz->setValue(640);

        trainParamsGrid->addWidget(edImgsz, 0, 5, 1, 1);

        lblLR = new QLabel(tabTrain);
        lblLR->setObjectName("lblLR");

        trainParamsGrid->addWidget(lblLR, 1, 0, 1, 1);

        edLR = new QDoubleSpinBox(tabTrain);
        edLR->setObjectName("edLR");
        edLR->setDecimals(5);
        edLR->setMinimum(0.000010000000000);
        edLR->setMaximum(0.100000000000000);
        edLR->setSingleStep(0.000100000000000);
        edLR->setValue(0.001000000000000);

        trainParamsGrid->addWidget(edLR, 1, 1, 1, 1);

        lblOptimizer = new QLabel(tabTrain);
        lblOptimizer->setObjectName("lblOptimizer");

        trainParamsGrid->addWidget(lblOptimizer, 1, 2, 1, 1);

        cbOptimizer = new QComboBox(tabTrain);
        cbOptimizer->addItem(QString());
        cbOptimizer->addItem(QString());
        cbOptimizer->addItem(QString());
        cbOptimizer->setObjectName("cbOptimizer");

        trainParamsGrid->addWidget(cbOptimizer, 1, 3, 1, 1);

        lblWD = new QLabel(tabTrain);
        lblWD->setObjectName("lblWD");

        trainParamsGrid->addWidget(lblWD, 1, 4, 1, 1);

        edWD = new QDoubleSpinBox(tabTrain);
        edWD->setObjectName("edWD");
        edWD->setDecimals(5);
        edWD->setMinimum(0.000000000000000);
        edWD->setMaximum(1.000000000000000);
        edWD->setSingleStep(0.000100000000000);
        edWD->setValue(0.000500000000000);

        trainParamsGrid->addWidget(edWD, 1, 5, 1, 1);

        lblDevice = new QLabel(tabTrain);
        lblDevice->setObjectName("lblDevice");

        trainParamsGrid->addWidget(lblDevice, 1, 6, 1, 1);

        cbDevice = new QComboBox(tabTrain);
        cbDevice->addItem(QString());
        cbDevice->addItem(QString());
        cbDevice->setObjectName("cbDevice");

        trainParamsGrid->addWidget(cbDevice, 1, 7, 1, 1);


        verticalLayout_3->addLayout(trainParamsGrid);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        btnStartTrain = new QPushButton(tabTrain);
        btnStartTrain->setObjectName("btnStartTrain");

        horizontalLayout_4->addWidget(btnStartTrain);

        btnStopTrain = new QPushButton(tabTrain);
        btnStopTrain->setObjectName("btnStopTrain");

        horizontalLayout_4->addWidget(btnStopTrain);


        verticalLayout_3->addLayout(horizontalLayout_4);

        label_5 = new QLabel(tabTrain);
        label_5->setObjectName("label_5");

        verticalLayout_3->addWidget(label_5);

        teTrainLog = new QTextEdit(tabTrain);
        teTrainLog->setObjectName("teTrainLog");
        teTrainLog->setReadOnly(true);

        verticalLayout_3->addWidget(teTrainLog);

        gbMetrics = new QGroupBox(tabTrain);
        gbMetrics->setObjectName("gbMetrics");
        metricsGrid = new QGridLayout(gbMetrics);
        metricsGrid->setObjectName("metricsGrid");
        lblMAP = new QLabel(gbMetrics);
        lblMAP->setObjectName("lblMAP");
        lblMAP->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);

        metricsGrid->addWidget(lblMAP, 0, 0, 1, 1);

        lblMAP95 = new QLabel(gbMetrics);
        lblMAP95->setObjectName("lblMAP95");
        lblMAP95->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);

        metricsGrid->addWidget(lblMAP95, 0, 1, 1, 1);

        lblPrecision = new QLabel(gbMetrics);
        lblPrecision->setObjectName("lblPrecision");
        lblPrecision->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);

        metricsGrid->addWidget(lblPrecision, 0, 2, 1, 1);

        lblRecall = new QLabel(gbMetrics);
        lblRecall->setObjectName("lblRecall");
        lblRecall->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);

        metricsGrid->addWidget(lblRecall, 0, 3, 1, 1);

        lblModelPath = new QLabel(gbMetrics);
        lblModelPath->setObjectName("lblModelPath");

        metricsGrid->addWidget(lblModelPath, 1, 0, 1, 1);

        lblTrainTime = new QLabel(gbMetrics);
        lblTrainTime->setObjectName("lblTrainTime");

        metricsGrid->addWidget(lblTrainTime, 1, 1, 1, 1);

        btnSaveTrainResult = new QPushButton(gbMetrics);
        btnSaveTrainResult->setObjectName("btnSaveTrainResult");

        metricsGrid->addWidget(btnSaveTrainResult, 1, 2, 1, 2);


        verticalLayout_3->addWidget(gbMetrics);

        tabWidget->addTab(tabTrain, QString());
        tabQuant = new QWidget();
        tabQuant->setObjectName("tabQuant");
        verticalLayout_4 = new QVBoxLayout(tabQuant);
        verticalLayout_4->setObjectName("verticalLayout_4");
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName("horizontalLayout_5");
        label_6 = new QLabel(tabQuant);
        label_6->setObjectName("label_6");

        horizontalLayout_5->addWidget(label_6);

        edModelPath = new QLineEdit(tabQuant);
        edModelPath->setObjectName("edModelPath");

        horizontalLayout_5->addWidget(edModelPath);

        btnSelectModel = new QPushButton(tabQuant);
        btnSelectModel->setObjectName("btnSelectModel");

        horizontalLayout_5->addWidget(btnSelectModel);


        verticalLayout_4->addLayout(horizontalLayout_5);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName("horizontalLayout_6");
        label_7 = new QLabel(tabQuant);
        label_7->setObjectName("label_7");

        horizontalLayout_6->addWidget(label_7);

        cbQuantType = new QComboBox(tabQuant);
        cbQuantType->addItem(QString());
        cbQuantType->addItem(QString());
        cbQuantType->setObjectName("cbQuantType");

        horizontalLayout_6->addWidget(cbQuantType);

        lblQuantScheme = new QLabel(tabQuant);
        lblQuantScheme->setObjectName("lblQuantScheme");

        horizontalLayout_6->addWidget(lblQuantScheme);

        cbQuantScheme = new QComboBox(tabQuant);
        cbQuantScheme->addItem(QString());
        cbQuantScheme->addItem(QString());
        cbQuantScheme->setObjectName("cbQuantScheme");

        horizontalLayout_6->addWidget(cbQuantScheme);


        verticalLayout_4->addLayout(horizontalLayout_6);

        quantPathwayLayout = new QHBoxLayout();
        quantPathwayLayout->setObjectName("quantPathwayLayout");
        lblQuantPathway = new QLabel(tabQuant);
        lblQuantPathway->setObjectName("lblQuantPathway");

        quantPathwayLayout->addWidget(lblQuantPathway);

        cbQuantPathway = new QComboBox(tabQuant);
        cbQuantPathway->addItem(QString());
        cbQuantPathway->addItem(QString());
        cbQuantPathway->addItem(QString());
        cbQuantPathway->setObjectName("cbQuantPathway");

        quantPathwayLayout->addWidget(cbQuantPathway);


        verticalLayout_4->addLayout(quantPathwayLayout);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName("horizontalLayout_7");
        btnStartQuant = new QPushButton(tabQuant);
        btnStartQuant->setObjectName("btnStartQuant");

        horizontalLayout_7->addWidget(btnStartQuant);

        btnStopQuant = new QPushButton(tabQuant);
        btnStopQuant->setObjectName("btnStopQuant");

        horizontalLayout_7->addWidget(btnStopQuant);


        verticalLayout_4->addLayout(horizontalLayout_7);

        label_8 = new QLabel(tabQuant);
        label_8->setObjectName("label_8");

        verticalLayout_4->addWidget(label_8);

        teQuantLog = new QTextEdit(tabQuant);
        teQuantLog->setObjectName("teQuantLog");
        teQuantLog->setReadOnly(true);

        verticalLayout_4->addWidget(teQuantLog);

        tabWidget->addTab(tabQuant, QString());
        tabInfer = new QWidget();
        tabInfer->setObjectName("tabInfer");
        verticalLayout_5 = new QVBoxLayout(tabInfer);
        verticalLayout_5->setObjectName("verticalLayout_5");
        inferEngineLayout = new QHBoxLayout();
        inferEngineLayout->setObjectName("inferEngineLayout");
        lblInferEngine = new QLabel(tabInfer);
        lblInferEngine->setObjectName("lblInferEngine");

        inferEngineLayout->addWidget(lblInferEngine);

        cbInferEngine = new QComboBox(tabInfer);
        cbInferEngine->addItem(QString());
        cbInferEngine->addItem(QString());
        cbInferEngine->setObjectName("cbInferEngine");

        inferEngineLayout->addWidget(cbInferEngine);


        verticalLayout_5->addLayout(inferEngineLayout);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName("horizontalLayout_8");
        label_9 = new QLabel(tabInfer);
        label_9->setObjectName("label_9");

        horizontalLayout_8->addWidget(label_9);

        edQuantModelPath = new QLineEdit(tabInfer);
        edQuantModelPath->setObjectName("edQuantModelPath");

        horizontalLayout_8->addWidget(edQuantModelPath);

        btnSelectQuantModel = new QPushButton(tabInfer);
        btnSelectQuantModel->setObjectName("btnSelectQuantModel");

        horizontalLayout_8->addWidget(btnSelectQuantModel);


        verticalLayout_5->addLayout(horizontalLayout_8);

        inferParamLayout = new QHBoxLayout();
        inferParamLayout->setObjectName("inferParamLayout");
        lblConf = new QLabel(tabInfer);
        lblConf->setObjectName("lblConf");

        inferParamLayout->addWidget(lblConf);

        edConfThresh = new QDoubleSpinBox(tabInfer);
        edConfThresh->setObjectName("edConfThresh");
        edConfThresh->setMinimum(0.010000000000000);
        edConfThresh->setMaximum(1.000000000000000);
        edConfThresh->setSingleStep(0.050000000000000);
        edConfThresh->setValue(0.150000000000000);

        inferParamLayout->addWidget(edConfThresh);

        lblIoU = new QLabel(tabInfer);
        lblIoU->setObjectName("lblIoU");

        inferParamLayout->addWidget(lblIoU);

        edIoUThresh = new QDoubleSpinBox(tabInfer);
        edIoUThresh->setObjectName("edIoUThresh");
        edIoUThresh->setMinimum(0.100000000000000);
        edIoUThresh->setMaximum(1.000000000000000);
        edIoUThresh->setSingleStep(0.050000000000000);
        edIoUThresh->setValue(0.600000000000000);

        inferParamLayout->addWidget(edIoUThresh);


        verticalLayout_5->addLayout(inferParamLayout);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName("horizontalLayout_9");
        btnOpenInferImage = new QPushButton(tabInfer);
        btnOpenInferImage->setObjectName("btnOpenInferImage");

        horizontalLayout_9->addWidget(btnOpenInferImage);

        btnRunInfer = new QPushButton(tabInfer);
        btnRunInfer->setObjectName("btnRunInfer");

        horizontalLayout_9->addWidget(btnRunInfer);

        btnBatchInfer = new QPushButton(tabInfer);
        btnBatchInfer->setObjectName("btnBatchInfer");

        horizontalLayout_9->addWidget(btnBatchInfer);

        btnSaveInferResult = new QPushButton(tabInfer);
        btnSaveInferResult->setObjectName("btnSaveInferResult");

        horizontalLayout_9->addWidget(btnSaveInferResult);


        verticalLayout_5->addLayout(horizontalLayout_9);

        widgetInfer = new AnnotWidget(tabInfer);
        widgetInfer->setObjectName("widgetInfer");
        widgetInfer->setMinimumSize(QSize(400, 300));

        verticalLayout_5->addWidget(widgetInfer);

        tableInferResults = new QTableWidget(tabInfer);
        if (tableInferResults->columnCount() < 6)
            tableInferResults->setColumnCount(6);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        tableInferResults->setHorizontalHeaderItem(0, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        tableInferResults->setHorizontalHeaderItem(1, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        tableInferResults->setHorizontalHeaderItem(2, __qtablewidgetitem8);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        tableInferResults->setHorizontalHeaderItem(3, __qtablewidgetitem9);
        QTableWidgetItem *__qtablewidgetitem10 = new QTableWidgetItem();
        tableInferResults->setHorizontalHeaderItem(4, __qtablewidgetitem10);
        QTableWidgetItem *__qtablewidgetitem11 = new QTableWidgetItem();
        tableInferResults->setHorizontalHeaderItem(5, __qtablewidgetitem11);
        tableInferResults->setObjectName("tableInferResults");
        tableInferResults->setMaximumSize(QSize(16777215, 130));
        tableInferResults->setColumnCount(6);

        verticalLayout_5->addWidget(tableInferResults);

        tabWidget->addTab(tabInfer, QString());
        tabHistory = new QWidget();
        tabHistory->setObjectName("tabHistory");
        historyLayout = new QVBoxLayout(tabHistory);
        historyLayout->setObjectName("historyLayout");
        historyTabWidget = new QTabWidget(tabHistory);
        historyTabWidget->setObjectName("historyTabWidget");
        historyTrainTab = new QWidget();
        historyTrainTab->setObjectName("historyTrainTab");
        vboxLayout = new QVBoxLayout(historyTrainTab);
        vboxLayout->setObjectName("vboxLayout");
        tableTrainHistory = new QTableWidget(historyTrainTab);
        if (tableTrainHistory->columnCount() < 9)
            tableTrainHistory->setColumnCount(9);
        QTableWidgetItem *__qtablewidgetitem12 = new QTableWidgetItem();
        tableTrainHistory->setHorizontalHeaderItem(0, __qtablewidgetitem12);
        QTableWidgetItem *__qtablewidgetitem13 = new QTableWidgetItem();
        tableTrainHistory->setHorizontalHeaderItem(1, __qtablewidgetitem13);
        QTableWidgetItem *__qtablewidgetitem14 = new QTableWidgetItem();
        tableTrainHistory->setHorizontalHeaderItem(2, __qtablewidgetitem14);
        QTableWidgetItem *__qtablewidgetitem15 = new QTableWidgetItem();
        tableTrainHistory->setHorizontalHeaderItem(3, __qtablewidgetitem15);
        QTableWidgetItem *__qtablewidgetitem16 = new QTableWidgetItem();
        tableTrainHistory->setHorizontalHeaderItem(4, __qtablewidgetitem16);
        QTableWidgetItem *__qtablewidgetitem17 = new QTableWidgetItem();
        tableTrainHistory->setHorizontalHeaderItem(5, __qtablewidgetitem17);
        QTableWidgetItem *__qtablewidgetitem18 = new QTableWidgetItem();
        tableTrainHistory->setHorizontalHeaderItem(6, __qtablewidgetitem18);
        QTableWidgetItem *__qtablewidgetitem19 = new QTableWidgetItem();
        tableTrainHistory->setHorizontalHeaderItem(7, __qtablewidgetitem19);
        QTableWidgetItem *__qtablewidgetitem20 = new QTableWidgetItem();
        tableTrainHistory->setHorizontalHeaderItem(8, __qtablewidgetitem20);
        tableTrainHistory->setObjectName("tableTrainHistory");
        tableTrainHistory->setColumnCount(9);

        vboxLayout->addWidget(tableTrainHistory);

        historyTabWidget->addTab(historyTrainTab, QString());
        historyInferTab = new QWidget();
        historyInferTab->setObjectName("historyInferTab");
        vboxLayout1 = new QVBoxLayout(historyInferTab);
        vboxLayout1->setObjectName("vboxLayout1");
        tableInferHistory = new QTableWidget(historyInferTab);
        if (tableInferHistory->columnCount() < 6)
            tableInferHistory->setColumnCount(6);
        QTableWidgetItem *__qtablewidgetitem21 = new QTableWidgetItem();
        tableInferHistory->setHorizontalHeaderItem(0, __qtablewidgetitem21);
        QTableWidgetItem *__qtablewidgetitem22 = new QTableWidgetItem();
        tableInferHistory->setHorizontalHeaderItem(1, __qtablewidgetitem22);
        QTableWidgetItem *__qtablewidgetitem23 = new QTableWidgetItem();
        tableInferHistory->setHorizontalHeaderItem(2, __qtablewidgetitem23);
        QTableWidgetItem *__qtablewidgetitem24 = new QTableWidgetItem();
        tableInferHistory->setHorizontalHeaderItem(3, __qtablewidgetitem24);
        QTableWidgetItem *__qtablewidgetitem25 = new QTableWidgetItem();
        tableInferHistory->setHorizontalHeaderItem(4, __qtablewidgetitem25);
        QTableWidgetItem *__qtablewidgetitem26 = new QTableWidgetItem();
        tableInferHistory->setHorizontalHeaderItem(5, __qtablewidgetitem26);
        tableInferHistory->setObjectName("tableInferHistory");
        tableInferHistory->setColumnCount(6);

        vboxLayout1->addWidget(tableInferHistory);

        historyTabWidget->addTab(historyInferTab, QString());

        historyLayout->addWidget(historyTabWidget);

        tabWidget->addTab(tabHistory, QString());

        verticalLayout->addWidget(tabWidget);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1100, 20));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName("menuFile");
        menuEdit = new QMenu(menubar);
        menuEdit->setObjectName("menuEdit");
        menuView = new QMenu(menubar);
        menuView->setObjectName("menuView");
        menuHelp = new QMenu(menubar);
        menuHelp->setObjectName("menuHelp");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuEdit->menuAction());
        menubar->addAction(menuView->menuAction());
        menubar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionOpenImage);
        menuFile->addAction(actionSave);
        menuFile->addSeparator();
        menuFile->addAction(actionExit);
        menuEdit->addAction(actionUndo);
        menuEdit->addAction(actionRedo);
        menuView->addAction(actionZoomIn);
        menuView->addAction(actionZoomOut);
        menuView->addAction(actionZoomFit);
        menuView->addSeparator();
        menuView->addAction(actionRotateLeft);
        menuView->addAction(actionRotateRight);
        menuView->addAction(actionResetView);
        menuHelp->addAction(actionAbout);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "TOMATO \342\200\224 \347\225\252\350\214\204\346\243\200\346\265\213\346\240\207\346\263\250\350\256\255\347\273\203\346\216\250\347\220\206\345\271\263\345\217\260", nullptr));
        actionOpenImage->setText(QCoreApplication::translate("MainWindow", "\346\211\223\345\274\200\345\233\276\347\211\207", nullptr));
#if QT_CONFIG(tooltip)
        actionOpenImage->setToolTip(QCoreApplication::translate("MainWindow", "\346\211\223\345\274\200\345\233\276\347\211\207\350\277\233\350\241\214\346\240\207\346\263\250", nullptr));
#endif // QT_CONFIG(tooltip)
        actionSave->setText(QCoreApplication::translate("MainWindow", "\344\277\235\345\255\230\346\240\207\346\263\250", nullptr));
#if QT_CONFIG(tooltip)
        actionSave->setToolTip(QCoreApplication::translate("MainWindow", "\344\277\235\345\255\230YOLO\346\240\274\345\274\217\346\240\207\346\263\250", nullptr));
#endif // QT_CONFIG(tooltip)
        actionUndo->setText(QCoreApplication::translate("MainWindow", "\346\222\244\351\224\200", nullptr));
#if QT_CONFIG(shortcut)
        actionUndo->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+Z", nullptr));
#endif // QT_CONFIG(shortcut)
        actionRedo->setText(QCoreApplication::translate("MainWindow", "\351\207\215\345\201\232", nullptr));
#if QT_CONFIG(shortcut)
        actionRedo->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+Y", nullptr));
#endif // QT_CONFIG(shortcut)
        actionZoomIn->setText(QCoreApplication::translate("MainWindow", "\346\224\276\345\244\247", nullptr));
#if QT_CONFIG(shortcut)
        actionZoomIn->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl++", nullptr));
#endif // QT_CONFIG(shortcut)
        actionZoomOut->setText(QCoreApplication::translate("MainWindow", "\347\274\251\345\260\217", nullptr));
#if QT_CONFIG(shortcut)
        actionZoomOut->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+-", nullptr));
#endif // QT_CONFIG(shortcut)
        actionZoomFit->setText(QCoreApplication::translate("MainWindow", "\351\200\202\345\272\224\347\252\227\345\217\243", nullptr));
        actionExit->setText(QCoreApplication::translate("MainWindow", "\351\200\200\345\207\272", nullptr));
#if QT_CONFIG(shortcut)
        actionExit->setShortcut(QCoreApplication::translate("MainWindow", "Alt+F4", nullptr));
#endif // QT_CONFIG(shortcut)
        actionRotateLeft->setText(QCoreApplication::translate("MainWindow", "\345\267\246\346\227\213 15\302\260", nullptr));
#if QT_CONFIG(tooltip)
        actionRotateLeft->setToolTip(QCoreApplication::translate("MainWindow", "\351\200\206\346\227\266\351\222\210\346\227\213\350\275\25415\345\272\246 (\345\277\253\346\215\267\351\224\256: [)", nullptr));
#endif // QT_CONFIG(tooltip)
        actionRotateRight->setText(QCoreApplication::translate("MainWindow", "\345\217\263\346\227\213 15\302\260", nullptr));
#if QT_CONFIG(tooltip)
        actionRotateRight->setToolTip(QCoreApplication::translate("MainWindow", "\351\241\272\346\227\266\351\222\210\346\227\213\350\275\25415\345\272\246 (\345\277\253\346\215\267\351\224\256: ])", nullptr));
#endif // QT_CONFIG(tooltip)
        actionResetView->setText(QCoreApplication::translate("MainWindow", "\351\207\215\347\275\256\350\247\206\345\233\276", nullptr));
#if QT_CONFIG(tooltip)
        actionResetView->setToolTip(QCoreApplication::translate("MainWindow", "\351\207\215\347\275\256\347\274\251\346\224\276/\346\227\213\350\275\254/\345\271\263\347\247\273 (\345\277\253\346\215\267\351\224\256: 0)", nullptr));
#endif // QT_CONFIG(tooltip)
        actionAbout->setText(QCoreApplication::translate("MainWindow", "\345\205\263\344\272\216", nullptr));
        lblCategory->setText(QCoreApplication::translate("MainWindow", "\345\275\223\345\211\215\347\261\273\345\210\253\357\274\232", nullptr));
        btnAddCategory->setText(QCoreApplication::translate("MainWindow", "\346\226\260\345\273\272\347\261\273\345\210\253", nullptr));
        btnDelCategory->setText(QCoreApplication::translate("MainWindow", "\345\210\240\351\231\244\347\261\273\345\210\253", nullptr));
        btnOpenImage->setText(QCoreApplication::translate("MainWindow", "\346\211\223\345\274\200\345\233\276\347\211\207", nullptr));
        btnSaveYolo->setText(QCoreApplication::translate("MainWindow", "\344\277\235\345\255\230YOLO\346\240\207\346\263\250", nullptr));
        btnClearAnnot->setText(QCoreApplication::translate("MainWindow", "\346\270\205\351\231\244\346\240\207\346\263\250", nullptr));
        btnDeleteBox->setText(QCoreApplication::translate("MainWindow", "\345\210\240\351\231\244\351\200\211\344\270\255\346\241\206", nullptr));
        btnPanLeft->setText(QCoreApplication::translate("MainWindow", "\342\227\200", nullptr));
#if QT_CONFIG(tooltip)
        btnPanLeft->setToolTip(QCoreApplication::translate("MainWindow", "\345\267\246\347\247\273 (\346\226\271\345\220\221\351\224\256 \342\206\220)", nullptr));
#endif // QT_CONFIG(tooltip)
        btnPanUp->setText(QCoreApplication::translate("MainWindow", "\342\226\262", nullptr));
#if QT_CONFIG(tooltip)
        btnPanUp->setToolTip(QCoreApplication::translate("MainWindow", "\344\270\212\347\247\273 (\346\226\271\345\220\221\351\224\256 \342\206\221)", nullptr));
#endif // QT_CONFIG(tooltip)
        btnPanDown->setText(QCoreApplication::translate("MainWindow", "\342\226\274", nullptr));
#if QT_CONFIG(tooltip)
        btnPanDown->setToolTip(QCoreApplication::translate("MainWindow", "\344\270\213\347\247\273 (\346\226\271\345\220\221\351\224\256 \342\206\223)", nullptr));
#endif // QT_CONFIG(tooltip)
        btnPanRight->setText(QCoreApplication::translate("MainWindow", "\342\226\266", nullptr));
#if QT_CONFIG(tooltip)
        btnPanRight->setToolTip(QCoreApplication::translate("MainWindow", "\345\217\263\347\247\273 (\346\226\271\345\220\221\351\224\256 \342\206\222)", nullptr));
#endif // QT_CONFIG(tooltip)
        btnZoomOut->setText(QCoreApplication::translate("MainWindow", "\342\210\222", nullptr));
#if QT_CONFIG(tooltip)
        btnZoomOut->setToolTip(QCoreApplication::translate("MainWindow", "\347\274\251\345\260\217 (\345\277\253\346\215\267\351\224\256: -)", nullptr));
#endif // QT_CONFIG(tooltip)
        btnZoomIn->setText(QCoreApplication::translate("MainWindow", "+", nullptr));
#if QT_CONFIG(tooltip)
        btnZoomIn->setToolTip(QCoreApplication::translate("MainWindow", "\346\224\276\345\244\247 (\345\277\253\346\215\267\351\224\256: +)", nullptr));
#endif // QT_CONFIG(tooltip)
        lblRotation->setText(QCoreApplication::translate("MainWindow", "\346\227\213\350\275\254:", nullptr));
        edRotation->setSuffix(QCoreApplication::translate("MainWindow", "\302\260", nullptr));
#if QT_CONFIG(tooltip)
        edRotation->setToolTip(QCoreApplication::translate("MainWindow", "\344\273\273\346\204\217\346\227\213\350\275\254\350\247\222\345\272\246 (0-360\302\260)", nullptr));
#endif // QT_CONFIG(tooltip)
        btnRotateLeft->setText(QCoreApplication::translate("MainWindow", "\342\206\272", nullptr));
#if QT_CONFIG(tooltip)
        btnRotateLeft->setToolTip(QCoreApplication::translate("MainWindow", "\345\267\246\346\227\213 15\302\260 (\345\277\253\346\215\267\351\224\256: [)", nullptr));
#endif // QT_CONFIG(tooltip)
        btnRotateRight->setText(QCoreApplication::translate("MainWindow", "\342\206\273", nullptr));
#if QT_CONFIG(tooltip)
        btnRotateRight->setToolTip(QCoreApplication::translate("MainWindow", "\345\217\263\346\227\213 15\302\260 (\345\277\253\346\215\267\351\224\256: ])", nullptr));
#endif // QT_CONFIG(tooltip)
        btnResetView->setText(QCoreApplication::translate("MainWindow", "\351\207\215\347\275\256", nullptr));
#if QT_CONFIG(tooltip)
        btnResetView->setToolTip(QCoreApplication::translate("MainWindow", "\351\207\215\347\275\256\347\274\251\346\224\276/\346\227\213\350\275\254/\345\271\263\347\247\273 (\345\277\253\346\215\267\351\224\256: 0)", nullptr));
#endif // QT_CONFIG(tooltip)
        btnLoadDataset->setText(QCoreApplication::translate("MainWindow", "\345\212\240\350\275\275\346\225\260\346\215\256\351\233\206", nullptr));
        btnPrevImage->setText(QCoreApplication::translate("MainWindow", "\342\227\200 \344\270\212\344\270\200\345\274\240", nullptr));
        lblDatasetNav->setText(QCoreApplication::translate("MainWindow", "\346\234\252\345\212\240\350\275\275\346\225\260\346\215\256\351\233\206", nullptr));
        btnNextImage->setText(QCoreApplication::translate("MainWindow", "\344\270\213\344\270\200\345\274\240 \342\226\266", nullptr));
#if QT_CONFIG(tooltip)
        btnSaveAllLabels->setToolTip(QCoreApplication::translate("MainWindow", "\346\211\271\351\207\217\344\277\235\345\255\230\346\211\200\346\234\211\345\267\262\346\240\207\346\263\250\345\233\276\347\211\207\347\232\204YOLO\346\240\207\347\255\276", nullptr));
#endif // QT_CONFIG(tooltip)
        btnSaveAllLabels->setText(QCoreApplication::translate("MainWindow", "\344\277\235\345\255\230\345\205\250\351\203\250\346\240\207\346\263\250", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableAnnotations->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("MainWindow", "\345\272\217\345\217\267", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableAnnotations->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("MainWindow", "\347\261\273\345\210\253", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableAnnotations->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("MainWindow", "X", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableAnnotations->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("MainWindow", "Y", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tableAnnotations->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("MainWindow", "\345\256\275\345\272\246", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = tableAnnotations->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("MainWindow", "\351\253\230\345\272\246", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabAnnot), QCoreApplication::translate("MainWindow", "\346\240\207\346\263\250\345\267\245\345\205\267", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "\346\225\260\346\215\256\351\233\206\350\267\257\345\276\204\357\274\232", nullptr));
        btnSelectDataset->setText(QCoreApplication::translate("MainWindow", "\351\200\211\346\213\251\346\226\207\344\273\266\345\244\271", nullptr));
        lblSplit->setText(QCoreApplication::translate("MainWindow", "\350\256\255\347\273\203/\351\252\214\350\257\201\351\233\206\346\257\224\344\276\213\357\274\232", nullptr));
        spinSplit->setSuffix(QCoreApplication::translate("MainWindow", "% \350\256\255\347\273\203\351\233\206", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "\350\256\255\347\273\203\350\275\256\346\254\241:", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "\346\211\271\346\254\241\345\244\247\345\260\217:", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "\345\233\276\345\203\217\345\244\247\345\260\217:", nullptr));
        lblLR->setText(QCoreApplication::translate("MainWindow", "\345\255\246\344\271\240\347\216\207:", nullptr));
        lblOptimizer->setText(QCoreApplication::translate("MainWindow", "\344\274\230\345\214\226\345\231\250:", nullptr));
        cbOptimizer->setItemText(0, QCoreApplication::translate("MainWindow", "AdamW", nullptr));
        cbOptimizer->setItemText(1, QCoreApplication::translate("MainWindow", "SGD", nullptr));
        cbOptimizer->setItemText(2, QCoreApplication::translate("MainWindow", "Adam", nullptr));

        lblWD->setText(QCoreApplication::translate("MainWindow", "\346\235\203\351\207\215\350\241\260\345\207\217:", nullptr));
        lblDevice->setText(QCoreApplication::translate("MainWindow", "\350\256\276\345\244\207:", nullptr));
        cbDevice->setItemText(0, QCoreApplication::translate("MainWindow", "cpu", nullptr));
        cbDevice->setItemText(1, QCoreApplication::translate("MainWindow", "gpu", nullptr));

        btnStartTrain->setText(QCoreApplication::translate("MainWindow", "\345\274\200\345\247\213\350\256\255\347\273\203", nullptr));
        btnStopTrain->setText(QCoreApplication::translate("MainWindow", "\345\201\234\346\255\242\350\256\255\347\273\203", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "\350\256\255\347\273\203\345\256\236\346\227\266\346\227\245\345\277\227\357\274\232", nullptr));
        gbMetrics->setTitle(QCoreApplication::translate("MainWindow", "\350\256\255\347\273\203\347\273\223\346\236\234\346\214\207\346\240\207", nullptr));
        lblMAP->setText(QCoreApplication::translate("MainWindow", "mAP50: --", nullptr));
        lblMAP95->setText(QCoreApplication::translate("MainWindow", "mAP50-95: --", nullptr));
        lblPrecision->setText(QCoreApplication::translate("MainWindow", "Precision: --", nullptr));
        lblRecall->setText(QCoreApplication::translate("MainWindow", "Recall: --", nullptr));
        lblModelPath->setText(QCoreApplication::translate("MainWindow", "\346\250\241\345\236\213\350\267\257\345\276\204: --", nullptr));
        lblTrainTime->setText(QCoreApplication::translate("MainWindow", "\350\256\255\347\273\203\350\200\227\346\227\266: --", nullptr));
        btnSaveTrainResult->setText(QCoreApplication::translate("MainWindow", "\344\277\235\345\255\230\350\256\255\347\273\203\347\273\223\346\236\234\345\210\260\346\225\260\346\215\256\345\272\223", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabTrain), QCoreApplication::translate("MainWindow", "\346\250\241\345\236\213\350\256\255\347\273\203", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "\346\250\241\345\236\213\350\267\257\345\276\204\357\274\232", nullptr));
        btnSelectModel->setText(QCoreApplication::translate("MainWindow", "\351\200\211\346\213\251\346\250\241\345\236\213", nullptr));
        label_7->setText(QCoreApplication::translate("MainWindow", "\351\207\217\345\214\226\347\261\273\345\236\213\357\274\232", nullptr));
        cbQuantType->setItemText(0, QCoreApplication::translate("MainWindow", "fp16", nullptr));
        cbQuantType->setItemText(1, QCoreApplication::translate("MainWindow", "int8", nullptr));

        lblQuantScheme->setText(QCoreApplication::translate("MainWindow", "\351\207\217\345\214\226\346\226\271\346\241\210\357\274\232", nullptr));
        cbQuantScheme->setItemText(0, QCoreApplication::translate("MainWindow", "PTQ\357\274\210\350\256\255\347\273\203\345\220\216\351\207\217\345\214\226\357\274\211", nullptr));
        cbQuantScheme->setItemText(1, QCoreApplication::translate("MainWindow", "QAT\357\274\210\346\204\237\347\237\245\351\207\217\345\214\226\357\274\211", nullptr));

        lblQuantPathway->setText(QCoreApplication::translate("MainWindow", "\351\207\217\345\214\226\351\200\224\345\276\204\357\274\232", nullptr));
        cbQuantPathway->setItemText(0, QCoreApplication::translate("MainWindow", "ONNX Runtime \351\207\217\345\214\226", nullptr));
        cbQuantPathway->setItemText(1, QCoreApplication::translate("MainWindow", "TensorRT \351\207\217\345\214\226", nullptr));
        cbQuantPathway->setItemText(2, QCoreApplication::translate("MainWindow", "PyTorch \350\207\252\345\270\246\351\207\217\345\214\226", nullptr));

        btnStartQuant->setText(QCoreApplication::translate("MainWindow", "\345\274\200\345\247\213\351\207\217\345\214\226", nullptr));
        btnStopQuant->setText(QCoreApplication::translate("MainWindow", "\345\201\234\346\255\242\351\207\217\345\214\226", nullptr));
        label_8->setText(QCoreApplication::translate("MainWindow", "\351\207\217\345\214\226\345\256\236\346\227\266\346\227\245\345\277\227\357\274\232", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabQuant), QCoreApplication::translate("MainWindow", "\346\250\241\345\236\213\351\207\217\345\214\226", nullptr));
        lblInferEngine->setText(QCoreApplication::translate("MainWindow", "\346\216\250\347\220\206\345\274\225\346\223\216\357\274\232", nullptr));
        cbInferEngine->setItemText(0, QCoreApplication::translate("MainWindow", "\360\237\220\215 Python \345\212\240\350\275\275\350\256\255\347\273\203\345\216\237\345\247\213\346\250\241\345\236\213 (.pt)", nullptr));
        cbInferEngine->setItemText(1, QCoreApplication::translate("MainWindow", "\342\232\241 C++ \345\212\240\350\275\275\351\207\217\345\214\226\346\250\241\345\236\213 (.onnx)", nullptr));

        label_9->setText(QCoreApplication::translate("MainWindow", "\346\250\241\345\236\213\350\267\257\345\276\204\357\274\232", nullptr));
        btnSelectQuantModel->setText(QCoreApplication::translate("MainWindow", "\351\200\211\346\213\251\346\250\241\345\236\213", nullptr));
        lblConf->setText(QCoreApplication::translate("MainWindow", "\347\275\256\344\277\241\345\272\246\351\230\210\345\200\274:", nullptr));
#if QT_CONFIG(tooltip)
        edConfThresh->setToolTip(QCoreApplication::translate("MainWindow", "\350\260\203\344\275\216 = \346\243\200\345\207\272\346\233\264\345\244\232\347\233\256\346\240\207 + \346\233\264\345\244\232\350\257\257\346\243\200\357\274\233\350\260\203\351\253\230 = \345\217\252\344\277\235\347\225\231\351\253\230\347\275\256\344\277\241\345\272\246\346\243\200\346\265\213", nullptr));
#endif // QT_CONFIG(tooltip)
        lblIoU->setText(QCoreApplication::translate("MainWindow", "NMS\345\216\273\351\207\215\351\230\210\345\200\274 (\350\266\212\351\253\230\345\216\273\351\207\215\350\266\212\345\260\221):", nullptr));
#if QT_CONFIG(tooltip)
        edIoUThresh->setToolTip(QCoreApplication::translate("MainWindow", "\350\260\203\351\253\230 = \345\205\201\350\256\270\346\243\200\346\265\213\346\241\206\346\233\264\345\244\232\351\207\215\345\217\240\357\274\214\351\201\277\345\205\215\345\244\232\347\233\256\346\240\207\346\274\217\346\243\200", nullptr));
#endif // QT_CONFIG(tooltip)
        btnOpenInferImage->setText(QCoreApplication::translate("MainWindow", "\346\211\223\345\274\200\346\216\250\347\220\206\345\233\276\347\211\207", nullptr));
        btnRunInfer->setText(QCoreApplication::translate("MainWindow", "\345\274\200\345\247\213\346\216\250\347\220\206", nullptr));
        btnBatchInfer->setText(QCoreApplication::translate("MainWindow", "\346\211\271\351\207\217\346\216\250\347\220\206", nullptr));
        btnSaveInferResult->setText(QCoreApplication::translate("MainWindow", "\344\277\235\345\255\230\347\273\223\346\236\234\345\205\245\345\272\223", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = tableInferResults->horizontalHeaderItem(0);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("MainWindow", "\345\272\217\345\217\267", nullptr));
        QTableWidgetItem *___qtablewidgetitem7 = tableInferResults->horizontalHeaderItem(1);
        ___qtablewidgetitem7->setText(QCoreApplication::translate("MainWindow", "\347\261\273\345\210\253", nullptr));
        QTableWidgetItem *___qtablewidgetitem8 = tableInferResults->horizontalHeaderItem(2);
        ___qtablewidgetitem8->setText(QCoreApplication::translate("MainWindow", "X", nullptr));
        QTableWidgetItem *___qtablewidgetitem9 = tableInferResults->horizontalHeaderItem(3);
        ___qtablewidgetitem9->setText(QCoreApplication::translate("MainWindow", "Y", nullptr));
        QTableWidgetItem *___qtablewidgetitem10 = tableInferResults->horizontalHeaderItem(4);
        ___qtablewidgetitem10->setText(QCoreApplication::translate("MainWindow", "\345\256\275\345\272\246", nullptr));
        QTableWidgetItem *___qtablewidgetitem11 = tableInferResults->horizontalHeaderItem(5);
        ___qtablewidgetitem11->setText(QCoreApplication::translate("MainWindow", "\351\253\230\345\272\246", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabInfer), QCoreApplication::translate("MainWindow", "\346\250\241\345\236\213\346\216\250\347\220\206", nullptr));
        QTableWidgetItem *___qtablewidgetitem12 = tableTrainHistory->horizontalHeaderItem(0);
        ___qtablewidgetitem12->setText(QCoreApplication::translate("MainWindow", "ID", nullptr));
        QTableWidgetItem *___qtablewidgetitem13 = tableTrainHistory->horizontalHeaderItem(1);
        ___qtablewidgetitem13->setText(QCoreApplication::translate("MainWindow", "\346\227\266\351\227\264", nullptr));
        QTableWidgetItem *___qtablewidgetitem14 = tableTrainHistory->horizontalHeaderItem(2);
        ___qtablewidgetitem14->setText(QCoreApplication::translate("MainWindow", "\346\225\260\346\215\256\351\233\206", nullptr));
        QTableWidgetItem *___qtablewidgetitem15 = tableTrainHistory->horizontalHeaderItem(3);
        ___qtablewidgetitem15->setText(QCoreApplication::translate("MainWindow", "mAP50", nullptr));
        QTableWidgetItem *___qtablewidgetitem16 = tableTrainHistory->horizontalHeaderItem(4);
        ___qtablewidgetitem16->setText(QCoreApplication::translate("MainWindow", "mAP50-95", nullptr));
        QTableWidgetItem *___qtablewidgetitem17 = tableTrainHistory->horizontalHeaderItem(5);
        ___qtablewidgetitem17->setText(QCoreApplication::translate("MainWindow", "Precision", nullptr));
        QTableWidgetItem *___qtablewidgetitem18 = tableTrainHistory->horizontalHeaderItem(6);
        ___qtablewidgetitem18->setText(QCoreApplication::translate("MainWindow", "Recall", nullptr));
        QTableWidgetItem *___qtablewidgetitem19 = tableTrainHistory->horizontalHeaderItem(7);
        ___qtablewidgetitem19->setText(QCoreApplication::translate("MainWindow", "\346\250\241\345\236\213\350\267\257\345\276\204", nullptr));
        QTableWidgetItem *___qtablewidgetitem20 = tableTrainHistory->horizontalHeaderItem(8);
        ___qtablewidgetitem20->setText(QCoreApplication::translate("MainWindow", "\350\200\227\346\227\266", nullptr));
        historyTabWidget->setTabText(historyTabWidget->indexOf(historyTrainTab), QCoreApplication::translate("MainWindow", "\350\256\255\347\273\203\345\216\206\345\217\262", nullptr));
        QTableWidgetItem *___qtablewidgetitem21 = tableInferHistory->horizontalHeaderItem(0);
        ___qtablewidgetitem21->setText(QCoreApplication::translate("MainWindow", "ID", nullptr));
        QTableWidgetItem *___qtablewidgetitem22 = tableInferHistory->horizontalHeaderItem(1);
        ___qtablewidgetitem22->setText(QCoreApplication::translate("MainWindow", "\346\227\266\351\227\264", nullptr));
        QTableWidgetItem *___qtablewidgetitem23 = tableInferHistory->horizontalHeaderItem(2);
        ___qtablewidgetitem23->setText(QCoreApplication::translate("MainWindow", "\346\250\241\345\236\213", nullptr));
        QTableWidgetItem *___qtablewidgetitem24 = tableInferHistory->horizontalHeaderItem(3);
        ___qtablewidgetitem24->setText(QCoreApplication::translate("MainWindow", "\345\233\276\347\211\207", nullptr));
        QTableWidgetItem *___qtablewidgetitem25 = tableInferHistory->horizontalHeaderItem(4);
        ___qtablewidgetitem25->setText(QCoreApplication::translate("MainWindow", "\346\243\200\346\265\213\346\225\260", nullptr));
        QTableWidgetItem *___qtablewidgetitem26 = tableInferHistory->horizontalHeaderItem(5);
        ___qtablewidgetitem26->setText(QCoreApplication::translate("MainWindow", "\347\275\256\344\277\241\345\272\246\345\235\207\345\200\274", nullptr));
        historyTabWidget->setTabText(historyTabWidget->indexOf(historyInferTab), QCoreApplication::translate("MainWindow", "\346\216\250\347\220\206\345\216\206\345\217\262", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabHistory), QCoreApplication::translate("MainWindow", "\345\216\206\345\217\262\350\256\260\345\275\225", nullptr));
        menuFile->setTitle(QCoreApplication::translate("MainWindow", "\346\226\207\344\273\266(&F)", nullptr));
        menuEdit->setTitle(QCoreApplication::translate("MainWindow", "\347\274\226\350\276\221(&E)", nullptr));
        menuView->setTitle(QCoreApplication::translate("MainWindow", "\350\247\206\345\233\276(&V)", nullptr));
        menuHelp->setTitle(QCoreApplication::translate("MainWindow", "\345\270\256\345\212\251(&H)", nullptr));
#if QT_CONFIG(tooltip)
        statusbar->setToolTip(QString());
#endif // QT_CONFIG(tooltip)
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
