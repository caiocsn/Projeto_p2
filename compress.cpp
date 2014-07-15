#include "Compress.h"
#include <QVariant>
#include <QBitArray>
#include <QDebug>

Compress::Compress(QString pathFile) {
    m_path = pathFile;
    m_fileName = pathFile;
    for (int i = pathFile.size() -1; i >= 0; --i) {
        if (pathFile.at(i) == '/') {
            m_path.remove(i, pathFile.size()-i);
            m_path.append("/");
            m_fileName.remove(0,i+1);
            break;
        }
    }
}

Compress::Compress(QString pathFile, QString pathCompressedFile, QString uncompressDirectory) {
    m_path = pathFile;
    m_fileName = pathFile;
    m_compressedFileName = pathCompressedFile;
    m_uncompressDirectory = uncompressDirectory;

    for (int i = pathFile.size() -1; i >= 0; --i) {
        if (pathFile.at(i) == '/') {
            m_path.remove(i, pathFile.size()-i);
            m_path.append("/");
            m_fileName.remove(0,i+1);
            break;
        }
    }

    if (m_uncompressDirectory != ""
            && m_uncompressDirectory.at(m_uncompressDirectory.size()-1) != '/') {
        m_uncompressDirectory.append('/');
    } else if (m_uncompressDirectory == "") {
        m_uncompressDirectory = m_path;
    }
}

Compress::~Compress() {
    m_fileName = "";
    m_path = "";
}

bool Compress::uncompress() {
    File * f = new File(m_path, m_fileName);
    QByteArray qba = f->read();
    if (qba != NULL) {
        QString size;
        size = qba.mid(0, 2);
        qDebug() << "Arquivo a ser descomprimido" << qba << "<<<FIM>>>";

        int garbageSize = size.at(0).unicode();
        int treeSize = size.at(1).unicode();

        qDebug() << "garbageSize" << garbageSize;
        qDebug() << "treeSize" << treeSize;

        QString namefile;
        for (int i = 2; i < 130 ; ++i) {
            if (qba[i] == '#') {
                break;
            }
            namefile.append(qba.at(i));
        }

        qDebug() << "namefile" << namefile;
        QString treeRep;
        treeRep = qba.mid(130, treeSize);
        qDebug() << "treeRep" << treeRep.size();
        Tree * tr = new Tree(treeRep);
        HuffmanTree op;
        op.createHash(tr);
        op.showHash();
        // Arvore OK


        QString content;
        QString contentAux = qba.mid(treeSize+130, qba.size());
        for (int i = 0; i < contentAux.size(); ++i) {
            QChar simbol = contentAux.at(i);
            int unicode = simbol.unicode();
            content.append(fill(QString::number(unicode, 2)));
            qDebug() << content;
        }
        qDebug() << "content:" << content;
        QByteArray decoded;
        Node * n = tr->root();


        for (int i = 0; i <= content.size()-garbageSize; i++) {
            if (n->key()) {
                decoded.append(n->key());
                n = tr->root();
                i--;
            }
            else {
                if (content.at(i) == '0') {
                    n = n->left();
                } else if (content.at(i) == '1') {
                    n = n->right();
                } else {
                    qDebug() << "Arquivo Corrompido!";
                }
            }
        }
        qDebug() << decoded;
        f->write(decoded, m_uncompressDirectory + "out.txt");
        qDebug() << namefile + " descompactado";

        return true;
    } else {
        qDebug() << "Arquivo .huff não encontrado";
        return false;
    }
}

bool Compress::compress() {
    File * f = new File(m_path, m_fileName);
    QByteArray qba = f->read();
    if (qba != NULL) {
        CountOccurrence * co = new CountOccurrence(qba);
        QList<Occurrence> occur = co->orderByOccurrence();
        HuffmanTree * cht = new HuffmanTree(occur);
        Tree * tree = cht->createTree();
        tree->createRep();
        cht->createHash(tree);
        QString data;

        for (int i = 0; i < qba.size(); ++i) {
            QString pathNode = cht->hash()->value(qba.at(i));
            data.append(pathNode);
        }

        int garbageSize = 8 - data.size()%8;
        if (garbageSize == 8) {
            garbageSize = 0;
        }
        for (int i = 0; i < garbageSize; ++i) {
            data.append("0");
        }
        QBitArray toByte;
        toByte.resize(data.size());
        for (int i = 0; i < data.size(); ++i) {
            if (data.at(i) == '0') {
                toByte[i] = true;
            }
            else {
                toByte[i] = false;
            }
        }
        qDebug() << "data/size" << data << "/" << data.size();
        // ate aqui ta certinho
        bool ok;
        QByteArray encoded;
        QString c;
        for (int i = 0; i < data.size(); i+=8) {
            QString h = data.mid(i,8);
            qDebug() << "h :" << h;
            encoded.append(QChar(h.toInt(&ok, 2)));
            qDebug() << "h2:" << QChar(h.toInt(&ok, 2)) << "//";
            c.append(h);
        }
        qDebug() << "encoded" << encoded << "//";

        QByteArray toWrite;
        toWrite.clear();
        toWrite.append(QChar(garbageSize));
        toWrite.append(QChar(tree->rep().size()));
        toWrite.append(m_fileName);

        for (int i = m_fileName.size(); i < 128; ++i ) {
            toWrite.append("#");
        }

        toWrite.append(tree->rep());
        toWrite.append(encoded);
        toWrite.append('#');
        f->write(toWrite, m_path + m_compressedFileName);

        qDebug() << "Arquivo comprimido";

        return true;
    } else {
        qDebug() << "Arquivo não encontrado";

        return false;
    }
}

QString Compress::fill(QString str)
{
    int zeros = 8 - str.size()%8;
    if (zeros == 8) {
        zeros = 0;
    }
    for (int i = 0; i < zeros; ++i) {
        str.prepend('0');
    }
    return str;
}


