#include <QDebug>
#include "HuffmanTree.h"

HuffmanTree::HuffmanTree(QList<Occurrence> oc) {
    m_hash = new QHash<unsigned char, QString>();
    m_occurrences = new Queue();
    for (int i = 0; i < oc.size(); ++i) {
        Node * n = new Node(oc[i].character(), oc[i].count());
        m_occurrences->enqueue(n);
    }
}

HuffmanTree::HuffmanTree() {
    m_hash = new QHash<unsigned char, QString>();
    m_occurrences = new Queue();
}

HuffmanTree::~HuffmanTree() {
    m_occurrences = 0;
    delete m_tree;
}

Queue * HuffmanTree::occurrence() {
    return m_occurrences;
}

void HuffmanTree::setOccurence(Queue * occur) {
    m_occurrences = occur;
}

Tree * HuffmanTree::tree() {
    return m_tree;
}

void HuffmanTree::setTree(Tree * t) {
    m_tree = t;
}

Tree * HuffmanTree::createTree() {
    Tree * t = new Tree();
    Node * root;
    while (m_occurrences->length() > 1) {
        Node * n1 = m_occurrences->frontValue();
        m_occurrences->dequeue();
        Node * n2 = m_occurrences->frontValue();
        m_occurrences->dequeue();
        Node * n3 = new Node();
        int n3freq = n1->frequency() + n2->frequency();
        n3->setFrequency(n3freq);
        n3->setRight(n1);
        n3->setLeft(n2);
        m_occurrences->enqueue(n3);
        root = n3;
    }
    t->setRoot(root);
    int h = t->setHeight(root);
    root->setHeight(h);
    return t;
}


void HuffmanTree::setHash(QHash<unsigned char, QString> * hash) {
    m_hash = hash;
}


QHash<unsigned char, QString> * HuffmanTree::hash() {
    return m_hash;
}

void HuffmanTree::createHash(Tree *t) {

    addNodeToHash(t->root(), "");

}

void HuffmanTree::addNodeToHash(Node *node, QString path) {
    if (node->isLeaf()) {
        addNodeToHash(node->key(), path);
    } else {
        if (node->left()) {
            QString path = node->path();
            path.append("0");
            node->left()->setPath(path);
            addNodeToHash(node->left(), node->left()->path());
        }

        if (node->right()) {
            QString path = node->path();
            path.append("1");
            node->right()->setPath(path);
            addNodeToHash(node->right(), node->right()->path());
        }
    }
}

void HuffmanTree::addNodeToHash(unsigned char key, QString path) {
    m_hash->insert(key, path);
}


void HuffmanTree::showHash() {
    QList<unsigned char> keys = m_hash->keys();
    for (int i = 0; i < keys.size(); ++i) {
        qDebug() << keys[i];
        qDebug() << m_hash->take(keys[i]);
    }
}
