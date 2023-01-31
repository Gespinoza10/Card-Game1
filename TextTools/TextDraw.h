#pragma once
#include <iostream>
#include <string>
#include <conio.h>
#include <fstream>
#include "MyMouse.h"
#include <Windows.h>
#include <iomanip>      // std::setw
#include "dirent.h";
#include "TextArt.h"

using namespace std;

class BoardData
{
public:
    int symbol;
    int color;
    int bgcolor;
};

class TextDraw : public Coordinates
{
private:
    int symbols[17][15];
    int colorPallet[5][5];
    int colorPallet_Background[5][5];
    void DisplaySymbols();
    void PasteClipboard();
    int PalletForegroundColor_y, PalletForegroundColor_x;
    int PalletBackgroundColor_y, PalletBackgroundColor_x;
    bool fillFlag;
    bool drawBorderFlag;
    int symbolX, symbolY;
    int xPivot, yPivot; // used when displaying all cards in files folder
public:
    int rows, columns;
    BoardData board[100][100];
    MyMouse M;
    int symbol;
    int currentColor, currentBackgroundColor;

    TextDraw(int r, int c, bool hollow);
    void DisplayInterface();
    void DrawShape();
    void StartDrawing();
    void LoadFile(string filename);
    void PrintCommandOptions(string menu);
    void DisplayFrameTypes();
    void UpdateBoardElement(int r, int c, int value, int clr, int bgclr);
};