#include <QDataStream>
#include <file.h>
#include <QBitArray>
#include <QDebug>
#include <QTextStream>
#include <iostream>
#include <vector>

File::File(){
    this->_name = "";
    this->_path = "";
}

File::File(QString path, QString filename) {
    this->_name = filename;
    this->_path = path;
}

File::~File() {
    this->_path.clear();
    this->_name.clear();
}

QByteArray File::read() {
    QFile file(this->_path + this->_name);
    file.open(QIODevice::ReadOnly);

    QTextStream aux(&file);
    QString toString = aux.readAll();
    QByteArray qba;
    qba.append(toString);

    return qba;
}


void File::write(QByteArray byteArray, QString filename) {
    QFile file(filename);
    file.open(QIODevice::ReadWrite);
    file.write(byteArray);
    file.close();
}
