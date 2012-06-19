#-------------------------------------------------
#
# Project created by QtCreator 2012-06-16T20:28:15
#
#-------------------------------------------------

QT       += core gui network

TARGET = QICQ
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qicqserver.cpp \
    mailsender.cpp \
    login.cpp \
    moc_qicqserver.cpp \
    messagebox.cpp \
    users.cpp

HEADERS  += mainwindow.h \
    qicqserver.h \
    mailsender.h \
    login.h \
    messagebox.h \
    users.h

FORMS    += mainwindow.ui \
    login.ui \
    messagebox.ui
