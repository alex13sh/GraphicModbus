#include "logger.h"

#include <QtSql>
//#include <QSqlDatabase>
#include <QTimer>
#include <QDebug>

Logger::Logger(QObject *parent) : QObject(parent)
{
//    if(!connect_db()) return;
//    create_tables();
//    test_printCount();

    m_start = QDateTime::currentDateTime();
//    m_updateValues->start();
}

Logger::~Logger(){
    setWrite(false);
//    QSqlDatabase::database().close();
}

void Logger::setWrite(bool v) {
    if (m_isWrite == v) return;
    m_isWrite = v;
    if (m_isRead && m_isWrite) setRead(false);
    if (v) {
        m_start = QDateTime::currentDateTime();
        v_hash.clear();
        v_value.clear();
        v_dt.clear();
    } else {
        m_finish = QDateTime::currentDateTime();
        commit_values();
        QSqlQuery my_query;
        my_query.prepare("INSERT INTO session_table (start, finish)"
                                      "VALUES (:start, :finish);");
        my_query.bindValue(":start", m_start);
        my_query.bindValue(":finish", m_finish);
        my_query.exec();
        m_sessions.push_front(new LoggerSession(m_start, m_finish, this));
        emit sessionsChanged();
    }
}

void Logger::setRead(bool v) {
    if (m_isRead == v) return;
    m_isRead = v;
    if (m_isRead && m_isWrite) setWrite(false);
    query_read();
}

#include "modbusvalue.h"
#include "modbussensor.h"
#include "modbusdevice.h"
#include <QDateTime>
void Logger::pushValues() {

    QDateTime cur = QDateTime::currentDateTime();

    for(auto v:m_values){
        cur = cur.addMSecs(1);
        v_hash<<v->hash_str().left(10);
        v_value<<v->value_uint32();
        v_dt<<cur;

#ifdef LOGGER_VALUES_INT32
        v_value_int32 << v->value_int32();
        v_value_uint32<< v->value_uint32();
        v_value_float << v->value_float();
#endif
    }

}

void Logger::readValues()
{
    if (!m_isRead) return;
//    static QSqlQuery m_queryRead("select * from values_table", m_sdb);

    if (!m_queryRead->next()) return;
//    QDateTime dt = a_query.value("datetime").toDateTime();
    QString hash = m_queryRead->value("value_hash").toString();
    quint32 value = m_queryRead->value("value").toUInt();
    static QDateTime dt_n, dt_p;
    dt_n = m_queryRead->value("datetime").toDateTime();
    if (dt_p.addSecs(5)<dt_n) {
        qDebug()<<"Logger::readValues hash:"<<hash<<"; value:"<<value<<"; date:"<<dt_n;
        dt_p = dt_n;
    }
    if (m_values_.contains(hash)) m_values_[hash]->setValue_uint32(value);
//    a_query.previous();
}

bool Logger::connect_db(const QString &filePath) {

    auto m_sdb = QSqlDatabase::database();
    m_sdb.setDatabaseName(filePath);
//    qDebug()<<"connectOptions:"<<m_sdb.connectOptions();
//    m_sdb.setConnectOptions("QSQLITE_OPEN_URI;QSQLITE_ENABLE_SHARED_CACHE");


    if (!m_sdb.open()) {
        qDebug()<<"Sql is not open. Error:"<<m_sdb.lastError().text();
        return false;
    }
    else qDebug()<<"Sql is open";
    create_tables();
    query_read();
    read_sessions();
    return true;
}

void Logger::openJsonFile(const QString &fileName)
{
    QFile file;
    file.setFileName("./log/"+fileName);
    file.open(QIODevice::ReadOnly);
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    auto jobj = doc.object();
    v_hash = jobj.take("v_hash").toArray().toVariantList();
    v_value = jobj.take("v_value").toArray().toVariantList();
    v_dt = jobj.take("v_dt").toArray().toVariantList();
}

QList<QPointF> Logger::getValuesPoint(const QString &hash, QDateTime start, QDateTime finish)
{
    qDebug()<<"__getValues hash:"<<hash<<";";
    QList<QPointF> res;
#ifdef LOGGER_SQL
    QSqlQuery q;
    q.prepare("select value_hash, value, datetime from values_table where datetime >= :start and datetime <= :finish and value_hash = :hash;");
    q.bindValue(":start", start);
    q.bindValue(":finish", finish);
    q.bindValue(":hash", hash);
    if(!q.exec())
        qDebug()<<"MyQuery Error:"<<q.lastError().text();

    while (q.next()){
        QDateTime dt = q.value(2).toDateTime();
        quint32 val = q.value(1).toInt();
        quint64 msec = dt.toMSecsSinceEpoch();
        res.append(QPointF(msec, val));
    }
#endif

#ifdef LOGGER_JSON
    openJsonFile(QString("./values_%1.json").arg(start.toString("dd_MM_yyyy__hh_mm_ss_zzz")));
    for (int i=0; i<v_value.size(); ++i){
        if (v_hash[i].toString()==hash) {
            QDateTime dt =v_dt[i].toDateTime();
            quint32 val = v_value[i].toInt();
            quint64 msec = dt.toMSecsSinceEpoch();
            res.append(QPointF(msec, val));
        }
    }
#endif

    return res;
}

