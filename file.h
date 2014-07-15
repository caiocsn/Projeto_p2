#ifndef FILE_H
#include <QString>
#include <QFile>
#include <QByteArray>
#define FILE_H

class File {
public:
    // Constructor
    File();
    File(QString path, QString filename);
    // Destructor
    ~File();
    // Reads the file and return a Qbytearray
    QByteArray read();
    // Convert a Qbytearray into file
    void write(QByteArray byteArray, QString file);
private:
    // File's path
    QString _path;
    // File's name
    QString _name;
};

#endif // FILE_H
