TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
LIBS += -lpthread

SOURCES += main.c \
    websocket_common.c \
    uart_common.c

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    websocket_common.h \
    uart_common.h

