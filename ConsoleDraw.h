#pragma once
#include <string>
#include "General.h"

// DRAW SHAPES FUNCS
void drawLine(wchar_t filler, int x0, int y0, int x1, int y1);
void drawPixel(wchar_t filler, int x0, int y0);
void drawFrameSolid(wchar_t hFiller, wchar_t vFiller, int x0, int y0, int x1, int y1, bool verticalFirst = true);
void drawFrameWCorners(int x0, int y0, int x1, int y1, wchar_t hFiller = 0x2550, wchar_t vFiller = 0x2551, wchar_t tlC = 0x2554, wchar_t trC = 0x2557, wchar_t brC = 0x255D, wchar_t blC = 0x255A);
// FILL FUNCS
void fillRect(wchar_t filler, int x0, int y0, int x1, int y1);
void fillScreen(wchar_t filler);
void fillImage(std::wstring image, int x0, int y0, int width, int height);
// TEXT FUNCS
void showString(std::wstring literal, int x, int y);