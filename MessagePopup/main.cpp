#include <QtGui/QApplication>
#include "MainWindow.h"

#include <QSettings>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;    
    QSettings* settings = new QSettings("config.ini",QSettings::IniFormat,0);
    int operationMode = settings->value("System/operationmode",1).toInt();
    if (operationMode == 1) {
        w.show();
    } else {
        w.showFullScreen();
    }
    delete settings;

    return a.exec();
}
