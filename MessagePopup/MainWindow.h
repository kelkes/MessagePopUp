#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QKeyEvent>
#include <QTime>
#include <QList>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:   
    void keyPressEvent(QKeyEvent* event);
    void mousePressEvent(QMouseEvent* event);
    bool eventFilter(QObject *obj, QEvent *event);
private:
    Ui::MainWindow *ui;
    QTimer* _timer;
    QTimer* _popUpTimer;

    QList<QTime> _ticks;
    int _tickIdx;
    QTime _nextTick;
    QTime _start;
    QTime _end;
    int _popUpCount;
    int _operationMode;

    void showWindow();


private slots:
    void timeout();
    void popUpTimeout();

};

#endif // MAINWINDOW_H
