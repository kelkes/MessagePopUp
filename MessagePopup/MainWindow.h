#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

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
    bool event(QEvent* event);

private:
    Ui::MainWindow *ui;
    QTimer* _timer;


private slots:
    void timeout();
};

#endif // MAINWINDOW_H
