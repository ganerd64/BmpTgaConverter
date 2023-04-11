#include <iostream>
#include <fstream>
#include <memory>
#include "../include/TgaData.h"

namespace detail {
    constexpr int HeaderSize = 18;
    constexpr int FooterSize = 26;
}

////////////////////////////////////////////////////////////
// バイナリデータからパラメータを取得する
////////////////////////////////////////////////////////////
bool TgaData::getParamsFromBinary(char& binary, const long long int& binarySize)
{
    char* binaryItr = reinterpret_cast<char*>(&binary);

    // ヘッダデータ取得処理
    binaryItr = getParamFromBinary(header_.idFieldLength_, *binaryItr);
    binaryItr = getParamFromBinary(header_.colorMapType_, *binaryItr);
    binaryItr = getParamFromBinary(header_.imageType_, *binaryItr);
    binaryItr = getParamFromBinary(header_.colorMapIndex_, *binaryItr);
    binaryItr = getParamFromBinary(header_.colorMapLength_, *binaryItr);
    binaryItr = getParamFromBinary(header_.colorMapSize_, *binaryItr);
    binaryItr = getParamFromBinary(header_.imageOriginX_, *binaryItr);
    binaryItr = getParamFromBinary(header_.imageOriginY_, *binaryItr);
    binaryItr = getParamFromBinary(header_.imageWitdth_, *binaryItr);
    binaryItr = getParamFromBinary(header_.imageHeight_, *binaryItr);
    binaryItr = getParamFromBinary(header_.bitPerPixel_, *binaryItr);
    binaryItr = getParamFromBinary(header_.discripter_, *binaryItr);

    int colorUseByte = 0;
    {
        if (header_.bitPerPixel_ == static_cast<char>(ColorBitType::Bit24)) {
            colorUseByte = static_cast<int>(ColorUseByteSize::Bit24UseByte);
        }
        else if (header_.bitPerPixel_ == static_cast<char>(ColorBitType::Bit32)) {
            colorUseByte = static_cast<int>(ColorUseByteSize::Bit32UseByte);
        }
        else {
            // ピクセルのビット数が24bitと32bitの時以外は対応しない
            header_ = {};
            return false;
        }

        // 非圧縮フルカラーのみ対応
        if (header_.imageType_ != ImageType::FullColor) {
            header_ = {};
            return false;
        }
    }


    // 色データ取得処理
    long long int imageDataSize = header_.imageWitdth_ * header_.imageHeight_;
    colorDatas_.reserve(imageDataSize);
    Color tmpColor;
    for (int i = 0; i < header_.imageWitdth_; ++i) {
        for (int j = 0; j < header_.imageHeight_; ++j) {
            binaryItr = getParamFromBinary(tmpColor.r_, *binaryItr);
            binaryItr = getParamFromBinary(tmpColor.g_, *binaryItr);
            binaryItr = getParamFromBinary(tmpColor.b_, *binaryItr);
            if (colorUseByte == static_cast<int>(ColorUseByteSize::Bit32UseByte)) {
                binaryItr = setParamToBinary(tmpColor.reserve_, *binaryItr);
            }
            colorDatas_.push_back(tmpColor);
        }
    }

    // 情報データ取得処理
    binaryItr = getParamFromBinary(footer_.filePosition_, *binaryItr);
    binaryItr = getParamFromBinary(footer_.developerDirectoryFilePosition_, *binaryItr);
    for (auto& charData : footer_.trueVisionTargaCharacter_) {
        binaryItr = getParamFromBinary(charData, *binaryItr);
    }
    binaryItr = getParamFromBinary(footer_.eof_, *binaryItr);

    return true;
}

////////////////////////////////////////////////////////////
// ヘッダデータを設定
////////////////////////////////////////////////////////////
void TgaData::setOutputParam(unsigned short width, unsigned short height, char colorUseBitVal, const std::vector<Color>& colorDatas)
{
    header_.imageType_ = static_cast<char>(ImageType::FullColor);
    header_.imageWitdth_ = width;
    header_.imageHeight_ = height;
    header_.bitPerPixel_ = colorUseBitVal;
    colorDatas_ = colorDatas;
}

