#-------------------------------------------------
#
# Project created by QtCreator 2010-11-27T22:07:51
#
#-------------------------------------------------

QT       += core gui

TARGET = MessagePopup
TEMPLATE = app

SOURCES += main.cpp\
        MainWindow.cpp

HEADERS  += MainWindow.h

FORMS    += MainWindow.ui

OTHER_FILES += \
    config.ini

CURR_CONFIG = release

contains( CONFIG, debug ) {
        CURR_CONFIG = debug
}

contains( CONFIG, release ) {
        CURR_CONFIG = release
}

DESTDIR_COPY = $$PWD/$$CURR_CONFIG

copy_config.commands = xcopy \
    /F \
    /Y \
    config.ini \
    "\"$$DESTDIR_COPY"\"

POST_TARGETDEPS += copy_config
QMAKE_EXTRA_TARGETS += copy_config
