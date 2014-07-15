#include "Compress.h"
#include <QVariant>
#include <QBitArray>

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
        size = qba.mid(0, 3);
        // Traduz c1 e c2 os tamanhos da árvore e do array
        int c1 = size.at(0).unicode();
        int c2 = size.at(1).unicode();
        size.clear();

        // Os transforma em binário e completa com zeros através da função fill
        QString s1 = fill(QString::number(c1, 2));
        QString s2 = fill(QString::number(c2, 2));
        size.append(s1);
        size.append(s2);

        s1.clear();
        s2.clear();

        for (int i = 0; i < 16; ++i) {
            if (i < 3) {
                s1.append(size.at(i));
            } else {
                s2.append(size.at(i));
            }
        }
        //Obtem o tamanho do "lixo" e da árvore
        bool ok;
        int garbageSize = s1.toInt(&ok, 2);
        int treeSize = s2.toInt(&ok, 2);

        // Decodifica o nome do arquivo
        QString namefile;
        for (int i = 3; i < 130; ++i) {
            if (qba[i] == '#') {
                break;
            }
            namefile.append(qba.at(i));
        }

        // Recria a árvore apartir do elemento 131
        QString treeRep;
        treeRep = qba.mid(131, treeSize);

        Tree * tr = new Tree(treeRep);

        //Decodifica o conteúdo(o arquivo em si)
        QString content;
        QString contentAux = qba.mid(treeSize+131, qba.size());
        for (int i = 0; i < contentAux.size(); ++i) {
            QChar simbol = contentAux.at(i);
            int unicode = simbol.unicode();
            content.append(fill(QString::number(unicode, 2)));
        }

        //Através do arquivo content e da árvore reconstruida, faz a descompactação
        QByteArray decoded;
        Node * n = tr->root();
        for (int i = 0; i <= content.size()-garbageSize; ++i) {
            if (n->isLeaf()) {
                decoded.append(n->key());
                n = tr->root();
                --i;
            } else {
                if (content.at(i) == '0') {
                    n = n->left();
                } else if (content.at(i) == '1') {
                    n = n->right();
                } else {
                    qDebug() << "Arquivo Corrompido!";
                }
            }
        }

        f->write(decoded, m_uncompressDirectory + namefile);
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
        // Count the frequence in qba file
        CountOccurrence * co = new CountOccurrence(qba);
        // Take the ordered frequency list
        QList<Occurrence> occur = co->orderByOccurrence();
        // Create a new Huffman Tree with the ordered list
        HuffmanTree * cht = new HuffmanTree(occur);
        // Take the huffman tree
        Tree * tree = cht->createTree();
        tree->createRep();
        //build hash table for qba file
        cht->createHash(tree);

        //adiciona o código de cada elemento de qba no arquivo data
        QString data;
        for (int i = 0; i < qba.size(); ++i) {
            //Fazendo codificação
            QString pathNode = cht->hash()->value(qba.at(i));
            data.append(pathNode);
        }

        //fechando os blocos de 8 na string data
        int garbageSize = 8 - data.size()%8;
        if (garbageSize == 8) {
            garbageSize = 0;
        }
        for (int i = 0; i < garbageSize; ++i) {
            data.append("0");
        }

        // Convertendo para inteiro e armazenando no encoded
        bool ok;
        QByteArray encoded;
        for (int i = 0; i < data.size(); i+=8) {
            QString h = data.mid(i,8);
            encoded.append(QChar(h.toInt(&ok, 2)));
        }

        //calculando tamanho da arvore
        QString binaryGarbageSize = QString::number(garbageSize,2);
        QString binaryTreeSize = QString::number(tree->rep().size(),2);
        int zeros = 16 - (binaryGarbageSize.size() + binaryTreeSize.size());
        for (int i = 0; i < zeros; ++i) {
            binaryTreeSize.prepend(QString::number(0));
        }

        QString toBit = binaryGarbageSize;
        toBit.append(binaryTreeSize);
        //transformando tamanho do lixo e da arvore em inteiros
        int h1 = toBit.mid(0,8).toInt(&ok, 2);
        int h2 = toBit.mid(8,8).toInt(&ok, 2);

        // criando arquivo towrite para ser encodado
        QByteArray toWrite;
        toWrite.clear();
        toWrite.append(QChar(h1));
        toWrite.append(QChar(h2));
        toWrite.append(m_fileName);

        for (int i = m_fileName.size(); i < 128; ++i ) {
            toWrite.append("#");
        }

        toWrite.append(tree->rep());
        toWrite.append(encoded);

        f->write(toWrite, m_path + m_compressedFileName);

        qDebug() << m_fileName << " comprimido";

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


