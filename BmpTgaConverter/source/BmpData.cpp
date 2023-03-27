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
    // 色データの横サイズは4バイトの倍数になるように保管される
    int actualWidthSize = info_.biWidth_ + (4 - (info_.biWidth_ % 4));
    long int imageDataSize = actualWidthSize * info_.biHeight_;
    colorDatas_.reserve(imageDataSize);

    Color tmpColor;
    for (int i = 0; i < imageDataSize; ++i) {
        binaryItr = getParamFromBinary(tmpColor.r_, *binaryItr);
        binaryItr = getParamFromBinary(tmpColor.g_, *binaryItr);
        binaryItr = getParamFromBinary(tmpColor.b_, *binaryItr);
        colorDatas_.push_back(tmpColor);
    }

    // BMPファイルの色データは上下が反転している状態なのでこちらで反転させる
    flipVerticalColorDatas(actualWidthSize, info_.biHeight_, colorDatas_);
    return true;
}

////////////////////////////////////////////////////////////
// ヘッダデータを取得
////////////////////////////////////////////////////////////
BmpData::Header BmpData::getHeader() const
{
    return header_;
}

////////////////////////////////////////////////////////////
// 画像の色データの上下を反転させる
////////////////////////////////////////////////////////////
void BmpData::flipVerticalColorDatas(int width, int height, std::vector<Color>& colorDatas)
{
    if (colorDatas.empty()) {
        return;
    }
    int totalSize = width * height;
    if (colorDatas.size() < totalSize) {
        return;
    }
    
    // 上下反転させるには上半分と下半分の色データにアクセスして入れ替えていく
    int halfHeight = static_cast<int>(height / 2);

    for (int i = 0; i < halfHeight; ++i) {
        for (int j = 0; j < width; ++j) {
            // 上側の入れ替え位置を計算する
            int swapUpPos = (width * i) + j;
            
            // 下側の入れ替え位置を計算する
            int swapDownPos = (height - 1 - i) * width + j;
            
            // 下側の入れ替え位置の計算は難しいので別解をおいておく
            //int swapDownPos = (totalSize) + j - ((width) * (i + 1));

            Color temp;
            temp = colorDatas[swapUpPos];
            colorDatas[swapUpPos] = colorDatas[swapDownPos];
            colorDatas[swapDownPos] = temp;
        }
    }
}
