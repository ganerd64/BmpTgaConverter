#include <iostream>
#include "../include/TgaData.h"

////////////////////////////////////////////////////////////
// バイナリデータからパラメータを取得する
////////////////////////////////////////////////////////////
bool TgaData::getParamsFromBinary(char& binary, const long long int& binarySize)
{
    return true;
}


////////////////////////////////////////////////////////////
// ヘッダデータを取得
////////////////////////////////////////////////////////////
TgaData::Header TgaData::getHeader() const
{
    return header_;
}

////////////////////////////////////////////////////////////
// 画像の色データの上下を反転させる
////////////////////////////////////////////////////////////
void TgaData::flipVerticalColorDatas(int width, int height, std::vector<Color>& colorDatas)
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
            // 上側と下側の入れ替え先を計算する
            int swapUpPos = (height * i) + j;
            int swapDownPos = totalSize - 1 + j - ((width - 1) * (i + 1));

            Color temp;
            temp = colorDatas[swapUpPos];
            colorDatas[swapUpPos] = colorDatas[swapDownPos];
            colorDatas[swapDownPos] = temp;
        }
    }
}
