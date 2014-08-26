TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += /usr/include/mysql

LIBS += -L/usr/lib/mysql -lmysqlclient_r

SOURCES += main.cpp \
    connector.cpp \
    connectpool.cpp \
    connectinterface.cpp \
    CException.cpp \
    util.cpp

HEADERS += \
    connector.h \
    connectpool.h \
    connectinterface.h \
    CException.h \
    util.h

