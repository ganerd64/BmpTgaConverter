////////////////////////////////////////////////////////////
// バイナリデータからデータを読み取る用の関数定義ヘッダ
////////////////////////////////////////////////////////////
#include <iostream>
#pragma once


// バイナリデータからTのサイズ分イテレータを進めてvalに値を取得させる
// [out] val 取得した値が入る
// [in] binary バイナリデータ
// [return] binaryからTのサイズ分進めたイテレータが返る
template<class T>
char* getParamFromBinary(T& val, char& binary)
{
    // 値取得用の変数
    char* valItr = reinterpret_cast<char*>(&val);
    char* binaryItr = reinterpret_cast<char*>(&binary);
    for (int i = 0; i < sizeof(T); ++i) {
        *valItr = *binaryItr;
        binaryItr++;
        valItr++;
    }
    return binaryItr;
}

// エンディアンの反転処理
template<class T>
T reverseEndian(T& val)
{
    // スカラー型でなければ処理しない
    static_assert(std::is_scalar<T>::value, L"reverseEndian：渡されたTの型がスカラーではありません");

    // 反転処理
    char* first = reinterpret_cast<char*>(&val);
    char* last = first + sizeof(T);
    std::reverse(first, last);
    return val;
}
