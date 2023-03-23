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
    binaryItr = getParamFromBinary(header_.bfType1_, *binaryItr);
    binaryItr = getParamFromBinary(header_.bfType2_, *binaryItr);
    binaryItr = getParamFromBinary(header_.bfSize_, *binaryItr);
    binaryItr = getParamFromBinary(header_.bfReserved1_, *binaryItr);
    binaryItr = getParamFromBinary(header_.bfReserved2_, *binaryItr);
    binaryItr = getParamFromBinary(header_.bfOffBits_, *binaryItr);

    return true;
}


////////////////////////////////////////////////////////////
// ヘッダデータを取得
////////////////////////////////////////////////////////////
BmpData::Header BmpData::getHeader() const
{
    return header_;
}
