#include "testwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QMainWindow w;
    w.resize(1400, 800);
    w.show();
    return a.exec();
}
