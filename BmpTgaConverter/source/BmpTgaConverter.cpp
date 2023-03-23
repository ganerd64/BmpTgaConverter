// BmpTgaConverter.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include <fstream>
#include <iostream>
#include <string>
#include <windows.h>
#include <memory>
#include <cassert>

#include "../include/BmpData.h"
#include "../include/FileLoader.h"
#pragma once

namespace detail {
    std::string ReadFilePath("../Convert/testImage.bmp");
}

int main()
{
    // ファイルをバイナリで読み込み
    FileLoader file;
    if (file.fileLoadBinary(detail::ReadFilePath)) {
        BmpData bmpData;

        // バイナリからBMPデータを読み取る
        if (file.getLoadDataTopAddress() != nullptr) {
            auto binary = file.getLoadDataTopAddress();
            auto size = file.getLeadDataSize();
            bmpData.getParamsFromBinary(*binary, size);
        }
    }
    else {
        std::cout << "ファイルが読み込めませんでした" << "\n";
        std::cout << "Convertフォルダの中にBMPかTGAのファイルを入れて再度お試しください" << "\n";
    }
}
