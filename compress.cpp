#include <compress.h>
#include <QVariant>
#include <QBitArray>
#include <QDebug>

Compress::Compress(QString pathFile) {
    this->_path = pathFile;
    this->_fileName = pathFile;
    // Fazendo tratamento de String, separação de path e nome do arquivo
    for (int i = pathFile.size() -1; i >= 0; --i) {
        if (pathFile.at(i) == '/') {
            this->_path.remove(i, pathFile.size()-i);
            this->_path.append("/");
            this->_fileName.remove(0,i+1);
            break;
        }
    }
}

Compress::Compress(QString pathFile, QString pathCompressedFile, QString uncompressDirectory) {
    this->_path = pathFile;
    this->_fileName = pathFile;
    this->_compressedFileName = pathCompressedFile;
    this->_uncompressDirectory = uncompressDirectory;

    for(int i = pathFile.size() -1; i >= 0; --i) {
        if (pathFile.at(i) == '/') {
            this->_path.remove(i, pathFile.size()-i);
            this->_path.append("/");
            this->_fileName.remove(0,i+1);
            break;
        }
    }

    if(this->_uncompressDirectory != ""
            && this->_uncompressDirectory.at(this->_uncompressDirectory.size()-1) != '/') {
        this->_uncompressDirectory.append('/');
    }
    else if(this->_uncompressDirectory == "") {
        this->_uncompressDirectory = this->_path;
    }
}

Compress::~Compress() {
    this->_fileName = "";
    this->_path = "";
}

bool Compress::uncompress() {
    File * f = new File(this->_path, this->_fileName);
    QByteArray qba = f->read();
    if (qba != NULL) {
        QString size;
        size = qba.mid(0, 3);
        // Traduz c1 e c2 os tamanhos da árvore e do array (usando unicode)
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

        for(int i = 0; i < 16; ++i) {
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

        f->write(decoded, this->_uncompressDirectory + this->_fileName);
        qDebug() << namefile + " descompactado com sucesso";

        return true;
    } else {
        qDebug() << "Erro: arquivo a ser descompactado não encontrado";
        return false;
    }
}

bool Compress::compress() {
    File * f = new File(this->_path, this->_fileName);
    QByteArray qba = f->read();
    if (qba != NULL) {
        // Contando a frequencia dos caracteres no arquivo
        FrequencyCount * freq = new FrequencyCount(qba);
        // Ordenando a lista de caracteres pela frequencia dos mesmos
        QList<Frequency> ordered = freq->order();
        // Criando a árvore de Huffman
        Huffman * huff = new Huffman(ordered);
        Tree * tree = huff->buildTree();
        tree->createRep();
        // Criando a tabela para a árvore huff
        huff->buildHash(tree);

        //adiciona o código de cada elemento de qba no arquivo data
        QString data;
        for (int i = 0; i < qba.size(); ++i) {
            //Fazendo codificação
            QString pathNode = huff->getHash()->value(qba.at(i));
            data.append(pathNode);
        }

        //fechando os blocos de 8 na string data
        //calculando o tamanho do lixo (complemento)
        int garbageSize = 8 - data.size()%8;
        if (garbageSize == 8) {
            garbageSize = 0;
        }
        for (int i = 0; i < garbageSize; ++i) {
            data.append("0");
        }

        // Convertendo o conteúdo para inteiro e armazenando no encoded
        bool ok;
        QByteArray encoded;
        for (int i = 0; i < data.size(); i+=8) {
            QString h = data.mid(i,8);
            encoded.append(QChar(h.toInt(&ok, 2)));
        }

        // Calculando tamanho da arvore e do lixo e os transforma em binário
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
        toWrite.append(this->_fileName);

        // Adicionando '#' após o nome do arquivo
        for (int i = this->_fileName.size(); i < 128; ++i ) {
            toWrite.append("#");
        }

        //Adicionando a árvore e o conteúdo
        toWrite.append(tree->rep());
        toWrite.append(encoded);

        f->write(toWrite, this->_path + this->_compressedFileName);

        qDebug() << "Arquivo comprimido com sucesso";

        return true;
    } else {
        qDebug() << "Erro: Arquivo não encontrado";

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


