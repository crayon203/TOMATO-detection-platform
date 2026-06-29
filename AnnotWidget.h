#ifndef ANNOTWIDGET_H
#define ANNOTWIDGET_H

#include <QWidget>
#include <QPixmap>
#include <QVector>
#include <QRect>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QMenu>
#include <QStringList>
#include <QStack>
#include <QVariantMap>

class AnnotWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AnnotWidget(QWidget *parent = nullptr);

    void setImage(const QPixmap &pix, const QString &path = "");
    QPixmap getImage() const;
    QString getImagePath() const;

    // 健壮的图片加载：QImage → QPixmap → Python PIL 回退
    static QPixmap loadImageRobust(const QString& filePath);

    QVector<QRect>& getBoxes();
    void clearBoxes();

    QVector<float>& getConfidences();
    void setConfidences(const QVector<float>& confs);

    // ========== 类别管理 ==========
    QStringList& getCategories();
    void setCategories(const QStringList& cats);
    void addCategory(const QString& name);
    void removeCategory(int index);
    int getCurrentCategory() const;
    void setCurrentCategory(int index);
    QVector<int>& getBoxCategories();
    void setBoxCategory(int boxIndex, int categoryIndex);

    // ========== 框操作 ==========
    void deleteSelectedBox();
    int selectedBoxIndex() const;
    void selectBox(int index);

    // ========== 撤销/重做 ==========
    void undo();
    void redo();
    bool canUndo() const;
    bool canRedo() const;

    // ========== 图像视图 ==========
    void zoomIn();
    void zoomOut();
    void zoomFit();
    void rotateImage(qreal angle);
    void setRotationAngle(qreal degrees);
    void panBy(qreal dx, qreal dy);
    void resetView();
    qreal zoomFactor() const;
    qreal rotationAngle() const;

    // ========== 数据集加载与导航 ==========
    void loadYoloDataset(const QString& dirPath);
    QStringList getDatasetImages() const;
    int currentDatasetIndex() const;
    int datasetImageCount() const;
    bool hasPrevImage() const;
    bool hasNextImage() const;
    void navigateToImage(int index);

    // 标注状态管理
    int labeledImageCount() const;
    bool currentImageHasLabel() const;

    // 批量获取 label 路径
    bool hasLabelFile(const QString& imgPath) const;
    QString findLabelPath(const QString& imgPath) const;

signals:
    void boxSelected(int index);
    void boxDeselected();
    void boxesChanged();
    void categoryChanged(int index);
    void statusMessage(const QString& msg);
    void viewChanged(qreal rotation, qreal zoom, QPointF panOffset);

protected:
    void paintEvent(QPaintEvent *) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *event) override;

private:
    enum InteractionMode { Idle, Drawing, Moving, Resizing, Panning };
    enum ResizeHandle { None = -1, TopLeft, Top, TopRight, Left, Right, BottomLeft, Bottom, BottomRight };

    QPixmap m_pix;
    QString m_imagePath;

    QVector<QRect> m_boxes;
    QVector<float> m_confidences;

    // 类别
    QStringList m_categories;
    QVector<int> m_boxCategories;
    int m_currentCategory = 0;

    // 选中
    int m_selectedBox = -1;

    // 交互状态机
    InteractionMode m_mode = Idle;
    ResizeHandle m_activeHandle = None;

    QPoint m_startPoint;
    QPoint m_endPoint;
    QRect m_originalBox;
    int m_interactingIndex = -1;

    // 视图变换
    qreal m_zoomFactor = 1.0;
    qreal m_rotationAngle = 0.0;
    QPointF m_panOffset;

    // 撤销/重做栈
    struct UndoAction {
        QString description;
        QVector<QRect> boxes;
        QVector<int> categories;
        QVector<float> confidences;
    };
    QStack<UndoAction> m_undoStack;
    QStack<UndoAction> m_redoStack;
    void pushUndo(const QString& desc);
    int m_undoLimit = 50;

    // 数据集
    QStringList m_datasetImages;
    QVector<bool> m_datasetLabeled;  // 每张图片是否有标注
    int m_datasetIndex = -1;

    // 辅助
    QRect imageToWidget(const QRect& r) const;
    QRect widgetToImage(const QRect& r) const;
    QRect getImageFitRect() const;
    QRect getScaledBoxRect(const QRect& origBox) const;
    ResizeHandle hitTest(const QPoint& pos) const;
    int boxAtPoint(const QPoint& pos) const;
    QRect getHandleRect(const QRect& boxRect, ResizeHandle handle) const;
};

#endif
