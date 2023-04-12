// BmpTgaConverter.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include <fstream>
#include <iostream>
#include <string>
#include <windows.h>
#include <memory>
#include <cassert>
#include <filesystem>
#include <regex>

#include "../include/BmpData.h"
#include "../include/TgaData.h"
#include "../include/FileLoader.h"
#pragma once

namespace detail {
    constexpr std::string_view ReadFilePath("../Convert");
    constexpr std::string_view OutputFilePath("../Result/");
}

int main()
{
    // Convertフォルダ内のファイルを全取得してループさせる
    std::filesystem::recursive_directory_iterator directory
        = std::filesystem::recursive_directory_iterator(detail::ReadFilePath);
    for (const auto& itr : directory) {
        bool isFile = !std::filesystem::is_directory(itr.path());
        if (!isFile) {
            continue;
        }

        // ファイルをバイナリで読み込み
        FileLoader file;
        if (file.fileLoadBinary(itr.path().string())) {
            // バイナリからデータを読み取る
            if (file.getLoadDataTopAddress() == nullptr) {
                return 0;
            }

            // バイナリデータから情報を読み取る
            BmpData bmpData;
            TgaData tgaData;
            auto binary = file.getLoadDataTopAddress();
            auto size = file.getLeadDataSize();
            if (bmpData.getParamsFromBinary(*binary, size)) {
                // 読み取ったBMPデータを使ってTGAデータを出力する
                const auto& bmpInfo = bmpData.getInformation();
                tgaData.setOutputParam(
                    static_cast<unsigned short>(bmpInfo.biWidth_),
                    static_cast<unsigned short>(bmpInfo.biHeight_),
                    static_cast<char>(bmpInfo.biBitCount_),
                    bmpData.getColorDatas()
                );
                // 出力時のファイルパスの設定処理
                std::string outputFileName = std::regex_replace(itr.path().filename().string(), std::regex("bmp"), "tga");
                std::string outputPath = detail::OutputFilePath.data() + outputFileName;

                // 出力処理
                tgaData.outputTgaData(outputPath);
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
                // 出力時のファイルパスの設定処理
                std::string outputFileName = std::regex_replace(itr.path().filename().string(), std::regex("tga"), "bmp");
                std::string outputPath = detail::OutputFilePath.data() + outputFileName;

                // 出力処理
                bmpData.outputBmpData(outputPath);
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
}
