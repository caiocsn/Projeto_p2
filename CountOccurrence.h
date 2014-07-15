#ifndef COUNTOCCURRENCE_H
#include <QString>
#include <QVariant>
#include <QList>
#include <Occurrence.h>
#define COUNTOCCURRENCE_H

class CountOccurrence {
public:
    CountOccurrence (QByteArray file);
    ~CountOccurrence ();
    void count ();
    QList<Occurrence> orderByOccurrence ();
    void set_file (QByteArray file);
    void set_occurrence (QList<Occurrence> Occurrence);
    QByteArray file ();
    QList<Occurrence> occurrence ();
    void print ();

private:
    QByteArray m_file;
    QList<Occurrence> m_occurrence;
};

#endif // COUNTOCCURRENCE_H

