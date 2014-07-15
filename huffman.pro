#-------------------------------------------------
#
# Project created by QtCreator 2013-12-19T19:12:47
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = huffman
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    Node.cpp \
    Tree.cpp \
    Occurrence.cpp \
    CountOccurence.cpp \
    File.cpp \
    CreateHuffmanTree.cpp \
    Queue.cpp \
    Compress.cpp

HEADERS += \
    Node.h \
    Tree.h \
    File.h \
    CountOccurrence.h \
    Occurrence.h \
    File.h \
    Queue.h \
    Compress.h \
    HuffmanTree.h
