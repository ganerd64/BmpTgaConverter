////////////////////////////////////////////////////////////
// ファイルの読み込みを行う専用クラス
////////////////////////////////////////////////////////////
#include <iostream>
#pragma once

class FileLoader
{
public:
    FileLoader() = default;
    ~FileLoader() = default;

    // ファイルの読み込みを行う
    // fileName ファイル名
    // result   true:読み取り成功
    // result   false:読み取り失敗
    bool fileLoadBinary(std::string& fileName);

    // 読み取ったファイルデータの先頭アドレスを取得
    char* getLoadDataTopAddress() const;

    // 読み取ったファイルデータのサイズを取得
    long long int getLeadDataSize() const;

private:
    std::unique_ptr<char[]> data_ = nullptr;
    long long int size_ = 0;
};





