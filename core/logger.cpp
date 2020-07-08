#include "logger.h"

//#include <QtSql>
//#include <QSqlDatabase>
#include <QTimer>
#include <QDebug>

Logger::Logger(QObject *parent) : QObject(parent)
{

    m_updateValues = new QTimer(this);
    m_updateValues->setInterval(1000);
    connect(m_updateValues, &QTimer::timeout, this, &Logger::pushValues);

    m_sdb = QSqlDatabase::addDatabase("QSQLITE");
    m_sdb.setDatabaseName("./test_1.sqlite");

    if (!m_sdb.open()) {
        qDebug()<<"Sql is not open. Error:"<<m_sdb.lastError().text();
        return;
    }
    else qDebug()<<"Sql is open";

    QSqlQuery a_query(m_sdb);
    // DDL query
    QString str = "CREATE TABLE values_table ("
           "name VARCHAR(255),"
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

    QString execute = "SELECT COUNT(datetime) FROM values_table";
    a_query.exec(execute);
    qDebug()<<"MyQuery Error:"<<a_query.lastError().text();
    qDebug()<<"MyQuery Value:"<<a_query.value(0).toInt();

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

    my_query.prepare("INSERT INTO values_table (name, value, datetime)"
                                  "VALUES (:name, :value, :datetime);");
    QDateTime cur = QDateTime::currentDateTime();
    QVariantList v_name, v_value, v_dt;
    for(auto v:m_values){
        cur = cur.addMSecs(1);
        auto txt_name = QString("%1) %2/%3 - %4/%5")
                .arg(v->countUpdate()).arg(v->module()->name())
                .arg(v->sensor()->pin()).arg(v->sensor()->name())
                .arg(v->name());
        v_name<<txt_name;
        v_value<<v->value_int();
        v_dt<<cur;
    }
    my_query.bindValue(":name", v_name);
    my_query.bindValue(":value", v_value);
    my_query.bindValue(":datetime", v_dt);
        if(!my_query.execBatch())
            qDebug()<<"Error:"<<my_query.lastError().text();
}
