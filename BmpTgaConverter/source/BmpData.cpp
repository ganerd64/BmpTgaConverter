#include <iostream>
#include <fstream>
#include "../include/BmpData.h"

namespace detail {

    constexpr char BfType[3] = "BM";

    // ヘッダデータのバイトサイズ
    constexpr int HeaderSize = 14;

    // 情報データの最低サイズ
    constexpr int InfoMinSize = 40;

    // プレーン数の固定の値（常に１）
    constexpr int PlanesVal = 1;

    // データの最低保障サイズ（ヘッダ14バイト　＋　情報データ40バイト）
    constexpr long long int DataMinSize = HeaderSize + InfoMinSize;
}

////////////////////////////////////////////////////////////
// バイナリデータからパラメータを取得する
////////////////////////////////////////////////////////////
bool BmpData::getParamsFromBinary(char& binary, const long long int& binarySize)
{
    if (binarySize <= 0 && binarySize >= detail::DataMinSize) {
        return false;
    }

    char* binaryItr = reinterpret_cast<char*>(&binary);

    // ヘッダデータ取得処理
    {
        binaryItr = getParamFromBinary(header_.bfType1_, *binaryItr);
        binaryItr = getParamFromBinary(header_.bfType2_, *binaryItr);
        binaryItr = getParamFromBinary(header_.bfSize_, *binaryItr);
        binaryItr = getParamFromBinary(header_.bfReserved1_, *binaryItr);
        binaryItr = getParamFromBinary(header_.bfReserved2_, *binaryItr);
        binaryItr = getParamFromBinary(header_.bfOffBits_, *binaryItr);

        // BMPデータか確認
        if (header_.bfType1_ != detail::BfType[0] || header_.bfType2_ != detail::BfType[1]) {
            header_ = {};
            return false;
        }
    }

    // 情報データ取得処理
    {
        binaryItr = getParamFromBinary(info_.biSize_, *binaryItr);
        binaryItr = getParamFromBinary(info_.biWidth_, *binaryItr);
        binaryItr = getParamFromBinary(info_.biHeight_, *binaryItr);
        binaryItr = getParamFromBinary(info_.biPlanes_, *binaryItr);
        binaryItr = getParamFromBinary(info_.biBitCount_, *binaryItr);
        binaryItr = getParamFromBinary(info_.biCompression_, *binaryItr);
        binaryItr = getParamFromBinary(info_.biSizeImage_, *binaryItr);
        binaryItr = getParamFromBinary(info_.biXPixPerMeter_, *binaryItr);
        binaryItr = getParamFromBinary(info_.biYPixPerMeter_, *binaryItr);
        binaryItr = getParamFromBinary(info_.biClrUsed_, *binaryItr);
        binaryItr = getParamFromBinary(info_.biCirImportant_, *binaryItr);
        if (info_.biSize_ >= InfoCategory::B4) {
            binaryItr = getParamFromBinary(info_.bV4RedMask_, *binaryItr);
            binaryItr = getParamFromBinary(info_.bV4GreenMask_, *binaryItr);
            binaryItr = getParamFromBinary(info_.bV4BlueMask_, *binaryItr);
            binaryItr = getParamFromBinary(info_.bV4AlphaMask_, *binaryItr);
            binaryItr = getParamFromBinary(info_.bV4CSType_, *binaryItr);
            for (auto& data : info_.bV4Endpoints_) {
                // 使わないデータなのでバイト数分回してイテレータを進めています
                binaryItr = getParamFromBinary(data, *binaryItr);
            }
            binaryItr = getParamFromBinary(info_.bV4GammaRed_, *binaryItr);
            binaryItr = getParamFromBinary(info_.bV4GammaGreen_, *binaryItr);
            binaryItr = getParamFromBinary(info_.bV4GammaBlue_, *binaryItr);
        }
        if (info_.biSize_ >= InfoCategory::B5) {
            binaryItr = getParamFromBinary(info_.bV5Intent_, *binaryItr);
            binaryItr = getParamFromBinary(info_.bV5ProfileData_, *binaryItr);
            binaryItr = getParamFromBinary(info_.bV5ProfileSize_, *binaryItr);
            binaryItr = getParamFromBinary(info_.bV5Reserved_, *binaryItr);
        }
    }

    // 色データ取得処理
    {
        int colorUseByte = 0;
        if (info_.biBitCount_ == static_cast<char>(ColorBitType::Bit24)) {
            colorUseByte = static_cast<int>(ColorUseByteSize::Bit24UseByte);
        }
        else if (info_.biBitCount_ == static_cast<char>(ColorBitType::Bit32)) {
            colorUseByte = static_cast<int>(ColorUseByteSize::Bit32UseByte);
        }
        else {
            // ピクセルのビット数が24bitと32bitの時以外は対応しない
            header_ = {};
            info_ = {};
            return false;
        }

        long long int imageDataSize = info_.biWidth_ * info_.biHeight_;
        colorDatas_.reserve(imageDataSize);

        // 色データの横のバイトサイズは4バイトの倍数になるように補完される
        int widthBytesize = (info_.biWidth_ * colorUseByte);
        int actualWidthByteSize = widthBytesize + ((4 - widthBytesize % 4) % 4);

        Color tmpColor;
        for (int i = 0; i < info_.biHeight_; ++i) {
            for (int j = 0; j < info_.biWidth_; ++j) {
                binaryItr = getParamFromBinary(tmpColor.r_, *binaryItr);
                binaryItr = getParamFromBinary(tmpColor.g_, *binaryItr);
                binaryItr = getParamFromBinary(tmpColor.b_, *binaryItr);
                if (colorUseByte == static_cast<int>(ColorUseByteSize::Bit32UseByte)) {
                    binaryItr = getParamFromBinary(tmpColor.reserve_, *binaryItr);
                }
                if (j < info_.biWidth_) {
                    colorDatas_.push_back(tmpColor);
                }
            }
            // 横のバイトサイズが４バイトの倍数になるように補完された分だけイテレータを進める
            for (int j = 0; j < actualWidthByteSize - widthBytesize; ++j) {
                char temp;
                binaryItr = getParamFromBinary(temp, *binaryItr);
            }
        }
    }

    return true;
}

