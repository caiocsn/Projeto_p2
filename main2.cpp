#include <compress.h>
#include <readme.h>

int main() {
    readMe();

    QString fileName = "asasa";
    QString compressedFileName = "aaaa";
    QString directory = "/";

    Compress a = Compress("C:/Qt/teste.txt","out","C:/Qt/");
    a.compress();
    a.~Compress();

}

