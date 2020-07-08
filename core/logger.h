#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QtSql>
#include "defines.h"
class QTimer;

class Logger : public QObject
{
    Q_OBJECT
public:
    explicit Logger(QObject *parent = nullptr);
    ~Logger();

    void setValues(const QVector<ModbusValue*> &values) {m_values=values;}
    void pushValues();

signals:

private:
    QVector<ModbusValue*> m_values;
    QTimer *m_updateValues=nullptr;
    QSqlDatabase m_sdb;
    QDateTime m_start, m_finish;

    bool connect_db();
    void create_tables();
    void test_printCount();
};

#endif // LOGGER_H