////////////////////////////////////////////////////////////
// ヘッダデータを取得
////////////////////////////////////////////////////////////
const BmpData::Header& BmpData::getHeader() const
{
    return header_;
}

////////////////////////////////////////////////////////////
// 情報データを取得
////////////////////////////////////////////////////////////
const BmpData::Information& BmpData::getInformation() const
{
    return info_;
}

////////////////////////////////////////////////////////////
// 色データを取得
////////////////////////////////////////////////////////////
const std::vector<ImageData::Color>& BmpData::getColorDatas() const
{
    return colorDatas_;
}

////////////////////////////////////////////////////////////
// 出力時に必要なデータを設定
////////////////////////////////////////////////////////////
void BmpData::setOutputParam(unsigned short width, unsigned short height, char colorUseBitVal, const std::vector<Color>& colorDatas)
{
    // 渡されたパラメータを設定
    info_.biWidth_ = width;
    info_.biHeight_ = height;
    info_.biBitCount_ = colorUseBitVal;
    colorDatas_ = colorDatas;

    // 出力時に必要なデータを設定
    header_.bfType1_ = detail::BfType[0];
    header_.bfType2_ = detail::BfType[1];
    header_.bfOffBits_ = detail::DataMinSize;
    info_.biSize_ = detail::InfoMinSize;
    info_.biPlanes_ = detail::PlanesVal;

}

