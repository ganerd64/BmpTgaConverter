#include "../include/FileLoader.h"
#include <fstream>

////////////////////////////////////////////////////////////
// ファイルの読み込みを行う
////////////////////////////////////////////////////////////
bool FileLoader::fileLoadBinary(std::string& fileName)
{
    std::ifstream readfile(fileName, std::ios::in | std::ios::binary);

    if (readfile) {
        // ファイルの大きさを測る
        readfile.seekg(0, std::ios::end);
        size_ = readfile.tellg();
        readfile.seekg(0);

        //読み込んだデータをchar型に出力する
        data_ = std::make_unique<char[]>(size_);
        readfile.read(data_.get(), size_);
        return true;
    }
    return false;
}

////////////////////////////////////////////////////////////
// 読み取ったファイルデータの先頭アドレスを取得
////////////////////////////////////////////////////////////
char* FileLoader::getLoadDataTopAddress()  const
{
    return data_.get();
}

////////////////////////////////////////////////////////////
// 読み取ったファイルデータのサイズを取得
////////////////////////////////////////////////////////////
long long int FileLoader::getLeadDataSize()  const
{
    return size_;
}
