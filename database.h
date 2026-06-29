#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariantMap>
#include <QVector>
#include <QDateTime>

// ============================================================
// SQLite 数据库管理 — 持久化训练/推理/标注记录
// ============================================================

class DatabaseManager : public QObject
{
    Q_OBJECT
public:
    static DatabaseManager& instance();

    bool initialize(const QString& dbPath = "tomato.db");
    void close();

    // ---- 训练记录 ----
    int saveTrainingRecord(const QVariantMap& record);
    QVector<QVariantMap> getAllTrainingRecords();
    QVariantMap getTrainingRecord(int id);
    bool deleteTrainingRecord(int id);

    // ---- 推理记录 ----
    int saveInferenceRecord(const QVariantMap& record);
    QVector<QVariantMap> getAllInferenceRecords();
    QVariantMap getInferenceRecord(int id);
    bool saveDetectionDetail(int inferenceId, const QVariantMap& detection);
    QVector<QVariantMap> getDetectionDetails(int inferenceId);
    bool deleteInferenceRecord(int id);

    // ---- 类别记录 ----
    bool saveCategories(const QStringList& categories);
    QStringList loadCategories();

    // ---- 标注项目记录 ----
    int saveAnnotationProject(const QVariantMap& project);
    QVector<QVariantMap> getAllAnnotationProjects();
    bool deleteAnnotationProject(int id);

    // ---- 统计 ----
    int getTrainingCount();
    int getInferenceCount();

private:
    DatabaseManager(QObject* parent = nullptr);
    ~DatabaseManager();
    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;

    void createTables();
    bool executeSQL(const QString& sql);

    QSqlDatabase m_db;
    bool m_initialized = false;
};

#endif
