#include <iostream>
#include "../include/BmpData.h"

////////////////////////////////////////////////////////////
// バイナリデータからパラメータを取得する
////////////////////////////////////////////////////////////
bool BmpData::getParamsFromBinary(char& binary, const long long int& binarySize)
{
    char* binaryItr = reinterpret_cast<char*>(&binary);

    // ヘッダデータ取得処理
    {
        binaryItr = getParamFromBinary(header_.bfType1_, *binaryItr);
        binaryItr = getParamFromBinary(header_.bfType2_, *binaryItr);
        binaryItr = getParamFromBinary(header_.bfSize_, *binaryItr);
        binaryItr = getParamFromBinary(header_.bfReserved1_, *binaryItr);
        binaryItr = getParamFromBinary(header_.bfReserved2_, *binaryItr);
        binaryItr = getParamFromBinary(header_.bfOffBits_, *binaryItr);
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
                // 使わないデータなのでバイト数分回して取得しています
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
            return false;
        }

        long long int imageDataSize = info_.biWidth_ * info_.biHeight_;
        colorDatas_.reserve(imageDataSize);

        // 色データの横のバイトサイズは4バイトの倍数になるように保管される
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
            // 横のバイトサイズが４バイトの倍数になるように保管された分だけイテレータを進める
            for (int j = 0; j < actualWidthByteSize - widthBytesize; ++j) {
                char temp;
                binaryItr = getParamFromBinary(temp, *binaryItr);
            }
        }
    }

    // BMPファイルの色データは上下が反転している状態なので必要であれば反転させる
    //flipVerticalColorDatas(info_.biWidth_, info_.biHeight_, colorDatas_);

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

