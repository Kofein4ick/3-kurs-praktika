QT       += core gui
QT += widgets
greaterThan(QT_MAJOR_VERSION, 4):
QT+= network
CONFIG += c++11


# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        TcpClient.cpp \
        TcpServer.cpp \
        downloadform.cpp \
        imagedelegate.cpp \
        main.cpp \
        mainwindow.cpp \
        serverform.cpp \
        widget.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

FORMS += \
    downloadform.ui \
    mainwindow.ui \
    serverform.ui \
    widget.ui

HEADERS += \
    TcpClient.h \
    TcpServer.h \
    downloadform.h \
    imagedelegate.h \
    mainwindow.h \
    serverform.h \
    widget.h
