#include<frequencycount.h>
#include<QDebug>

FrequencyCount::FrequencyCount(QByteArray file){
    this->setFile(file);
    QList<Frequency> freq = QList<Frequency>();
    this->setFrequency(freq);
}

FrequencyCount::~FrequencyCount(){}

void FrequencyCount::count() {
    for (int i = 0; i < this->getFile().size(); i++) {
        bool exist = false;
        for (int j = 0; j < this->getFrequency().size(); j++) {
            if(this->_file[i] == this->_frequency[j].getValue()) {
                exist = true;
            }
        }

        if (!exist) {
            unsigned char value = this->_file.at(i);
            Frequency freq = Frequency(value);
            int frequency = this->_file.count(value);
            freq.setFreq(frequency);
            this->_frequency.append(freq);
        }
    }
}

QList<Frequency> FrequencyCount::order(){
    this->count();

    QList<Frequency> ordered;

    for(int i = 0; i < this->_frequency.size(); i++) {
        for (int j = i; j > 0 && this->_frequency[j].getFreq() < this->_frequency[j-1].getFreq(); j--) {
            Frequency aux = this->_frequency[j];
            this->_frequency[j] = this->_frequency[j+1];
            this->_frequency[j+1] = aux;
        }
    }
    for(int i = 0; i < this->_frequency.size(); i++) {
        ordered.append(this->_frequency[i]);
    }

    return ordered;
}

void FrequencyCount::setFile(QByteArray file){
    this->_file = file;
}

void FrequencyCount::setFrequency(QList<Frequency> freq){
    this->_frequency = freq;
}


QByteArray FrequencyCount::getFile(){
    return this->_file;
}

QList<Frequency> FrequencyCount::getFrequency(){
    return this->_frequency;
}

void FrequencyCount::print() {
    for (int i = 0; i < this->getFrequency().size(); i++) {
        qDebug() << "Value: " << this->_frequency[i].getValue();
        qDebug() << "   Frequency: " << this->_frequency[i].getFreq();
    }
}
