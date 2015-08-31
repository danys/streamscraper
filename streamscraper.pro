#-------------------------------------------------
#
# Project created by QtCreator 2015-08-23T10:46:54
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = streamscraper
TEMPLATE = app


SOURCES += main.cpp \
    AudioExtractor.cpp \
    AudioFile.cpp \
    Downloader.cpp \
    DownloadInfoBox.cpp \
    filenameselector.cpp \
    Flv2m4a.cpp \
    GetLinks.cpp \
    LineEditButton.cpp \
    MP4AudioFile.cpp \
    Mp4tom4a.cpp \
    yload.cpp

HEADERS  += \
    AudioExtractor.h \
    AudioFile.h \
    Downloader.h \
    DownloadInfoBox.h \
    filenameselector.h \
    Flv2m4a.h \
    GetLinks.h \
    LineEditButton.h \
    MP4AudioFile.h \
    Mp4tom4a.h \
    yload.h

FORMS    += \
    yload.ui

DISTFILES += \
    streamscraper.ico \
    icon.rc
RC_FILE = icon.rc
