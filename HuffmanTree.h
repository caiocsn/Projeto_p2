#ifndef HUFFMANTREE_H
#define HUFFMANTREE_H

#include <Node.h>
#include <Tree.h>
#include <Queue.h>
#include <QList>
#include <Occurrence.h>
#include <QHash>

class HuffmanTree
{
public:
    HuffmanTree();
    HuffmanTree(QList<Occurrence> oc);
    ~HuffmanTree();
    Queue * occurrence();
    void setOccurence(Queue * occur);
    Tree * tree();
    void setTree(Tree * t);
    Tree * createTree();
    void setHash(QHash<unsigned char, QString> * hash);
    QHash<unsigned char, QString> * hash();
    void createHash(Tree * t);
    void showHash();

private:
    void addNodeToHash(Node * node, QString path);
    void addNodeToHash(unsigned char key, QString path);

private:
    Queue * m_occurrences;
    Tree * m_tree;
    QHash<unsigned char, QString> * m_hash;
};

#endif // HUFFMANTREE_H
