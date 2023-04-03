////////////////////////////////////////////////////////////
// BMPファイルのデータを読み取り、情報を格納するクラス
////////////////////////////////////////////////////////////
#include <iostream>
#include <vector> 
#include "../include/ImageData.h"
#pragma once

class BmpData : public ImageData
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
    struct Information
    {
        unsigned long   biSize_ = 0;
        long            biWidth_ = 0;
        long            biHeight_ = 0;
        unsigned short  biPlanes_ = 0;
        unsigned short  biBitCount_ = 0;
        unsigned long   biCompression_ = 0;
        unsigned long   biSizeImage_ = 0;
        long            biXPixPerMeter_ = 0;
        long            biYPixPerMeter_ = 0;
        unsigned long   biClrUsed_ = 0;
        unsigned long   biCirImportant_ = 0;
    };
public:

    // バイナリデータからパラメータを取得する
    // [in] binary バイナリデータの先頭アドレス
    // [in] binarySize バイナリデータサイズ
    // [return]    true:取得成功
    // [return]    false:取得失敗
    bool getParamsFromBinary(char& binary, const long long int& binarySize);

    // ヘッダデータを取得
    const Header& getHeader() const;

    // 情報データを取得
    const Information& getInformation() const;

    // 色データを取得
    const std::vector<Color>& getColorDatas() const;

private:
    Header header_{};
    Information info_{};
    std::vector<Color> colorDatas_;
};



