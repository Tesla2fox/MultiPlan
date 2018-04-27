#-------------------------------------------------
#
# Project created by QtCreator 2017-10-29T16:34:00
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QPlanGUI
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

DESTDIR=../bin#定义项目编译之后生成的结果文件的存放路径

#win32{#表示Windows环境的特殊设置，这个大括号不能单独放在下一行，否则编译会报错
#    LIBS+=../bin/SmallBoy.lib#包含项目需要用到的共享库
#}

#unix{#表示Linux环境的特殊设置，这个大括号不能单独放在下一行，否则编译会报错
#    LIBS+=../bin/libSmallBoy.so#包含项目需要用到的共享库，注意文件名多了关键字"lib"，扩展名是.so
#}



PROJECT_VERSION_MAJOR = 2
PROJECT_VERSION_MINOR = 2
PROJECT_VERSION_BUILD = 0

DEFINES += "PROJECT_VERSION_MAJOR=$$PROJECT_VERSION_MAJOR"\
       "PROJECT_VERSION_MINOR=$$PROJECT_VERSION_MINOR"\
       "PROJECT_VERSION_BUILD=$$PROJECT_VERSION_BUILD"

#Target version
VERSION = $${PROJECT_VERSION_MAJOR}.$${PROJECT_VERSION_MINOR}.$${PROJECT_VERSION_BUILD}


SOURCES += main.cpp\
        mainwindow.cpp \
    bgeometry.cpp \
    XMl/tinystr.cpp \
    XMl/tinyxml.cpp \
    XMl/tinyxmlerror.cpp \
    XMl/tinyxmlparser.cpp \
    line.cpp \
    Point.cpp \
    polygonob.cpp \
    obmap.cpp \
    Draw.cpp \
    Aplan.cpp \
    splan.cpp \
    code/demodata.cpp \
    stree.cpp \
    Obstacles.cpp

HEADERS  += mainwindow.h \
    bgeometry.h \
    XMl/tinystr.h \
    XMl/tinyxml.h \
    line.h \
    Point.h \
    readmap.hpp \
    polygonob.h \
    obmap.h \
    Draw.h \
    Aplan.h \
    splan.h \
    code/pathPlanning.h \
    code/PointArray.h \
    code/demodata.h \
    stree.h \
    Obstacles.hpp

FORMS    += mainwindow.ui


#add the planner lib
message(current location is $${PWD})
LIBS += -L"/home/robot/qt_code/multiPlan/Library/planner" -lplanner

LibraryUse = qcustomplot libGPS libssconfig libBoost

include(../common.pri)
