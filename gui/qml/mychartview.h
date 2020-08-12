#ifndef MYCHARTVIEW_H
#define MYCHARTVIEW_H

#include <QObject>
#include <QQuickItem>
#include <private/declarativechart_p.h>
QT_CHARTS_USE_NAMESPACE

class MyChartView : public DeclarativeChart
{
    Q_OBJECT
    QML_ELEMENT
public:
    MyChartView(QQuickItem *parent = 0);

signals:

};

#endif // MYCHARTVIEW_H
