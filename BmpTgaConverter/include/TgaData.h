////////////////////////////////////////////////////////////
// BMPファイルのデータを読み取り、情報を格納するクラス
////////////////////////////////////////////////////////////
#include <iostream>
#include <vector> 
#include "../include/ImageData.h"
#pragma once

class TgaData : ImageData
{
public:
    TgaData() = default;
    ~TgaData() = default;

    struct Header
    {
        char                idFieldLength_  = 0;
        char                colorMapType_   = 0;
        char                imageType_      = 0;
        unsigned short      colorMapIndex_  = 0;
        unsigned short      colorMapLength_ = 0;
        char                colorMapSize_   = 0;
        unsigned short      imageOriginX_   = 0;
        unsigned short      imageOriginY_   = 0;
        unsigned short      imageWitdth_    = 0;
        unsigned short      imageHeight_    = 0;
        char                bitPerPixel_    = 0;
        char                discripter_     = 0;
    };
    struct Footer
    {
        long    filePosition_ = 0;
        long    developerDirectoryFilePosition_ = 0;
        char    truevisionTargaCharacter_[17] = "TRUEVISION-TARGA";
        char    eof_ = 0;
    };

public:
    // バイナリデータからパラメータを取得する
    // [in] binary バイナリデータの先頭アドレス
    // [in] binarySize バイナリデータサイズ
    // [return]    true:取得成功
    // [return]    false:取得失敗
    bool getParamsFromBinary(char& binary, const long long int& binarySize);

    // ヘッダデータを取得
    Header getHeader() const;

    // ヘッダデータを設定
    //void setHeader(const Header& header);

    //void setColorDatas(const std::vector<Color>& colorDatas);

private:

    // 画像の色データの上下を反転させる
    // [in] width   画像横サイズ
    // [in] height  画像縦サイズ
    // [in/out] colorDatas バイナリデータの先頭アドレス
    void flipVerticalColorDatas(int width, int height, std::vector<Color>& colorDatas);

private:
    Header header_{}; 
    Footer footer_{};
};



