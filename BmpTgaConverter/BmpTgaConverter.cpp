// PngJpegConverter.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include <fstream>
#include <iostream>
#include <string>
#include <windows.h>
#include <memory>

int main()
{
    const std::string FilePath = "../Convert/testImage.bmp";
    std::ifstream readfile(FilePath, std::ios::in | std::ios::binary);

    if (readfile) {
        std::cout << "success\n";

        // ファイルの大きさを測る
        readfile.seekg(0, std::ios::end);
        long long int size = readfile.tellg();
        readfile.seekg(0);

        //読み込んだデータをchar型に出力する
        std::unique_ptr<char[]> data = std::make_unique<char[]>(size);
        const auto address = data.get();
        readfile.read(data.get(), size);

        int a = 0;
    }
    else {
        std::cout << "ファイルが読み込めませんでした" << "\n";
        std::cout << "Convertフォルダの中にJPEGかPNGのファイルを入れて再度お試しください" << "\n";

    }

}