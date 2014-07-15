#include <frequency.h>

Frequency::Frequency(unsigned char charCode) {
    this->setValue(charCode);
    this->setFreq(0);
}

Frequency::Frequency() {}

Frequency::~Frequency() {}

unsigned char Frequency::getFreq(){
    return this->value;
}

int Frequency::getValue(){
    return this->value;
}

void Frequency::setFreq(int a){
    this->freq = a;
}

void Frequency::setValue(unsigned char a){
    this->value = a;
}
