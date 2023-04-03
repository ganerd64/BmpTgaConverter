////////////////////////////////////////////////////////////
// バイナリデータからデータを読み取る用の関数定義ヘッダ
////////////////////////////////////////////////////////////
#include <iostream>
#include <vector> 
#pragma once
class ImageData
{
public:
    ImageData() = default;
    ~ImageData() = default;

    struct Color
    {
        unsigned char r_ = 0;
        unsigned char g_ = 0;
        unsigned char b_ = 0;
        unsigned char reserve_ = 0;
    };

    enum ColorBitType {
        Bit24 = 24,
        Bit32 = 32,
    };
    enum ColorUseByteSize {
        Bit24UseByte = 3,
        Bit32UseByte = 4,
    };
protected:
    // テンプレート関数はヘッダに定義しないとエラーになるのでこちらで中身を定義
    // https://qiita.com/i153/items/38f9688a9c80b2cb7da7

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


    template<class T>
    char* setParamToBinary(T& val, char& binary)
    {
        // 値代入用の変数
        char* valItr = reinterpret_cast<char*>(&val);
        char* binaryItr = reinterpret_cast<char*>(&binary);
        for (int i = 0; i < sizeof(T); ++i) {
            *binaryItr = *valItr;
            binaryItr++;
            valItr++;
        }
        return binaryItr;
    }

    // 画像の色データの上下を反転させる
    // [in] width   画像横サイズ
    // [in] height  画像縦サイズ
    // [in/out] colorDatas バイナリデータの先頭アドレス
    void flipVerticalColorDatas(int width, int height, std::vector<Color>& colorDatas);
};
