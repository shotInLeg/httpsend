TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp

HEADERS += \
    ANetworkTransfer/AHttp/ahttpdata.h \
    ANetworkTransfer/AHttp/ahttpparse.h \
    ANetworkTransfer/AHttp/ahttptransfer.h
