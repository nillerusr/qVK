#-------------------------------------------------
#
# Project created by QtCreator 2020-05-15T14:27:45
#
#-------------------------------------------------

QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = vk-client
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
    database.cpp \
    downloadmanager.cpp \
        main.cpp \
    loginwindow.cpp \
    messageswindow.cpp \
    dialogwidget.cpp \
    longpoll.cpp \
    messagewidget.cpp \
    utils.cpp \
    vksdk.cpp \
    wlabel.cpp \
    wscrollarea.cpp \
    wtextedit.cpp

HEADERS += \
    database.h \
    downloadmanager.h \
    loginwindow.h \
    messageswindow.h \
    dialogwidget.h \
    longpoll.h \
    messagewidget.h \
    utils.h \
    vksdk.h \
    wlabel.h \
    wscrollarea.h \
    wtextedit.h

FORMS += \
    loginwindow.ui \
    messageswindow.ui \
    dialogwidget.ui \
    messagewidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    icons.qrc