void Logger::create_tables() {
    QSqlQuery a_query;
    // DDL query
    QString str = "CREATE TABLE values_table ("
           "value_hash VARCHAR(20),"
           "value integer,"
           "datetime datetime PRIMARY KEY NOT NULL"
           ");";
    if (a_query.exec(str))
           qDebug() << "Вроде не удается создать таблицу, провертье карманы!";
    qDebug()<<"Error:"<<a_query.lastError().text();

    str = "CREATE TABLE session_table ("
           "start datetime PRIMARY KEY NOT NULL,"
           "finish datetime"
           ");";
    if (a_query.exec(str))
           qDebug() << "Вроде не удается создать таблицу, провертье карманы!";
    qDebug()<<"Error:"<<a_query.lastError().text();

    str = "CREATE TABLE value_table ("
            "hash VARCHAR(32) PRIMARY KEY NOT NULL," // QCryptographicHash::Sha256
            "value_name VARCHAR(255),"
            "value_address integer,"
            "value_type integer,"
            "sensor_name VARCHAR(255),"
            "sensor_pin integer,"
            "module_name VARCHAR(255),"
            "summary VARCHAR(255)" // module_name/sensor_pin - sensor_name/name
           ");";
    if (a_query.exec(str))
           qDebug() << "Вроде не удается создать таблицу, провертье карманы!";
    qDebug()<<"Error:"<<a_query.lastError().text();
}

void Logger::test_printCount() {
    QSqlQuery a_query;
    a_query.prepare("select COUNT(datetime) as cnt from values_table;");
//    a_query.bindValue(":today", QDateTime::currentDateTime());
    if(a_query.exec()) qDebug()<<"MyQuery query is done";
    qDebug()<<"MyQuery Error:"<<a_query.lastError().text();
    if(a_query.next())
        qDebug()<<"MyQuery Value cnt:"<<a_query.value("cnt");
}

void Logger::update_value_table() {
    qDebug()<<"Logger::update_value_table m_values:"<<m_values.size();
    QSqlQuery a_query;
    QString summary, hash,
            value_name, sensor_name, module_name;
    int value_address, value_type, sensor_pin;
    for(auto v: m_values){
        hash = v->hash_str();
        if (hash.isEmpty()) continue;
//        qDebug()<<"Hash:"<<hash;
        a_query.prepare("select value_name from value_table where hash=:hash");
        a_query.bindValue(":hash", hash);
        a_query.exec();
        if(a_query.next()){
            qDebug()<<"Value_name:"<< a_query.value("value_name").toString();
        }else{
            value_name = v->name();
            value_address = v->address();
            value_type = int(v->type());
            sensor_name = v->sensor()->name();
            sensor_pin = v->sensor()->pin();
            module_name = v->module()->name();
            summary = module_name+"/"+QString::number(sensor_pin)+" - "+sensor_name+"/"+value_name;

            a_query.prepare("INSERT INTO value_table (hash, value_name, value_address, value_type,"
                                    "sensor_name, sensor_pin, module_name, summary)"
                            "VALUES (:hash, :value_name, :value_address, :value_type,"
                            ":sensor_name, :sensor_pin, :module_name, :summary);");
            a_query.bindValue(":hash", hash);
            a_query.bindValue(":summary", summary);

            a_query.bindValue(":value_name", value_name);
            a_query.bindValue(":value_address", value_address);
            a_query.bindValue(":value_type", value_type);

            a_query.bindValue(":sensor_name", sensor_name);
            a_query.bindValue(":sensor_pin", sensor_pin);
            a_query.bindValue(":module_name", module_name);

            a_query.exec();
        }
        m_values_hash<<hash;
        m_values_.insert(hash.left(10), v);
    }
    qDebug()<<"Logger::update_value_table end";
}

void Logger::read_sessions()
{
    QSqlQuery q;
    q.exec("select * from session_table");
    m_sessions.clear();
    while(q.next()){
        auto s = new LoggerSession(
            q.value("start").toDateTime(), q.value("finish").toDateTime()
                    , this);
        m_sessions.push_front(s);
    }
}

