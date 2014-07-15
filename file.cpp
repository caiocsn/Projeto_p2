#include <QDataStream>
#include <QBitArray>
#include <QTextStream>
#include <QDebug>
#include <iostream>
#include <vector>
#include <File.h>

File::File() {
    m_path = "";
    m_filename = "";
}

File::File(QString path, QString filename) {
    m_path = path;
    m_filename = filename;
}

File::~File() {
    m_path.clear();
    m_filename.clear();
}


QByteArray File::read() {
    QFile file(m_path + m_filename);
    file.open(QIODevice::ReadOnly);

    QTextStream in(&file);
    QString string = in.readAll();
    QByteArray byteArray;
    byteArray.append(string);

    return byteArray;
}


void File::write(QByteArray byteArray, QString filename) {
    QFile file(filename);
    file.open(QIODevice::ReadWrite);
    file.write(byteArray);
    file.close();
}
