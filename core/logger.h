#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QtQml>

#include "defines.h"
class QTimer;
class QSqlQuery;

#include <QList>
#include <QPointF>
Q_DECLARE_METATYPE(QList<QPointF>)

class LoggerSession;
class Logger : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QVector<ModbusValue*> values WRITE setValues FINAL)
    Q_PROPERTY(QString filePath WRITE connect_db FINAL)
    Q_PROPERTY(bool isWrite READ isWrite WRITE setWrite FINAL)
    Q_PROPERTY(bool isRead READ isRead WRITE setRead FINAL)
    Q_PROPERTY(QList<LoggerSession*> sessions READ sessions NOTIFY sessionsChanged)
public:
    explicit Logger(QObject *parent = nullptr);
    ~Logger();

    void setWrite(bool v);
    bool isWrite() const {return m_isWrite;}
    void setRead(bool v);
    bool isRead() const {return m_isRead;}

    Q_INVOKABLE bool connect_db(const QString &filePath = "./test_2.sqlite");

    QList<LoggerSession*> sessions() {return m_sessions;}

    QList<QPointF> getValuesPoint(const QString &hash, QDateTime start, QDateTime finish);
    Q_INVOKABLE QVariant getValuesPoint_var(const QString &hash, QDateTime start, QDateTime finish) {
        return QVariant::fromValue(getValuesPoint(hash, start, finish));
    }

public slots:
    void setValues(const QVector<ModbusValue*> &values) {m_values=values; update_value_table();}
    void pushValues();
    void readValues();
signals:
    void sessionsChanged();
private:
    QList<LoggerSession*> m_sessions;
    QVector<ModbusValue*> m_values;
    QStringList m_values_hash;
    QMap<QString, ModbusValue*> m_values_;
    QTimer *m_updateValues=nullptr;
    QSqlQuery *m_queryRead = nullptr;
    QDateTime m_start, m_finish;

    bool m_isWrite = false, m_isRead = false;
    QVariantList v_hash, v_value, v_dt;
    QThread workerThread;

    void create_tables();
    void test_printCount();
    void update_value_table();
    void read_sessions();
    void query_read();

private slots:
    void commit_values();
};

class LoggerSession : public QObject
{
//    Q_GADGET
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QDateTime start READ start CONSTANT FINAL)
    Q_PROPERTY(QDateTime finish READ finish CONSTANT FINAL)
public:
    LoggerSession(QDateTime start, QDateTime finish, QObject *parent = nullptr);
    LoggerSession(const QString &query, QObject *parent = nullptr);
    ~LoggerSession();

    void setRange(QDateTime start, QDateTime finish);
    QDateTime start() const {return m_start;}
    QDateTime finish() const {return m_finish;}

    Q_INVOKABLE QStringList getValuesHash() const;
    Q_INVOKABLE QList<QPointF> getValues(const QString &hash);

private:
    QString m_query;
    QDateTime m_start, m_finish;
    quint16 m_id;
};

#endif // LOGGER_H
