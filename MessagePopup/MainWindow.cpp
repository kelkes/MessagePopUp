#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QFile>
#include <windows.h>
#include <QPixmap>
#include <QSettings>
#include <QDesktopWidget>
#include <QDateTime>
#include <QDebug>
#include <QStringList>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QSettings* settings = new QSettings("config.ini",QSettings::IniFormat,this);
    int check = settings->value("Timer/check",10000).toInt();
    QString startTimeString = settings->value("Timer/start","06:00").toString();
    QString endTimeString = settings->value("Timer/end","17:00").toString();
    QString ticksString = settings->value("Timer/ticks","").toString();
    QString text = settings->value("Message/text","Text").toString();
    int x = settings->value("Position/x",100).toInt();
    int y = settings->value("Position/y",100).toInt();
    int width = settings->value("Size/width",200).toInt();
    int height = settings->value("Size/height",200).toInt();
    QString background = settings->value("Color/background","darkred").toString();
    QString foreground = settings->value("Color/foreground","yellow").toString();
    QString logoFileName = settings->value("Message/logo","").toString();
    delete settings;

    _start = QTime::fromString(startTimeString+":00");
    qDebug() << "Start: " << _start.toString();
    _end = QTime::fromString(endTimeString+":00");
    qDebug() << "End: " << _end.toString();
    QStringList ticksList = ticksString.split('|');
    for(int i=0;i<ticksList.count();i++) {

        QString tickString = ticksList.at(i);
        tickString += ":00";
        QTime time = QTime::fromString(tickString);
        qDebug() << "Tick: " << time.toString();
        _ticks.append(time);
    }

    _tickIdx = 0;
    if (_ticks.count() > 0) {
        _nextTick = _ticks.at(_tickIdx);
    }

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


    this->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::WindowTitleHint | Qt::CustomizeWindowHint);
    //this->setWindowFlags(Qt::CustomizeWindowHint); //Set window with no title bar
    //this->setWindowFlags(Qt::Popup);
    //this->setWindowFlags(Qt::FramelessWindowHint); //Set a frameless window

    ui->label->setText(text);
    ui->label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    _timer = new QTimer(this);
    _timer->setInterval(check);
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

void MainWindow::mousePressEvent(QMouseEvent* event) {
    this->hide();
    QWidget::mousePressEvent(event);
}

void MainWindow::timeout() {

    QTime currentTime = QTime::currentTime();

    qDebug() << "Timeout: " << currentTime.toString();    

    if (( _start > currentTime ) && (_end < currentTime)) {
        qDebug() << "Not in active time";
    }


    //TODO _tickList iterieren zum nächsten möglichen tick
    if ((_nextTick.hour() == currentTime.hour()) &&
            (_nextTick.minute() == currentTime.minute())) {
        qDebug() << "Tick: " << _nextTick.toString();
        _tickIdx++;
        if (_tickIdx > _ticks.count()) {
            _tickIdx = 0;
        }
        _nextTick = _ticks.at(_tickIdx);
        qDebug() << "NextTick: " << _nextTick.toString();

        show();
        raise();
        activateWindow();
        raise();
    } else  {
        while ((_nextTick.hour() < currentTime.hour()) ||
               ((_nextTick.hour() == currentTime.hour()) && (_nextTick.minute() < currentTime.minute()))) {
            _tickIdx++;
            if (_tickIdx > _ticks.count()) {
                _tickIdx = 0;
            }
            _nextTick = _ticks.at(_tickIdx);
            qDebug() << "NextTick: " << _nextTick.toString();
        }
    }


    // THIS IS A HACK:
    // from QT documentation:
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // void QWidget::activateWindow ()
    // ...
    // On Windows, if you are calling this when the application is not currently
    // the active one then it will not make it the active window. It will change
    // the color of the taskbar entry to indicate that the window has changed in
    // some way. This is because Microsoft do not allow an application to
    // interrupt what the user is currently doing in another application.
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // This hack does not give the focus to the app but brings it to front so
    // the user sees it.
    //::SetWindowPos(effectiveWinId(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
    //::SetWindowPos(effectiveWinId(), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
    // HACK END

    //show();

    //SetWindowPos( winId(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
    //SetWindowPos( winId(), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );

    //raise();
    //activateWindow();
    //raise();

    //raise();
    //show();
    //activateWindow();
    //raise();
}
