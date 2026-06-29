#include "database.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QDebug>
#include <QDir>
#include <QStandardPaths>

DatabaseManager& DatabaseManager::instance()
{
    static DatabaseManager inst;
    return inst;
}

DatabaseManager::DatabaseManager(QObject* parent) : QObject(parent)
{
}

DatabaseManager::~DatabaseManager()
{
    close();
}

bool DatabaseManager::initialize(const QString& dbPath)
{
    if (m_initialized) return true;

    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(dbPath);

    if (!m_db.open()) {
        qWarning() << "数据库打开失败:" << m_db.lastError().text();
        return false;
    }

    createTables();
    m_initialized = true;
    qDebug() << "数据库初始化完成:" << dbPath;
    return true;
}

void DatabaseManager::close()
{
    if (m_db.isOpen()) {
        m_db.close();
    }
    m_initialized = false;
}

void DatabaseManager::createTables()
{
    // 训练记录表
    executeSQL(R"(
        CREATE TABLE IF NOT EXISTS training_records (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            timestamp TEXT NOT NULL,
            dataset_path TEXT,
            epochs INTEGER,
            batch_size INTEGER,
            image_size INTEGER,
            learning_rate REAL,
            optimizer TEXT,
            device TEXT,
            val_split REAL,
            map50 REAL,
            map50_95 REAL,
            precision REAL,
            recall REAL,
            model_path TEXT,
            train_time TEXT,
            extra_info TEXT
        )
    )");

    // 推理记录表
    executeSQL(R"(
        CREATE TABLE IF NOT EXISTS inference_records (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            timestamp TEXT NOT NULL,
            model_path TEXT,
            image_path TEXT,
            image_name TEXT,
            conf_threshold REAL,
            iou_threshold REAL,
            detection_count INTEGER,
            avg_confidence REAL,
            result_image_path TEXT
        )
    )");

    // 检测详情表
    executeSQL(R"(
        CREATE TABLE IF NOT EXISTS detection_details (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            inference_id INTEGER NOT NULL,
            class_name TEXT,
            x INTEGER, y INTEGER, width INTEGER, height INTEGER,
            confidence REAL,
            FOREIGN KEY (inference_id) REFERENCES inference_records(id) ON DELETE CASCADE
        )
    )");

    // 类别表
    executeSQL(R"(
        CREATE TABLE IF NOT EXISTS categories (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT UNIQUE NOT NULL
        )
    )");

    // 标注项目表
    executeSQL(R"(
        CREATE TABLE IF NOT EXISTS annotation_projects (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            timestamp TEXT NOT NULL,
            project_name TEXT,
            image_count INTEGER,
            annotation_count INTEGER,
            categories TEXT,
            image_dir TEXT
        )
    )");
}

bool DatabaseManager::executeSQL(const QString& sql)
{
    QSqlQuery query(m_db);
    if (!query.exec(sql)) {
        qWarning() << "SQL执行失败:" << query.lastError().text() << "\nSQL:" << sql;
        return false;
    }
    return true;
}

// ============================================================
//  训练记录
// ============================================================
int DatabaseManager::saveTrainingRecord(const QVariantMap& record)
{
    QSqlQuery query(m_db);
    query.prepare(R"(
        INSERT INTO training_records
        (timestamp, dataset_path, epochs, batch_size, image_size,
         learning_rate, optimizer, device, val_split,
         map50, map50_95, precision, recall, model_path, train_time)
        VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
    )");

    query.addBindValue(record.value("timestamp", QDateTime::currentDateTime().toString(Qt::ISODate)));
    query.addBindValue(record.value("dataset_path"));
    query.addBindValue(record.value("epochs"));
    query.addBindValue(record.value("batch_size"));
    query.addBindValue(record.value("image_size"));
    query.addBindValue(record.value("learning_rate"));
    query.addBindValue(record.value("optimizer"));
    query.addBindValue(record.value("device"));
    query.addBindValue(record.value("val_split"));
    query.addBindValue(record.value("map50"));
    query.addBindValue(record.value("map50_95"));
    query.addBindValue(record.value("precision"));
    query.addBindValue(record.value("recall"));
    query.addBindValue(record.value("model_path"));
    query.addBindValue(record.value("train_time"));

    if (!query.exec()) {
        qWarning() << "保存训练记录失败:" << query.lastError().text();
        return -1;
    }
    return query.lastInsertId().toInt();
}

