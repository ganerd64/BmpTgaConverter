// BmpTgaConverter.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include <fstream>
#include <iostream>
#include <string>
#include <windows.h>
#include <memory>
#include <cassert>

// 渡されたバイナリデータからTのサイズ分イテレータを進めてTの値を返す
template<class T>
char* getParamFromBinary(T& val, char& binary)
{
    // 値取得用の変数
    char* valItr = reinterpret_cast<char*>(&val);
    char* binaryItr = reinterpret_cast<char*>(&binary);
    for (int i = 0; i < sizeof(T); ++i) {
        *valItr = *binaryItr;
        binaryItr++;
        valItr++;
    }
    return binaryItr;
}

// エンディアンの反転処理
template<class T>
T reverseEndian(T& val)
{
    // スカラー型でなければ処理しない
    static_assert(std::is_scalar<T>::value, L"reverseEndian：渡されたTの型がスカラーではありません");

    // 反転処理
    char* first = reinterpret_cast<char*>(&val);
    char* last = first + sizeof(T);
    std::reverse(first, last);
    return val;
}

constexpr int BitMapHeaderSize = 14;

class BitMapFileHeader {
public:
    void paramsReverseEndian() {
        reverseEndian(bfSize_);
        reverseEndian(bfReserved1_);
        reverseEndian(bfReserved2_);
        reverseEndian(bfOffBits_);
    }
    void getParamsFromBinary(char& binary, const long long int& binarySize) {
        if (sizeof(BitMapFileHeader) < binarySize) {
            // todo:条件式をstatic_assertに入れたい
            static_assert(true, L"読み込み処理でイテレータがファイルサイズを超過");
        }

        char* binaryItr = reinterpret_cast<char*>(&binary);
        binaryItr = getParamFromBinary(bfType1_, *binaryItr);
        binaryItr = getParamFromBinary(bfType2_, *binaryItr);
        binaryItr = getParamFromBinary(bfSize_, *binaryItr);
        binaryItr = getParamFromBinary(bfReserved1_, *binaryItr);
        binaryItr = getParamFromBinary(bfReserved2_, *binaryItr);
        binaryItr = getParamFromBinary(bfOffBits_, *binaryItr);
    }

private:
    char            bfType1_ = 0;
    char            bfType2_ = 0;
    unsigned long   bfSize_ = 1;
    unsigned short	bfReserved1_ = 2;
    unsigned short	bfReserved2_ = 3;
    unsigned long   bfOffBits_ = 4;

};


int main()
{
    // ファイルをバイナリで読み込み
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
        readfile.read(data.get(), size);

        // 読み込んだデータを構造体に出力
        BitMapFileHeader header;
        char* itr = reinterpret_cast<char*>(data.get());
        header.getParamsFromBinary(*itr, size);
        
    }
    else {
        std::cout << "ファイルが読み込めませんでした" << "\n";
        std::cout << "Convertフォルダの中にBMPかTGAのファイルを入れて再度お試しください" << "\n";

    }

}