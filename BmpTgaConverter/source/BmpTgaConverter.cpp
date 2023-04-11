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
    constexpr std::string_view ReadFilePath("../Convert/testImage.tga");
    constexpr std::string_view OutputFilePath("../Result/testImage.bmp");
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

        auto binary = file.getLoadDataTopAddress();
        auto size = file.getLeadDataSize();
        // ファイルからデータを読み込む
        BmpData bmpData;
        TgaData tgaData;
        if (bmpData.getParamsFromBinary(*binary, size)) {
            // 読み取ったBMPデータを使ってTGAデータを出力する
            const auto& bmpInfo = bmpData.getInformation();
            tgaData.setOutputParam(
                static_cast<unsigned short>(bmpInfo.biWidth_),
                static_cast<unsigned short>(bmpInfo.biHeight_),
                static_cast<char>(bmpInfo.biBitCount_),
                bmpData.getColorDatas()
            );
            tgaData.outputTgaData(detail::OutputFilePath);
        }
        else if (tgaData.getParamsFromBinary(*binary, size)) {
            // 読み取ったTGAデータを使ってBMPデータを出力する
            const auto& tgaHeader = tgaData.getHeader();
            bmpData.setOutputParam(
                static_cast<unsigned short>(tgaHeader.imageWitdth_),
                static_cast<unsigned short>(tgaHeader.imageHeight_),
                static_cast<char>(tgaHeader.bitPerPixel_),
                tgaData.getColorDatas()
            );
            bmpData.outputBmpData(detail::OutputFilePath);
        }
        else {
            std::cout << "ファイルが読み込めませんでした" << "\n";
            std::cout << "対応していない形式のファイルです" << "\n";
        }
    }
    else {
        std::cout << "ファイルが読み込めませんでした" << "\n";
        std::cout << "Convertフォルダの中にBMPかTGAのファイルを入れて再度お試しください" << "\n";
    }
}
