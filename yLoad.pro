#-------------------------------------------------
#
# Project created by QtCreator 2012-12-17T23:30:29
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = yLoad
TEMPLATE = app


SOURCES += main.cpp\
    yload.cpp \
    LineEditButton.cpp \
    GetLinks.cpp \
    Flv2m4a.cpp \
    filenameselector.cpp \
    DownloadInfoBox.cpp \
    Downloader.cpp \
    AudioFile.cpp \
    AudioExtractor.cpp \
    Mp4tom4a.cpp \
    MP4AudioFile.cpp

HEADERS  += yload.h \
    LineEditButton.h \
    GetLinks.h \
    Flv2m4a.h \
    filenameselector.h \
    DownloadInfoBox.h \
    Downloader.h \
    AudioFile.h \
    AudioExtractor.h \
    Mp4tom4a.h \
    MP4AudioFile.h

FORMS    += yload.ui

OTHER_FILES += \
    icon.rc

RC_FILE = icon.rc
