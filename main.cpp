#include <Compress.h>

void manHuffman(){
    qDebug()<< "---- COMPRESSOR HUFFMAN - MANUAL DE COMANDOS GERAIS ----";
    qDebug()<< "";
    qDebug()<< "";
    qDebug()<< "- OPÇÕES:";
    qDebug()<< "";
    qDebug()<<"-> Compacta o arquivo para um arquivo .huff com o nome indicado: ";
    qDebug()<< "    huffman -c arquivoOriginal.x -o nomeDoArquivoCompactado.huff";
    qDebug()<< "";
    qDebug()<<"-> Compacta o arquivo para o arquivo out.huff: ";
    qDebug()<<"    huffman -c arquivoOriginal.x";
    qDebug()<< "";
    qDebug()<<"-> Descompacta um arquivo .huff na pasta local: ";
    qDebug()<<"    huffman arquivoCompactado.huff";
    qDebug()<< "";
    qDebug()<<"-> Descompacta um arquivo .huff no diretório indicado: ";
    qDebug()<<"    huffman arquivoCompactado.huff -d diretorio";
    qDebug()<< "";
    qDebug()<< "";
    qDebug()<< "";
    qDebug()<< "---- COMPRESSOR HUFFMAN - MANUAL DE COMANDOS GERAIS ----";


}

int main(int argc, char *argv[]) {
    if (argc <= 5 && argc >= 2){

        QString fileName = "";
        QString compressedFileName = "";
        QString directory = "";


        if (argc == 5) {
            if (argv[1][0] == '-' && argv[1][1] == 'c' && argv[3][0] == '-' && argv && argv[3][1] == 'o'){
                fileName = argv[2];
                compressedFileName = argv[4];
                Compress c = Compress(fileName, compressedFileName, directory);
                c.compress();
                c.~Compress();
            } else {
                manHuffman();
            }
        } else if (argc == 4){
            if (argv[2][0] == '-' && argv[2][1] == 'd'){
                fileName = argv[1];
                directory = argv[3];
                Compress c = Compress(fileName, compressedFileName, directory);
                c.uncompress();
                c.~Compress();
            } else {
                manHuffman();
            }
        } else if (argc == 3){
            if (argv[1][0] == '-' && argv[1][1] == 'c'){
                fileName = argv[2];
                compressedFileName.append("out.huff");
                Compress c = Compress(fileName, compressedFileName, directory);
                c.compress();
                c.~Compress();
            } else {
                manHuffman();
            }
        } else if (argc == 2){
            fileName = argv[1];
            Compress c = Compress(fileName, compressedFileName, directory);
            c.uncompress();
            c.~Compress();
        } else {
            manHuffman();
        }


    } else {
        manHuffman();
    }

}

