#ifndef HUFFMAN_H
#include <Node.h>
#include <Tree.h>
#include <Queue.h>
#include <QList>
#include <frequency.h>
#include <QHash>
#define HUFFMAN_H

class Huffman
{
private:
    // Queue that stores the characters frequency
    Queue * _frequency;
    // The huffman Tree
    Tree * _tree;
    // The hash with tree keys and paths
    QHash<unsigned char, QString> * _hash;

public:
    // Constructors
    Huffman();
    Huffman(QList<Frequency> freq);
    // Destructor
    ~Huffman();
    // Frequency getter
    Queue * getFrequency();
    // Frequency setter
    void setFrequency(Queue * frequency);
    // Tree getter
    Tree * getTree();
    // Tree setter
    void setTree(Tree * a);
    // Huffman Tree builder
    Tree * buildTree();
    // Hash setter
    void setHash(QHash<unsigned char, QString> * hash);
    // Hash getter
    QHash<unsigned char, QString> * getHash();
    // Build a huffman's tree hash table
    void buildHash(Tree * a);
    // Print the hash
    void printHash();

private:
    // Search for leafs and add set their paths
    void addNodeToHash(Node * node, QString path);
    // Add new nodes to hash
    void addNodeToHash(unsigned char key, QString path);
};

#endif // HUFFMAN_H
