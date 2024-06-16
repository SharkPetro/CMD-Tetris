#include "ConsoleDraw.h"
#include "General.h"
// DRAW SHAPES FUNCS
void drawLine(wchar_t filler, int x0, int y0, int x1, int y1) {
	int y, x, i;
	if (x0 == x1) {
		if (y0 < y1) i = 1; else i = -1;
		//if (x0 <= 0 && x0 < nScreenWidth)
			for (y = y0; y <= y1; y += i) {
				if (y >= 0 && y < nScreenHeight) screen[y * nScreenWidth + x0] = filler;
			}
	}

	else if (y0 == y1) {
		if (x0 < x1) i = 1; else i = -1;
		//if (y0 >= 0 && y0 < nScreenHeight)
			for (x = x0; x <= x1; x += i) {
				if (x >= 0 && x < nScreenWidth) screen[y0 * nScreenWidth + x] = filler;
			}
	}

	else {
		return; //TODO: cry about not being able to write a line drawing algorithm
	}
}

void drawPixel(wchar_t filler, int x0, int y0) {
	if (x0 >= 0 && x0 < nScreenWidth && y0 >= 0 && y0 < nScreenHeight)
		screen[y0 * nScreenWidth + x0] = filler;
}

void drawFrameSolid(wchar_t hFiller, wchar_t vFiller, int x0, int y0, int x1, int y1, bool verticalFirst) {
	if (verticalFirst) {
		drawLine(vFiller, x0, y0, x0, y1);
		drawLine(vFiller, x1, y0, x1, y1);

		drawLine(hFiller, x0, y0, x1, y0);
		drawLine(hFiller, x0, y1, x1, y1);
	}
	else {
		drawLine(hFiller, x0, y0, x1, y0);
		drawLine(hFiller, x0, y1, x1, y1);

		drawLine(vFiller, x0, y0, x0, y1);
		drawLine(vFiller, x1, y0, x1, y1);
	}
}
void drawFrameWCorners(int x0, int y0, int x1, int y1, wchar_t hFiller, wchar_t vFiller, wchar_t tlC, wchar_t trC, wchar_t brC, wchar_t blC) {
	drawFrameSolid(hFiller, vFiller, x0, y0, x1, y1, true);
	drawPixel(blC, x0, y1);
	drawPixel(tlC, x0, y0);
	drawPixel(brC, x1, y1);
	drawPixel(trC, x1, y0);
}

// FILL FUNCS
void fillRect(wchar_t filler, int x0, int y0, int x1, int y1) {
	for (int x = x0; x <= x1 && x < nScreenWidth; x++) {
		for (int y = y0; y <= y1 && y < nScreenHeight; y++) {
			screen[y * nScreenWidth + x] = filler;
		}
	}
}
void fillScreen(wchar_t filler) {
	fillRect(filler, 0, 0, nScreenWidth, nScreenHeight);
}
void fillImage(std::wstring image, int x0, int y0, int width, int height) {
	for (int x = x0; x < width + x0; x++) {
		if (x >= 0 && x < nScreenWidth)
		for (int y = y0; y < height + y0; y++) {
			int i = (y - y0) * width + x - x0;
			if (image[i] != '\0' && y >= 0 && y < nScreenHeight) screen[(y)*nScreenWidth + (x)] = image[i];
		}
	}
}
// TEXT FUNCS
void showString(std::wstring literal, int x, int y) {
	for (int i = 0; literal[i] != '\0'; i++) {
		screen[y * nScreenWidth + x + i] = literal[i];
	}
}