////////////////////////////////////////////////////////////
// 指定ファイルにBMPデータを出力する
////////////////////////////////////////////////////////////
void BmpData::outputBmpData(std::string_view fileName)
{
    // サイズ０はデータが無いのでreturn
    if (info_.biWidth_ <= 0 || info_.biHeight_ <= 0) {
        return;
    }

    // データサイズを計算する
    int totalDataSize = 0;
    int colorUseByte = 0;

    if (info_.biBitCount_ == static_cast<char>(ColorBitType::Bit24)) {
        colorUseByte = static_cast<int>(ColorUseByteSize::Bit24UseByte);
    }
    else if (info_.biBitCount_ == static_cast<char>(ColorBitType::Bit32)) {
        colorUseByte = static_cast<int>(ColorUseByteSize::Bit32UseByte);
    }
    else {
        // ピクセルのビット数が24bitと32bitの時以外は対応しない
        return;
    }
    // 色データの横のバイトサイズは4バイトの倍数になるように補完される
    int widthBytesize = (info_.biWidth_ * colorUseByte);
    int actualWidthByteSize = widthBytesize + ((4 - widthBytesize % 4) % 4);
    int colorDataSize = actualWidthByteSize * info_.biHeight_;
    totalDataSize = colorDataSize + detail::DataMinSize;

    // 出力前にファイルサイズをパラメータに反映
    header_.bfSize_ = totalDataSize;

    // バイナリデータに書き込んでいく
    std::ofstream ofs(fileName.data(), std::ios::binary);
    std::unique_ptr<char[]> outputData = std::make_unique<char[]>(totalDataSize);
    {
        char* binaryItr = reinterpret_cast<char*>(outputData.get());

        // ヘッダデータ書き込み処理
        binaryItr = setParamToBinary(header_.bfType1_, *binaryItr);
        binaryItr = setParamToBinary(header_.bfType2_, *binaryItr);
        binaryItr = setParamToBinary(header_.bfSize_, *binaryItr);
        binaryItr = setParamToBinary(header_.bfReserved1_, *binaryItr);
        binaryItr = setParamToBinary(header_.bfReserved2_, *binaryItr);
        binaryItr = setParamToBinary(header_.bfOffBits_, *binaryItr);

        binaryItr = setParamToBinary(info_.biSize_, *binaryItr);
        binaryItr = setParamToBinary(info_.biWidth_, *binaryItr);
        binaryItr = setParamToBinary(info_.biHeight_, *binaryItr);
        binaryItr = setParamToBinary(info_.biPlanes_, *binaryItr);
        binaryItr = setParamToBinary(info_.biBitCount_, *binaryItr);
        binaryItr = setParamToBinary(info_.biCompression_, *binaryItr);
        binaryItr = setParamToBinary(info_.biSizeImage_, *binaryItr);
        binaryItr = setParamToBinary(info_.biXPixPerMeter_, *binaryItr);
        binaryItr = setParamToBinary(info_.biYPixPerMeter_, *binaryItr);
        binaryItr = setParamToBinary(info_.biClrUsed_, *binaryItr);
        binaryItr = setParamToBinary(info_.biCirImportant_, *binaryItr);

        int count = 0;
        for (auto& colorData : colorDatas_) {
            binaryItr = setParamToBinary(colorData.r_, *binaryItr);
            binaryItr = setParamToBinary(colorData.g_, *binaryItr);
            binaryItr = setParamToBinary(colorData.b_, *binaryItr);
            count++;

            if (colorUseByte == static_cast<int>(ColorUseByteSize::Bit32UseByte)) {
                binaryItr = setParamToBinary(colorData.reserve_, *binaryItr);
            }
            else {
                // 横のバイトサイズが４バイトの倍数になるように0の値で補完する
                if (count % info_.biWidth_ == 0 && count != 0) {
                    for (int i = 0; i < actualWidthByteSize - widthBytesize; ++i) {
                        char tmp = 0;
                        binaryItr = setParamToBinary(tmp, *binaryItr);
                    }
                }
            }
        }
    }

    // バイナリとして書き込む
    ofs.write(outputData.get(), totalDataSize);
}