QVector<QVariantMap> DatabaseManager::getAllTrainingRecords()
{
    QVector<QVariantMap> results;
    QSqlQuery query(m_db);
    query.exec("SELECT * FROM training_records ORDER BY id DESC");

    while (query.next()) {
        QVariantMap row;
        QSqlRecord rec = query.record();
        for (int i = 0; i < rec.count(); ++i) {
            row[rec.fieldName(i)] = query.value(i);
        }
        results.append(row);
    }
    return results;
}

QVariantMap DatabaseManager::getTrainingRecord(int id)
{
    QVariantMap result;
    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM training_records WHERE id = ?");
    query.addBindValue(id);
    if (query.exec() && query.next()) {
        QSqlRecord rec = query.record();
        for (int i = 0; i < rec.count(); ++i) {
            result[rec.fieldName(i)] = query.value(i);
        }
    }
    return result;
}

bool DatabaseManager::deleteTrainingRecord(int id)
{
    QSqlQuery query(m_db);
    query.prepare("DELETE FROM training_records WHERE id = ?");
    query.addBindValue(id);
    return query.exec();
}

// ============================================================
//  推理记录
// ============================================================
int DatabaseManager::saveInferenceRecord(const QVariantMap& record)
{
    QSqlQuery query(m_db);
    query.prepare(R"(
        INSERT INTO inference_records
        (timestamp, model_path, image_path, image_name,
         conf_threshold, iou_threshold, detection_count,
         avg_confidence, result_image_path)
        VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)
    )");

    query.addBindValue(record.value("timestamp", QDateTime::currentDateTime().toString(Qt::ISODate)));
    query.addBindValue(record.value("model_path"));
    query.addBindValue(record.value("image_path"));
    query.addBindValue(record.value("image_name"));
    query.addBindValue(record.value("conf_threshold"));
    query.addBindValue(record.value("iou_threshold"));
    query.addBindValue(record.value("detection_count"));
    query.addBindValue(record.value("avg_confidence"));
    query.addBindValue(record.value("result_image_path"));

    if (!query.exec()) {
        qWarning() << "保存推理记录失败:" << query.lastError().text();
        return -1;
    }
    return query.lastInsertId().toInt();
}

QVector<QVariantMap> DatabaseManager::getAllInferenceRecords()
{
    QVector<QVariantMap> results;
    QSqlQuery query(m_db);
    query.exec("SELECT * FROM inference_records ORDER BY id DESC");

    while (query.next()) {
        QVariantMap row;
        QSqlRecord rec = query.record();
        for (int i = 0; i < rec.count(); ++i) {
            row[rec.fieldName(i)] = query.value(i);
        }
        results.append(row);
    }
    return results;
}

QVariantMap DatabaseManager::getInferenceRecord(int id)
{
    QVariantMap result;
    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM inference_records WHERE id = ?");
    query.addBindValue(id);
    if (query.exec() && query.next()) {
        QSqlRecord rec = query.record();
        for (int i = 0; i < rec.count(); ++i) {
            result[rec.fieldName(i)] = query.value(i);
        }
    }
    return result;
}

bool DatabaseManager::saveDetectionDetail(int inferenceId, const QVariantMap& detection)
{
    QSqlQuery query(m_db);
    query.prepare(R"(
        INSERT INTO detection_details (inference_id, class_name, x, y, width, height, confidence)
        VALUES (?, ?, ?, ?, ?, ?, ?)
    )");
    query.addBindValue(inferenceId);
    query.addBindValue(detection.value("class_name"));
    query.addBindValue(detection.value("x"));
    query.addBindValue(detection.value("y"));
    query.addBindValue(detection.value("width"));
    query.addBindValue(detection.value("height"));
    query.addBindValue(detection.value("confidence"));
    return query.exec();
}

