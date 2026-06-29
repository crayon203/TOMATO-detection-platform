#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <QLabel>
#include <QSettings>
#include <QVector>
#include <QObject>
#include <QImage>
#include <QRect>
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

// ============================================================
// 推理结果结构体
// ============================================================
struct DetectionResult {
    QRect box;
    float confidence;
    int classId;
    QString className;
};

struct InferDetection { int x, y, w, h; float conf; QString className; };

// ============================================================
// ONNX Runtime 推理引擎（运行时动态加载 onnxruntime.dll）
// ============================================================
class OnnxInference : public QObject
{
    Q_OBJECT
public:
    explicit OnnxInference(QObject *parent = nullptr);
    ~OnnxInference();

    bool loadModel(const QString& modelPath);
    bool isLoaded() const;
    QVector<DetectionResult> detect(const QImage& image,
                                     float confThreshold = 0.25f,
                                     float iouThreshold = 0.45f);
    int inputWidth()  const;
    int inputHeight() const;
    QStringList classNames() const;
    static bool isOnnxRuntimeAvailable();

signals:
    void logMessage(const QString& msg);

private:
    QImage preprocess(const QImage& image);
    QVector<DetectionResult> postprocess(const float* output, int numAnchors,
                                          int numClasses, int imgW, int imgH,
                                          float confThresh, float iouThresh);
    static QVector<int> nms(const QVector<DetectionResult>& dets, float iouThresh);
    static float computeIoU(const QRect& a, const QRect& b);

    static bool initOrtLibrary();
    static QString findOrtDll();

    void* m_session = nullptr;
    void* m_env = nullptr;
    void* m_memoryInfo = nullptr;
    void* m_allocator = nullptr;
    int m_inputW = 640;
    int m_inputH = 640;
    QStringList m_classNames;
    bool m_loaded = false;
    static bool s_ortAvailable;
    static bool s_ortChecked;
};

// ============================================================
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // ---- 标注工具 ----
    void onOpenImage();
    void onSaveYolo();
    void onClear();
    void onAddCategory();
    void onDelCategory();
    void onCategoryChanged(int index);
    void onDeleteSelectedBox();
    void onUndo();
    void onRedo();
    void onZoomIn();
    void onZoomOut();
    void onZoomFit();
    void onRotateLeft();
    void onRotateRight();
    void onRotationAngleChanged(double degrees);
    void onResetView();
    void onPanLeft();
    void onPanRight();
    void onPanUp();
    void onPanDown();
    void onLoadDataset();
    void onPrevImage();
    void onNextImage();
    void onSaveAllLabels();
    void updateDatasetNavState();
    void autoSaveCurrentLabels(class AnnotWidget *w);
    void syncAnnotationTable();
    void onAnnotationTableCellChanged(int row, int col);

    // ---- 模型训练 ----
    void onSelectDataset();
    void onStartTrain();
    void onStopTrain();
    void onSplitSliderChanged(int value);
    void onSaveTrainResult();
    void loadTrainingHistory();

    // ---- 模型量化 ----
    void onSelectModel();
    void onStartQuant();
    void onStopQuant();
    void onQuantOutput();
    void onQuantError();
    void onQuantFinished();

    // ---- 模型推理 ----
    void onSelectQuantModel();
    void onOpenInferImage();
    void onRunInfer();
    void onInferEngineChanged(int index);
    void onBatchInfer();
    void onSaveInferResult();
    void loadInferenceHistory();
    void syncInferTable();
    void runCppInference();
    void onCppInferLog(const QString& msg);
    void runPythonInference(const QString& model, class AnnotWidget* w);
    void finishInference(QVector<InferDetection>& detections,
                          const QString& modelPath, const QString& imagePath);

    // ---- 通用 ----
    void onAbout();
    void updateStatusBar(const QString& msg);
    void updateAnnotationStatus();

private:
    Ui::MainWindow *ui;
    QProcess *process;
    QProcess *quantProcess;
    OnnxInference *onnxEngine;

    bool m_dbReady = false;
    QVariantMap m_lastTrainMetrics;
    int m_currentInferId = -1;

    void setupStatusBar();
    void setupConnections();
    void initDatabase();
    void initCategories();
    void applySettings();
    void saveSettings();
    class AnnotWidget* currentAnnotWidget();
};

#endif
