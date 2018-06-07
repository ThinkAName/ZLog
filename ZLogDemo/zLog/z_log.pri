# 日志模块
include(z-file-log/z-file-log.pri)
HEADERS += \
    $$PWD/z_log.h \
    $$PWD/z_log_defines.h

SOURCES += \
    $$PWD/z_log.cpp \
    $$PWD/z_log_defines.cpp

INCLUDEPATH += $$PWD
DEPENDPATH  += $$PWD
