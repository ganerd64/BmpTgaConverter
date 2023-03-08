// PngJpegConverter.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include <fstream>
#include <iostream>
#include <string>
#include <windows.h>

int main()
{
    const std::string FilePath = "../Convert/testImage.png";
    std::ifstream readfile(FilePath, std::ios::binary);

    if (readfile) {
        std::cout << "success\n";

        readfile.seekg(0, std::ios::end);
        long long int size = readfile.tellg();
        readfile.seekg(0);

        //読み込んだデータをchar型に出力する
        char* data = new char[size];
        readfile.read(data, size);

        int a = 0;
    }
    else {
        std::cout << "ファイルが読み込めませんでした" << "\n";
        std::cout << "Convertフォルダの中にJPEGかPNGのファイルを入れて再度お試しください" << "\n";

    }

}