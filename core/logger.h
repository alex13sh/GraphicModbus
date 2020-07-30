#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QtQml>
#include <QSqlDatabase>
#include "defines.h"
class QTimer;

class Logger : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QVector<ModbusValue*> values WRITE setValues FINAL)
public:
    explicit Logger(QObject *parent = nullptr);
    ~Logger();

public slots:
    void setValues(const QVector<ModbusValue*> &values) {m_values=values; update_value_table();}
    void pushValues();
signals:

private:
    QVector<ModbusValue*> m_values;
    QStringList m_values_hash;
    QTimer *m_updateValues=nullptr;
    QSqlDatabase m_sdb;
    QDateTime m_start, m_finish;

    bool connect_db();
    void create_tables();
    void test_printCount();
    void update_value_table();
};

#endif // LOGGER_H
