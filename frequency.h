#ifndef FREQUENCY_H
#define FREQUENCY_H

class Frequency{
    public:
        // Constructors
        Frequency(unsigned char qCharCode);
        Frequency();
        // Destructor
        ~Frequency();
        // Freq getter
        unsigned char getFreq();
        // Value getter
        int getValue();
        // Value setter
        void setValue(unsigned char a);
        // Freq setter
        void setFreq(int a);
    private:
        // The value of the object
        unsigned char value;
        // The frequency it appears
        int freq;
};

#endif // FREQUENCY_H
