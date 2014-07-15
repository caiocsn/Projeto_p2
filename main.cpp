#include <Compress.h>
int main() {

        QString fileName = "";
        QString compressedFileName = "";
        QString directory = "";

                int cmd = 2;
                Compress c = Compress("C:/Qt/b.huff","b.huff","C:/Qt/");
                if(cmd == 1)
                    c.compress();
                else
                    c.uncompress();


}
