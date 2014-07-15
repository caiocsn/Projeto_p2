#ifndef COMPRESS_H
#define COMPRESS_H
#include <Tree.h>
#include <File.h>
#include <QDebug>
#include <iostream>
#include <QByteArray>
#include <QFile>
#include <CountOccurrence.h>
#include <HuffmanTree.h>
#include <QString>

class Compress
{
public:
    Compress(QString pathFile);
    Compress(QString pathFile, QString pathCompressedFile, QString uncompressDirectory);
    ~Compress();
    bool uncompress();
    bool compress();
private:
    QString fill(QString str);
private:
    QString m_path;
    QString m_fileName;
    QString m_compressedFileName;
    QString m_uncompressDirectory;

};

#endif // COMPRESS_H
