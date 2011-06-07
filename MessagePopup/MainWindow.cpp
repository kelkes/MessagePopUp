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


//Flags FramelessWindowHint und WindowStaysOnTopHint nicht entfernen, sonst kann sich die
//Anwendung nicht über die Software der Waage legen
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent, Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint),
    ui(new Ui::MainWindow)
{
    setCursor(Qt::BlankCursor);
    //Settings auslesen
    QSettings* settings = new QSettings("config.ini",QSettings::IniFormat,this);
    int check = settings->value("Timer/check",10000).toInt();
    QString startTimeString = settings->value("Timer/start","06:00").toString();
    QString endTimeString = settings->value("Timer/end","17:00").toString();
    QString ticksString = settings->value("Timer/ticks","").toString();    
    int x = settings->value("Position/x",100).toInt();
    int y = settings->value("Position/y",100).toInt();
    int width = settings->value("Size/width",200).toInt();
    int height = settings->value("Size/height",200).toInt();
    QString logoFileName = settings->value("Message/logo","").toString();    
    _operationMode = settings->value("System/operationmode",1).toInt();
    delete settings;

    _start = QTime::fromString(startTimeString+":00");    
    _end = QTime::fromString(endTimeString+":00");    
    QStringList ticksList = ticksString.split('|');
    for(int i=0;i<ticksList.count();i++) {
        QString tickString = ticksList.at(i);
        tickString += ":00";
        QTime time = QTime::fromString(tickString);        
        _ticks.append(time);
    }

    //Nächsten möglichen Alert bestimmen
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

    if (_operationMode == 2) {         
         ui->logo->setGeometry(x,y,width,height);
         ui->logo->updateGeometry();
         ui->logo->setMouseTracking(true);
         ui->logo->installEventFilter(this);
         //Transparenter Hintergrund
         setAttribute(Qt::WA_TranslucentBackground);
         setStyleSheet("background:transparent;");
    } else {
        ui->logo->setGeometry(0,0,width,height);
        ui->logo->updateGeometry();
        move(x,y);
        resize(width,height);
    }
    _timer = new QTimer(this);
    _timer->setInterval(check);
    QObject::connect(_timer,SIGNAL(timeout()),this,SLOT(timeout()));
    _timer->start();

    _popUpTimer = new QTimer(this);
    _popUpTimer->setInterval(10000);
    QObject::connect(_popUpTimer,SIGNAL(timeout()),this,SLOT(popUpTimeout()));


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

bool MainWindow::eventFilter(QObject *obj, QEvent *event) {

    if (event->type() == QEvent::MouseButtonPress) {


             this->hide();
             return true;
         } else {
             // standard event processing
             return QObject::eventFilter(obj, event);
         }
}

void MainWindow::mousePressEvent(QMouseEvent* event) {
    if (_operationMode != 2) this->hide();
    QWidget::mousePressEvent(event);
}

void MainWindow::showWindow() {
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

    raise();
    show();
    activateWindow();
    raise();

}

void MainWindow::popUpTimeout() {
    _popUpCount++;

    showWindow();

    if (_popUpCount >= 3) {
        _popUpTimer->stop();
        this->hide();
    }
}

void MainWindow::timeout() {
    QTime currentTime = QTime::currentTime();
    if (( _start > currentTime ) || (_end < currentTime)) {        
        return;
    }
    if ((_nextTick.hour() == currentTime.hour()) &&
            (_nextTick.minute() == currentTime.minute())) {       
        _tickIdx++;
        if (_tickIdx > _ticks.count()) {
            _tickIdx = 0;
        }
        _nextTick = _ticks.at(_tickIdx);
        if (_operationMode == 1) {
            _popUpCount = 0;
            _popUpTimer->start();
        }

        showWindow();
    } else  {
        while ((_nextTick.hour() < currentTime.hour()) ||
               ((_nextTick.hour() == currentTime.hour()) && (_nextTick.minute() < currentTime.minute()))) {
            _tickIdx++;
            if (_tickIdx > _ticks.count()) {
                _tickIdx = 0;
                _nextTick = _ticks.at(_tickIdx);
                return;
            }
            _nextTick = _ticks.at(_tickIdx);          
        }
    }
}
