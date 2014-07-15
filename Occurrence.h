#ifndef OCCURRENCE_H
#define OCCURRENCE_H

class Occurrence {
public:
    Occurrence (unsigned char qCharCode);
    Occurrence ();
    ~Occurrence ();
    unsigned char character();
    int count();
    void set_char(unsigned char ch);
    void set_count(int count);
private:
    unsigned char m_char;
    int m_count;
};

#endif // OCCURRENCE_H
