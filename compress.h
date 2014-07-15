#ifndef COMPRESS_H
#include <Tree.h>
#include <File.h>
#include <QDebug>
#include <iostream>
#include <QByteArray>
#include <QFile>
#include <frequencycount.h>
#include <Huffman.h>
#include <QString>
#define COMPRESS_H

class Compress
{
private:
    QString _path;
    QString _fileName;
    QString _compressedFileName;
    QString _uncompressDirectory;
public:
    // Constructors of compression
    Compress(QString pathFile);
    Compress(QString pathFile, QString pathCompressedFile, QString uncompressDirectory);
    // Destructor
    ~Compress();
    // Uncompression method
    bool uncompress();
    // Compression method
    bool compress();
private:
    // Fill a string with a certain number of zeros
    QString fill(QString str);
};

#endif // COMPRESS_H
