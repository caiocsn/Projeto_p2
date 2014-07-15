#ifndef FREQUENCYCOUNT_H
#include <QString>
#include <QVariant>
#include <QList>
#include <frequency.h>
#define FREQUENCYCOUNT_H

class FrequencyCount{
public:
    // Constructors
    FrequencyCount(QByteArray file);
    // Destructor
    ~FrequencyCount ();
    //Make the count of frequency for each value in the file
    void count ();
    // Order the list by the frequency count
    QList<Frequency> order ();
    // File setter
    void setFile (QByteArray file);
    // Frequency setter
    void setFrequency (QList<Frequency> freq);
    // File getter
    QByteArray getFile ();
    // Frequency getter
    QList<Frequency> getFrequency ();
    // Print the Freqnecy List
    void print ();

private:
    QByteArray _file;
    QList<Frequency> _frequency;
};

#endif // FREQUENCYCOUNT_H
