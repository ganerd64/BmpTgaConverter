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

    enum ImageType
    {
        None = 0,
        IndexColor,
        FullColor,
        Gray,
        IndexColorRLE,
        FullColorRLE,
        GrayRLE,
    };
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
        char    trueVisionTargaCharacter_[17] = "TRUEVISION-TARGA";
        char    eof_ = 0;
    };

public:
    // バイナリデータからパラメータを取得する
    // [in] binary バイナリデータの先頭アドレス
    // [in] binarySize バイナリデータサイズ
    // [return]    true:取得成功
    // [return]    false:取得失敗
    bool getParamsFromBinary(char& binary, const long long int& binarySize);

    // 出力時に必要なデータを設定
    // [in] width 画像データの横幅
    // [in] height 画像データの縦幅
    // [in] colorUseBitVal 色データの使用ビット数
    // [in] colorDatas 色データ
    void setOutputParam(unsigned short width, unsigned short height, char colorUseBitVal, const std::vector<Color>& colorDatas);

    // 指定ファイルにTGAデータを出力する
    // [in] fileName 出力するファイルパスとファイル名
    void outputTgaData(std::string_view fileName);

    // ヘッダデータを取得
    const Header& getHeader() const;

    // 色データを取得
    const std::vector<Color>& getColorDatas() const;

private:
    Header header_{}; 
    Footer footer_{};
    std::vector<Color> colorDatas_;
};