void Logger::query_read()
{
    if (m_queryRead) delete m_queryRead;
//    m_queryRead = new QSqlQuery("select * from values_table where "
////                                "   value_hash='56d69d8552'"
////                                "or value_hash='a439b8f7f2'"
////                                "or value_hash='1029а3с7в6'"
//                                " value_hash='86c6deedfb'"
//                                ";", m_sdb);
    m_queryRead = new QSqlQuery;
//    m_queryRead->prepare("select * from values_table where value_hash = ANY (:hash)");
    QStringList v_hash;
    for (auto v:m_values)
        v_hash<<"'"+v->hash_str().left(10)+"'";

//    m_queryRead->bindValue(":hash", v_hash.join(", "));
    auto txt = QString("select * from values_table where value_hash  IN (%1);").arg(v_hash.join(", "));
    qDebug()<< "Logger::query_read hash:"<<txt;
//    if (!m_queryRead->execBatch(QSqlQuery::ValuesAsColumns))
    if (!m_queryRead->exec(txt))
//    if (!m_queryRead->exec("select * from values_table;"))
        qDebug()<<"Error:"<<m_queryRead->lastError().text();

}

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
void Logger::commit_values()
{
//    if (!m_isWrite) return;
    if (v_value.isEmpty()) return;

#ifdef LOGGER_JSON
    QJsonObject jobj;
    jobj["start"] = m_start.toString();
    jobj["finish"] = m_finish.toString();
    jobj["v_hash"] = QJsonArray::fromVariantList(v_hash);
    jobj["v_value"] = QJsonArray::fromVariantList(v_value);
    jobj["v_dt"] = QJsonArray::fromVariantList(v_dt);

#ifdef LOGGER_VALUES_INT32
    jobj["v_value_int32"] = QJsonArray::fromVariantList(v_value_int32);
    jobj["v_value_uint32"] = QJsonArray::fromVariantList(v_value_uint32);
    jobj["v_value_float"] = QJsonArray::fromVariantList(v_value_float);
#endif

    QJsonDocument doc(jobj);
    QFile file;
    file.setFileName(QString("./log/values_%1.json").arg(m_start.toString("dd_MM_yyyy__hh_mm_ss_zzz")));
    file.open(QIODevice::WriteOnly);
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();
#endif

#ifdef LOGGER_SQL
    QSqlQuery my_query;

    my_query.prepare("INSERT INTO values_table (value_hash, value, datetime)"
                                  "VALUES (:hash, :value, :datetime);");

    my_query.bindValue(":hash", v_hash);
    my_query.bindValue(":value", v_value);
    my_query.bindValue(":datetime", v_dt);
        if(!my_query.execBatch())
            qDebug()<<"Error:"<<my_query.lastError().text();
#endif

    v_hash.clear();
    v_value.clear();
    v_dt.clear();

#ifdef LOGGER_VALUES_INT32
    v_value_float.clear();
    v_value_int32.clear();
    v_value_uint32.clear();
#endif
}


LoggerSession::LoggerSession(QDateTime start, QDateTime finish, QObject *parent)
    : QObject(parent)
{
    setRange(start, finish);
}

LoggerSession::LoggerSession(const QString &query, QObject *parent)
    : QObject(parent), m_query(query)
{
}

LoggerSession::~LoggerSession()
{
}

void LoggerSession::setRange(QDateTime start, QDateTime finish)
{
    m_start = start;
    m_finish = finish;
}

QStringList LoggerSession::getValuesHash() const
{
    QSqlQuery q;
    QString txt ="select * from values_table where datetime >= :start and datetime <= :finish";
//    txt = txt.left(txt.length()-1);
    txt = QString("select DISTINCT value_hash as hash from (%1)").arg(txt);
    qDebug()<<"getValuesHash: "<<txt;
    q.prepare(txt);
    q.bindValue(":start", m_start);
    q.bindValue(":finish", m_finish);
    if(!q.exec()) {
        qDebug()<<"Error:"<<q.lastError().text();
        return {};
    }
    QStringList res;
    while(q.next())
        res.append(q.value(0).toString());
    qDebug()<<"LoggerSession::getValuesHash res:"<<res;
    return res;
}

QList<QPointF> LoggerSession::getValues(const QString &hash)
{
    QSqlQuery q;
    q.prepare("select value, datetime from value_table where datetime >= :start and datetime <= :finish and value_hash = :hash");
    q.bindValue(":start", m_start);
    q.bindValue(":finish", m_finish);
    q.bindValue(":hash", hash);

    QList<QPointF> res;
    while (q.next()){
        QDateTime dt = q.value(1).toDateTime();
        res.append(QPointF(dt.toMSecsSinceEpoch(), q.value(0).toInt()));
    }
    return res;
}
