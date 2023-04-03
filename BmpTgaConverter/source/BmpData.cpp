#include <iostream>
#include "../include/BmpData.h"

////////////////////////////////////////////////////////////
// バイナリデータからパラメータを取得する
////////////////////////////////////////////////////////////
bool BmpData::getParamsFromBinary(char& binary, const long long int& binarySize)
{
    //if (sizeof(BmpData::Header) < binarySize) {
    //    // todo:条件式をstatic_assertに入れたい
    //    static_assert(true, L"読み込み処理でイテレータがファイルサイズを超過");
    //    return false;
    //}

    char* binaryItr = reinterpret_cast<char*>(&binary);

    // ヘッダデータ取得処理
    binaryItr = getParamFromBinary(header_.bfType1_, *binaryItr);
    binaryItr = getParamFromBinary(header_.bfType2_, *binaryItr);
    binaryItr = getParamFromBinary(header_.bfSize_, *binaryItr);
    binaryItr = getParamFromBinary(header_.bfReserved1_, *binaryItr);
    binaryItr = getParamFromBinary(header_.bfReserved2_, *binaryItr);
    binaryItr = getParamFromBinary(header_.bfOffBits_, *binaryItr);

    // 情報データ取得処理
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

    // 色データ取得処理
    long int imageDataSize = info_.biWidth_ * info_.biHeight_;
    colorDatas_.reserve(imageDataSize);

    // 色データの横サイズは4バイトの倍数になるように保管される
    int actualWidthSize = info_.biWidth_ + (4 - (info_.biWidth_ % 4));
    Color tmpColor;
    for (int i = 0; i < info_.biHeight_; ++i) {
        for (int j = 0; j < actualWidthSize; ++j) {
            binaryItr = getParamFromBinary(tmpColor.r_, *binaryItr);
            binaryItr = getParamFromBinary(tmpColor.g_, *binaryItr);
            binaryItr = getParamFromBinary(tmpColor.b_, *binaryItr);
            if (j < info_.biWidth_) {
                colorDatas_.push_back(tmpColor);
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

