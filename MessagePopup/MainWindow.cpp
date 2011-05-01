#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QFile>
#include <windows.h>
#include <QPixmap>
#include <QSettings>
#include <QDesktopWidget>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QSettings* settings = new QSettings("config.ini",QSettings::IniFormat,this);
    int ms = settings->value("Timer/ms",5000).toInt();
    QString text = settings->value("Message/text","Value").toString();
    int x = settings->value("Position/x").toInt();
    int y = settings->value("Position/y").toInt();
    int width = settings->value("Size/width").toInt();
    int height = settings->value("Size/height").toInt();
    QString background = settings->value("Color/background").toString();
    QString foreground = settings->value("Color/foreground").toString();
    QString logoFileName = settings->value("Message/logo").toString();

    delete settings;

    ui->setupUi(this);

    if ((logoFileName == "") || (!QFile::exists(logoFileName))) {
        ui->logo->setVisible(false);
    } else {
        QPixmap logo(logoFileName);
        ui->logo->setPixmap( logo );
    }

    //QRect frect = frameGeometry();
    //frect.moveCenter(QDesktopWidget().availableGeometry().center());
    //move(frect.topLeft());

    move(x,y);
    resize(width,height);

    QString backgroundStyleSheet = QString("background-color: %1").arg(background);
    ui->centralWidget->setStyleSheet(backgroundStyleSheet);

    QString foregroundStyleSheet = QString("color: %1").arg(foreground);
    ui->label->setStyleSheet(foregroundStyleSheet);

    this->setWindowFlags(Qt::WindowStaysOnTopHint); //Set a frameless window
    //this->setWindowFlags(Qt::CustomizeWindowHint); //Set window with no title bar
    //this->setWindowFlags(Qt::FramelessWindowHint); //Set a frameless window

    ui->label->setText(text);
    ui->label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    _timer = new QTimer(this);
    _timer->setInterval(ms);
    QObject::connect(_timer,SIGNAL(timeout()),this,SLOT(timeout()));
    _timer->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent* event) {

    if (event->key() == Qt::Key_F7) {
        this->close();
    } else {
        this->hide();
    }

    QWidget::keyPressEvent(event);
}


void MainWindow::timeout() {

    show();

    //SetWindowPos( winId(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
    //SetWindowPos( winId(), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );

    raise();
    activateWindow();
    raise();
}
