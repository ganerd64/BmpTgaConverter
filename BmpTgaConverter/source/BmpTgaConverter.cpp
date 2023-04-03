// BmpTgaConverter.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include <fstream>
#include <iostream>
#include <string>
#include <windows.h>
#include <memory>
#include <cassert>

#include "../include/BmpData.h"
#include "../include/TgaData.h"
#include "../include/FileLoader.h"
#pragma once

namespace detail {
    // todo:Convertフォルダの中にあるファイルを取得してコンバートすること
    constexpr std::string_view ReadFilePath("../Convert/testImage.bmp");
    constexpr std::string_view OutputFilePath("../Result/testImage.tga");
}

int main()
{
    // ファイルをバイナリで読み込み
    FileLoader file;
    if (file.fileLoadBinary(detail::ReadFilePath)) {
        // バイナリからBMPデータを読み取る
        if (file.getLoadDataTopAddress() == nullptr) {
            return 0;
        }

        // ファイルからBMPデータを読み込む
        BmpData bmpData;
        auto binary = file.getLoadDataTopAddress();
        auto size = file.getLeadDataSize();
        bmpData.getParamsFromBinary(*binary, size);

        // 読み取ったBMPデータを使ってTGAデータを出力する
        TgaData tgaData;
        const auto& bmpInfo = bmpData.getInformation();
        tgaData.setHeader(
            static_cast<unsigned short>(bmpInfo.biWidth_),
            static_cast<unsigned short>(bmpInfo.biHeight_),
            static_cast<char>(bmpInfo.biBitCount_)
        );
        tgaData.setColorDatas(bmpData.getColorDatas());
        tgaData.outputTgaData(detail::OutputFilePath);
    }
    else {
        std::cout << "ファイルが読み込めませんでした" << "\n";
        std::cout << "Convertフォルダの中にBMPかTGAのファイルを入れて再度お試しください" << "\n";
    }
}
