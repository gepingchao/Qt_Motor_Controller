#-------------------------------------------------
#
# Project created by QtCreator 2017-02-23T09:38:51
#
#-------------------------------------------------

QT       += core gui
QT       += core gui network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

RC_FILE += yun_tai.rc

TARGET = yuntai
TEMPLATE = app


SOURCES += main.cpp\
        yuntai.cpp \
    crc.cpp \
    mytcpserver.cpp \
    mytcpclient.cpp \
    protocol.cpp

HEADERS  += yuntai.h \
    myhelper.h \
    mytcpserver.h \
    mytcpclient.h \
    crc.h \
    protocol.h

FORMS    += yuntai.ui
