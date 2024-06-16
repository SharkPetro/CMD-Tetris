#include <Windows.h>
#include <iostream>
#include <string>
#include "General.h"
#include "ConsoleDraw.h"
#include "Tetris.h"
#include "InputHandler.h"
#include <chrono>
#include <random>

using namespace std;
using namespace chrono;

int nScreenHeight;
int nScreenWidth;
wchar_t* screen;
int speedLevel = 5;

int main() {
	srand(time(0));
	nScreenHeight = 44;
	nScreenWidth = 30;
	screen = new wchar_t[nScreenHeight * nScreenWidth];
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	HWND HWNDConsole = GetConsoleWindow();
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;

	RECT ConsoleRect;
	GetWindowRect(HWNDConsole, &ConsoleRect);
	MoveWindow(HWNDConsole, ConsoleRect.left, ConsoleRect.top, (nScreenWidth+1)*16, (nScreenHeight+4)*12, TRUE);

	
	int x = 5;
	int y = 5;
	int tetroRotation = 0;
	int messageScore = 0;
	int messageTimer = 0;
	wstring tetromino;
	fillScreen(' ');
	tetrisDrawFrame();
	fillGameBuffer();
	drawGameBuffer(1, 0);
	spawnTetromino(x, y, tetromino);
	predictTetromino();
	auto currentTime = system_clock::now();
	auto previousTime = system_clock::now();
	int elapsed = duration_cast<milliseconds>(currentTime - previousTime).count();
	gameBuffer = L"";
	gameBuffer += L"          ";
	gameBuffer += L"          ";
	gameBuffer += L"          ";
	gameBuffer += L"          ";
	gameBuffer += L"          ";
	gameBuffer += L"          ";
	gameBuffer += L"          ";
	gameBuffer += L"          ";
	gameBuffer += L"          ";
	gameBuffer += L"          ";
	gameBuffer += L"          ";
	gameBuffer += L"          ";
	gameBuffer += L"          ";
	gameBuffer += L"          ";
	gameBuffer += L"          ";
	gameBuffer += L"          ";
	gameBuffer += L"          ";
	gameBuffer += L"          ";
	gameBuffer += L"          ";
	gameBuffer += L"          ";
	gameBuffer += L"          ";
	gameBuffer += L"          ";
	gameBuffer += L"          ";
	gameBuffer += L"          ";
	gameBuffer += L"          ";
	gameBuffer += L"          ";
	gameBuffer += L"          ";
	gameBuffer += L"          ";
	gameBuffer += L"          ";
	gameBuffer += L"          ";
	gameBuffer += L"          ";
	gameBuffer += L"          ";
	gameBuffer += L"          ";
	gameBuffer += L"          ";
	gameBuffer += L"          ";
	gameBuffer += L"          ";
	gameBuffer += L"          ";
	gameBuffer += L"          ";
	gameBuffer += L"          ";
	gameBuffer += L"          ";
	while (1) {
		if (ongoingGame) {
			previousTime = currentTime;
			currentTime = system_clock::now();
			duration<double> delta = currentTime - previousTime;

			// GET INPUT
			LStickInput = { 0, 0 };
			if (getKeyType('A'))
				LStickInput.first -= 1;
			if (getKeyType('D'))
				LStickInput.first += 1;
			if (getKeyType('W')) {
				tetroRotation += 1;
				tetromino = getTetromino(tetrostatus[0], tetroRotation, tetrostatus[1], fillers[tetrostatus[2]]);
				if (checkTetrominoCollision(tetromino, x, y))
					tetroRotation -= 1;
			}
			if (getKeyType('S'))
				LStickInput.second += 1;
			// DRAW
			fillScreen(' ');
			tetrisDrawFrame(0, 0);
			drawGameBuffer(1, 1);

			fillImage(tetromino, x, y, 4, 4);
			drawPredictionTetromino();
			showString(L"Level " + to_wstring(level), gamePosX + gameSizeX + 2, gamePosY + 8);
			showString(L"Score " + to_wstring(score), gamePosX + gameSizeX + 2, gamePosY + 10);
			tetromino = getTetromino(tetrostatus[0], tetroRotation, tetrostatus[1], fillers[tetrostatus[2]]);

			if (messageTimer > 0) {
				showScoreMessage(0, messageScore).second;
			}
			else messageScore = 0;
			// GAME UPDATE
			moveTetromino(x, y, LStickInput, tetromino);
			elapsed += duration_cast<milliseconds>(delta).count();
			if (elapsed >= 30 - speedLevel) {
				elapsed = 0;
				messageTimer--;
				int a = removeClearedLines();
				fallTetromino(x, y, tetromino);
				checkUpdatedLines();
				if (a > 0) {
					messageScore = a;
					messageTimer = 5;
					score += showScoreMessage(0, messageScore).second;
				}
				levelUp();
			}
		}
		screen[nScreenWidth * nScreenHeight - 1] = '\0';
		WriteConsoleOutputCharacterW(hConsole, screen, nScreenHeight * nScreenWidth, { 0, 0 }, &dwBytesWritten);
	}
}