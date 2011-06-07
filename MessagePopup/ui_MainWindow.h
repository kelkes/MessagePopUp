/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created: Tue 7. Jun 16:36:40 2011
**      by: Qt User Interface Compiler version 4.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *layer;
    QLabel *logo;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(357, 152);
        MainWindow->setStyleSheet(QString::fromUtf8(""));
        layer = new QWidget(MainWindow);
        layer->setObjectName(QString::fromUtf8("layer"));
        layer->setStyleSheet(QString::fromUtf8("background-color: rgba(222, 222, 222, 25);"));
        logo = new QLabel(layer);
        logo->setObjectName(QString::fromUtf8("logo"));
        logo->setGeometry(QRect(130, 50, 46, 13));
        logo->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 0);"));
        MainWindow->setCentralWidget(layer);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Warning", 0, QApplication::UnicodeUTF8));
        logo->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
