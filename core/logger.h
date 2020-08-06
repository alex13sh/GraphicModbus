#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QtQml>
#include <QSqlDatabase>

#include "defines.h"
class QTimer;
class QSqlQuery;

class LoggerSession;
class Logger : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QVector<ModbusValue*> values WRITE setValues FINAL)
    Q_PROPERTY(QString filePath WRITE connect_db FINAL)
    Q_PROPERTY(bool isWrite READ isWrite WRITE setWrite FINAL)
    Q_PROPERTY(bool isRead READ isRead WRITE setRead FINAL)
    Q_PROPERTY(QList<LoggerSession*> sessions READ sessions CONSTANT)
public:
    explicit Logger(QObject *parent = nullptr);
    ~Logger();

    void setWrite(bool v);
    bool isWrite() const {return m_isWrite;}
    void setRead(bool v);
    bool isRead() const {return m_isRead;}

    Q_INVOKABLE bool connect_db(const QString &filePath = "./test_2.sqlite");

    QList<LoggerSession*> sessions() {return m_sessions;}

public slots:
    void setValues(const QVector<ModbusValue*> &values) {m_values=values; update_value_table();}
    void pushValues();
    void readValues();
signals:

private:
    QList<LoggerSession*> m_sessions;
    QVector<ModbusValue*> m_values;
    QStringList m_values_hash;
    QMap<QString, ModbusValue*> m_values_;
    QTimer *m_updateValues=nullptr;
    QSqlDatabase m_sdb;
    QSqlQuery *m_queryRead = nullptr;
    QDateTime m_start, m_finish;

    bool m_isWrite = false, m_isRead = false;

    void create_tables();
    void test_printCount();
    void update_value_table();
    void read_sessions();
    void query_read();
};

class LoggerSession : public QObject
{
//    Q_GADGET
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QDateTime start READ start CONSTANT FINAL)
    Q_PROPERTY(QDateTime finish READ finish CONSTANT FINAL)
public:
    LoggerSession(const QString &query, const QSqlDatabase *db, QObject *parent = nullptr);
    ~LoggerSession();

    void setRange(QDateTime start, QDateTime finish);
    QDateTime start() const {return m_start;}
    QDateTime finish() const {return m_finish;}

    Q_INVOKABLE QStringList getValuesHash() const;

private:
    QString m_query;
    const QSqlDatabase *m_db;
    QDateTime m_start, m_finish;
    quint16 m_id;
};

#endif // LOGGER_H
