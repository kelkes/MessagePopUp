#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    _timer = new QTimer(this);
    _timer->setInterval(10000);
    QObject::connect(_timer,SIGNAL(timeout()),this,SLOT(timeout()));
    _timer->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::event ( QEvent * e )
{
    // Somehow the correct state of window is not getting set,
    // so doing it manually
    if( e->type() == QEvent::Hide)
    {
        this->setWindowState(Qt::WindowMinimized);
    }
    else if( e->type() == QEvent::Show )
    {
        this->setWindowState((this->windowState() & ~Qt::WindowMinimized) |
                             Qt::WindowActive);
    }
    return QWidget::event(e);
}

void MainWindow::timeout() {
    this ->showMinimized(); // This is to bring up the window if not minimized
                            // but beneath some other window

    this ->setWindowState(Qt::WindowActive);
    this ->showNormal();
}
