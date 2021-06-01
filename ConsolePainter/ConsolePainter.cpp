// ConsolePainter.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <cmath>
#include <windows.h>
#include "BitmapLibrary.h"

enum ConsoleColors {
    CC_Black = 0, CC_Red, CC_Green, CC_Yellow, CC_Blue, CC_Magneta, CC_Cyan, CC_White,
    CC_BrightBlack, CC_BrightRed, CC_BrightGreen, CC_BrightYellow, CC_BrightBlue, 
    CC_BrightMagneta, CC_BrightCyan, CC_BrightWhite
};

HANDLE hConsole;

void setColor(ConsoleColors bgColor = CC_Black, ConsoleColors fColor = CC_White) {
    SetConsoleTextAttribute(hConsole, (bgColor << 4) + fColor);
}

ConsoleColors rgbToConsoleColor(uint8_t R, uint8_t G, uint8_t B) {
    const uint8_t sensitivity = 4;
    
    uint8_t mx = max(max(R, G), B);
    uint8_t iA = mx > 127 ? 1 : 0;
    uint8_t alpha = mx / sensitivity;

    uint8_t iR = mx - R <= alpha && mx > alpha ? 1 : 0;
    uint8_t iG = mx - G <= alpha && mx > alpha ? 1 : 0;
    uint8_t iB = mx - B <= alpha && mx > alpha ? 1 : 0;

    return static_cast<ConsoleColors>(iR + (iG << 1) + (iB << 2) + (iA << 3));
}

int main(int argc, char** argv) {
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    BitmapFile file = LoadBMP("example.bmp");

    uint32_t Height = file.bmpInfoHeader.biHeight;
    uint32_t Width = file.bmpInfoHeader.biWidth;

    if (Width > 80) {
        std::cout << "Can't represent image that width > 80\n";
        return 0;
    }

    for (uint32_t irow = 0; irow < Height; ++irow) {
        for (uint32_t icol = 0; icol < Width; ++icol) {
            uint32_t ipos = ((Height - 1 - irow) * Width + icol) * 3;
            uint8_t R = file.bmpData[ipos];
            uint8_t G = file.bmpData[ipos + 1];
            uint8_t B = file.bmpData[ipos + 2];

            setColor(rgbToConsoleColor(R, G, B));
            std::cout << ' ';
        }
        if (Width < 80) std::cout << '\n';
    }

    setColor();
}