////////////////////////////////////////////////////////////
// 指定ファイルにTGAデータを出力する
////////////////////////////////////////////////////////////
void TgaData::outputTgaData(std::string_view fileName)
{
    // サイズ０はデータが無いのでreturn
    if (header_.imageWitdth_ <= 0 || header_.imageHeight_ <= 0) {
        return;
    }

    // データサイズを計算する
    int totalDataSize = 0;
    int colorUseByte = 0;
    {
        if (header_.bitPerPixel_ == static_cast<char>(ColorBitType::Bit24)) {
            colorUseByte = static_cast<int>(ColorUseByteSize::Bit24UseByte);
        }
        else if (header_.bitPerPixel_ == static_cast<char>(ColorBitType::Bit32)) {
            colorUseByte = static_cast<int>(ColorUseByteSize::Bit32UseByte);
        }
        else {
            // ピクセルのビット数が24bitと32bitの時以外は対応しない
            return;
        }

        int colorDataSize = header_.imageWitdth_ * header_.imageHeight_ * colorUseByte;
        totalDataSize = detail::HeaderSize + detail::FooterSize + colorDataSize;
    }

    // バイナリデータを書き込んでいく
    std::ofstream ofs(fileName.data(), std::ios::binary);
    std::unique_ptr<char[]> outputData = std::make_unique<char[]>(totalDataSize);
    {
        char* binaryItr = reinterpret_cast<char*>(outputData.get());

        // ヘッダデータ書き込み処理
        binaryItr = setParamToBinary(header_.idFieldLength_, *binaryItr);
        binaryItr = setParamToBinary(header_.colorMapType_, *binaryItr);
        binaryItr = setParamToBinary(header_.imageType_, *binaryItr);
        binaryItr = setParamToBinary(header_.colorMapIndex_, *binaryItr);
        binaryItr = setParamToBinary(header_.colorMapLength_, *binaryItr);
        binaryItr = setParamToBinary(header_.colorMapSize_, *binaryItr);
        binaryItr = setParamToBinary(header_.imageOriginX_, *binaryItr);
        binaryItr = setParamToBinary(header_.imageOriginY_, *binaryItr);
        binaryItr = setParamToBinary(header_.imageWitdth_, *binaryItr);
        binaryItr = setParamToBinary(header_.imageHeight_, *binaryItr);
        binaryItr = setParamToBinary(header_.bitPerPixel_, *binaryItr);
        binaryItr = setParamToBinary(header_.discripter_, *binaryItr);

        for (auto& colorData : colorDatas_) {
            binaryItr = setParamToBinary(colorData.r_, *binaryItr);
            binaryItr = setParamToBinary(colorData.g_, *binaryItr);
            binaryItr = setParamToBinary(colorData.b_, *binaryItr);
            if (colorUseByte == static_cast<int>(ColorUseByteSize::Bit32UseByte)) {
                binaryItr = setParamToBinary(colorData.reserve_, *binaryItr);
            }
        }

        binaryItr = setParamToBinary(footer_.filePosition_, *binaryItr);
        binaryItr = setParamToBinary(footer_.developerDirectoryFilePosition_, *binaryItr);
        for (auto& charData : footer_.trueVisionTargaCharacter_) {
            binaryItr = setParamToBinary(charData, *binaryItr);
        }
        binaryItr = setParamToBinary(footer_.eof_, *binaryItr);
    }

    // バイナリとして書き込む
    ofs.write(outputData.get(), totalDataSize);
}

////////////////////////////////////////////////////////////
// ヘッダデータを取得
////////////////////////////////////////////////////////////
const TgaData::Header& TgaData::getHeader() const
{
    return header_;
}


////////////////////////////////////////////////////////////
// 色データを取得
////////////////////////////////////////////////////////////
const std::vector<ImageData::Color>& TgaData::getColorDatas() const
{
    return colorDatas_;
}
