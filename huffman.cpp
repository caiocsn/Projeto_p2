#include "huffman.h"
#include "QDebug"

Huffman::Huffman(QList<Frequency> freq){
    this->_hash = new QHash<unsigned char, QString>();
    this->_frequency = new Queue();
    for (int i = 0; i < freq.size(); i++) {
        Node * a = new Node(freq[i].getValue(), freq[i].getFreq());
        this->_frequency->enqueue(a);
    }
}

Huffman::Huffman(){
    this->_hash = new QHash<unsigned char, QString>();
    this->_frequency = new Queue();
}

Huffman::~Huffman(){
    this->_frequency = 0;
    delete this->_tree;
}

Queue * Huffman::getFrequency(){
    return this->_frequency;
}

void Huffman::setFrequency(Queue *frequency){
    this->_frequency = frequency;
}

Tree * Huffman::getTree(){
    return this->_tree;
}

void Huffman::setTree(Tree * a){
    this->_tree = a;
}

Tree * Huffman::buildTree(){
    Tree * t = new Tree();
    Node * root;
    while(this->_frequency->length() > 1){
        Node * a = this->_frequency->frontValue();
        this->_frequency->dequeue();
        Node * b = this->_frequency->frontValue();
        this->_frequency->dequeue();
        Node * c = new Node();
        int cFrequency = a->frequency() + b->frequency();
        c->setFrequency(cFrequency);
        c->setRight(a);
        c->setLeft(b);
        this->_frequency->enqueue(c);
        root = c;
    }
    t->setRoot(root);
    int height = t->setHeight(root);
    root->setHeight(height);
    return t;
}

void Huffman::setHash(QHash<unsigned char, QString> *hash){
    this->_hash = hash;
}

QHash<unsigned char, QString> * Huffman::getHash(){
    return this->_hash;
}

void Huffman::buildHash(Tree * a){
    this->addNodeToHash(a->root(),"");
}

void Huffman::addNodeToHash(Node *node, QString path){
    if(node->isLeaf()){
        addNodeToHash(node->key(), path);
    }
    else{
        if(node->left()){
            QString path = node->path();
            path.append("0");
            node->left()->setPath(path);
            addNodeToHash(node->left(), node->left()->path());
        }

        if(node->right()){
            QString path = node->path();
            path.append("1");
            node->right()->setPath(path);
            addNodeToHash(node->right(), node->right()->path());
        }
    }
}

void Huffman::addNodeToHash(unsigned char key, QString path){
    this->_hash->insert(key, path);
}

void Huffman::printHash(){
    QList<unsigned char> keys = this->_hash->keys();
    for (int i = 0; i < keys.size(); i++) {
        qDebug() << keys[i];
        qDebug() << this->_hash->take(keys[i]);
    }
}
