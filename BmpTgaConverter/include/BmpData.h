////////////////////////////////////////////////////////////
// BMPファイルのデータを読み取り、情報を格納するクラス
////////////////////////////////////////////////////////////
#include <iostream>
#include "../include/BinaryDataReadAPI.h"
#pragma once

class BmpData
{
public:
    BmpData() = default;
    ~BmpData() = default;

    struct Header
    {
        char            bfType1_ = 0;
        char            bfType2_ = 0;
        unsigned long   bfSize_ = 0;
        unsigned short	bfReserved1_ = 0;
        unsigned short	bfReserved2_ = 0;
        unsigned long   bfOffBits_ = 0;
    };

public:
    //// パラメータのエンディアンを反転させる
    //void paramsReverseEndian() {
    //    reverseEndian(bfSize_);
    //    reverseEndian(bfReserved1_);
    //    reverseEndian(bfReserved2_);
    //    reverseEndian(bfOffBits_);
    //}

    // バイナリデータからパラメータを取得する
    // [in] binary バイナリデータの先頭アドレス
    // [in] binarySize バイナリデータサイズ
    // [return]    true:取得成功
    // [return]    false:取得失敗
    bool getParamsFromBinary(char& binary, const long long int& binarySize);

    // ヘッダデータを取得
    Header getHeader() const;

private:
    Header header_{};
};