QVector<QVariantMap> DatabaseManager::getDetectionDetails(int inferenceId)
{
    QVector<QVariantMap> results;
    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM detection_details WHERE inference_id = ?");
    query.addBindValue(inferenceId);
    query.exec();

    while (query.next()) {
        QVariantMap row;
        QSqlRecord rec = query.record();
        for (int i = 0; i < rec.count(); ++i) {
            row[rec.fieldName(i)] = query.value(i);
        }
        results.append(row);
    }
    return results;
}

bool DatabaseManager::deleteInferenceRecord(int id)
{
    QSqlQuery query(m_db);
    query.prepare("DELETE FROM inference_records WHERE id = ?");
    query.addBindValue(id);
    return query.exec();
}

// ============================================================
//  类别记录
// ============================================================
bool DatabaseManager::saveCategories(const QStringList& categories)
{
    QSqlQuery query(m_db);
    query.exec("DELETE FROM categories");
    query.prepare("INSERT INTO categories (name) VALUES (?)");
    for (const QString& cat : categories) {
        query.addBindValue(cat);
        if (!query.exec()) {
            qWarning() << "保存类别失败:" << query.lastError().text();
            return false;
        }
    }
    return true;
}

QStringList DatabaseManager::loadCategories()
{
    QStringList cats;
    QSqlQuery query(m_db);
    query.exec("SELECT name FROM categories ORDER BY id");
    while (query.next()) {
        cats.append(query.value(0).toString());
    }
    return cats;
}

// ============================================================
//  标注项目记录
// ============================================================
int DatabaseManager::saveAnnotationProject(const QVariantMap& project)
{
    QSqlQuery query(m_db);
    query.prepare(R"(
        INSERT INTO annotation_projects
        (timestamp, project_name, image_count, annotation_count, categories, image_dir)
        VALUES (?, ?, ?, ?, ?, ?)
    )");
    query.addBindValue(project.value("timestamp", QDateTime::currentDateTime().toString(Qt::ISODate)));
    query.addBindValue(project.value("project_name"));
    query.addBindValue(project.value("image_count"));
    query.addBindValue(project.value("annotation_count"));
    query.addBindValue(project.value("categories"));
    query.addBindValue(project.value("image_dir"));

    if (!query.exec()) {
        qWarning() << "保存标注项目失败:" << query.lastError().text();
        return -1;
    }
    return query.lastInsertId().toInt();
}

QVector<QVariantMap> DatabaseManager::getAllAnnotationProjects()
{
    QVector<QVariantMap> results;
    QSqlQuery query(m_db);
    query.exec("SELECT * FROM annotation_projects ORDER BY id DESC");

    while (query.next()) {
        QVariantMap row;
        QSqlRecord rec = query.record();
        for (int i = 0; i < rec.count(); ++i) {
            row[rec.fieldName(i)] = query.value(i);
        }
        results.append(row);
    }
    return results;
}

bool DatabaseManager::deleteAnnotationProject(int id)
{
    QSqlQuery query(m_db);
    query.prepare("DELETE FROM annotation_projects WHERE id = ?");
    query.addBindValue(id);
    return query.exec();
}

// ============================================================
//  统计
// ============================================================
int DatabaseManager::getTrainingCount()
{
    QSqlQuery query(m_db);
    query.exec("SELECT COUNT(*) FROM training_records");
    if (query.next()) return query.value(0).toInt();
    return 0;
}

int DatabaseManager::getInferenceCount()
{
    QSqlQuery query(m_db);
    query.exec("SELECT COUNT(*) FROM inference_records");
    if (query.next()) return query.value(0).toInt();
    return 0;
}
