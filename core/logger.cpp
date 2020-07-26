#include "logger.h"

#include <QtSql>
//#include <QSqlDatabase>
#include <QTimer>
#include <QDebug>

Logger::Logger(QObject *parent) : QObject(parent)
{

    m_updateValues = new QTimer(this);
    m_updateValues->setInterval(1000);
    connect(m_updateValues, &QTimer::timeout, this, &Logger::pushValues);

    if(!connect_db()) return;
    create_tables();
    test_printCount();

    m_start = QDateTime::currentDateTime();
    m_updateValues->start();
}

Logger::~Logger(){
    m_finish = QDateTime::currentDateTime();
    QSqlQuery my_query(m_sdb);
    my_query.prepare("INSERT INTO session_table (start, finish)"
                                  "VALUES (:start, :finish);");
    my_query.bindValue(":start", m_start);
    my_query.bindValue(":finish", m_finish);
    my_query.exec();
    m_sdb.close();
}

#include "modbusvalue.h"
#include "modbussensor.h"
#include "modbusdevice.h"
#include <QDateTime>
void Logger::pushValues() {
    QSqlQuery my_query(m_sdb);

    my_query.prepare("INSERT INTO values_table (value_hash, value, datetime)"
                                  "VALUES (:hash, :value, :datetime);");
    QDateTime cur = QDateTime::currentDateTime();
    QVariantList v_hash, v_value, v_dt;
    for(auto v:m_values){
        cur = cur.addMSecs(1);
        v_hash<<v->hash_str().left(10);
        v_value<<v->value_int32();
        v_dt<<cur;
    }
    my_query.bindValue(":hash", v_hash);
    my_query.bindValue(":value", v_value);
    my_query.bindValue(":datetime", v_dt);
        if(!my_query.execBatch())
            qDebug()<<"Error:"<<my_query.lastError().text();
}

bool Logger::connect_db() {
    m_sdb = QSqlDatabase::addDatabase("QSQLITE");
    m_sdb.setDatabaseName("./test_2.sqlite");

    if (!m_sdb.open()) {
        qDebug()<<"Sql is not open. Error:"<<m_sdb.lastError().text();
        return false;
    }
    else qDebug()<<"Sql is open";
    return true;
}

void Logger::create_tables() {
    QSqlQuery a_query(m_sdb);
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
    QSqlQuery a_query(m_sdb);
    a_query.prepare("select COUNT(datetime) as cnt from values_table;");
//    a_query.bindValue(":today", QDateTime::currentDateTime());
    if(a_query.exec()) qDebug()<<"MyQuery query is done";
    qDebug()<<"MyQuery Error:"<<a_query.lastError().text();
    if(a_query.next())
        qDebug()<<"MyQuery Value cnt:"<<a_query.value("cnt");
}

void Logger::update_value_table() {
    QSqlQuery a_query(m_sdb);
    QString summary, hash,
            value_name, sensor_name, module_name;
    int value_address, value_type, sensor_pin;
    for(auto v: m_values){
        hash = v->hash_str();
//        qDebug()<<"Hash:"<<hash;
        a_query.prepare("select value_name from value_table where hash=:hash");
        a_query.bindValue(":hash", hash);
        a_query.exec();
        if(a_query.next()){
            qDebug()<<"Value_name:"<< a_query.value("value_name").toString();
            m_values_hash<<hash;
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
            m_values_hash<<hash;
        }
    }
}
