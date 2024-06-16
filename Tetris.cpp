#include "Tetris.h"
#include "ConsoleDraw.h"
#include <string>

bool ongoingGame = true;
int tetrominosSpawned = 0;
int linesCleared = 0;
int level = 1;
int score;
int gameSizeX = 10;
int gameSizeY = 40;
int gamePosX = 0;
int gamePosY = 0;
int tetrostatus[3] = { 0, 0, 0 };
int tetrostatusPred[3] = { 0, 0, 0 };
std::vector<int> updatedLines;
std::vector<int> clearedLines;
bool cleared;
std::pair<bool, int> checker = { false, 0 };

wchar_t fillers[5]{
	0x2591, 0x2592, 0x2593,
};

std::wstring gameBuffer;

const wchar_t *tetromino[17] = {
	L"..#...#...#...#.", //straight
	L".....##..##.....", //square
	L".....#...#...##.", //l-shape
	L".....#...##...#.", //s-shape
	L".....###..#....." //t-shape
};

void tetrisDrawFrame(int offsetX, int offsetY) {
	drawLine(0x2588, offsetX, offsetY, offsetX, gameSizeY + offsetY);
	drawLine(0x2588, gameSizeX+1 + offsetX, offsetY, gameSizeX+1 + offsetX, gameSizeY + offsetY);
	drawLine(0x2588, offsetX, gameSizeY+1 + offsetY, gameSizeX+1 + offsetX, gameSizeY+1 + offsetY);
}

std::wstring getTetromino(int index, int rotation, bool inversion, wchar_t filler) {
	std::wstring result = L"................";
	for (int i = 0; i < 16; i++) {
		int x = i % 4;
		if (inversion) x = 3 - x;
		int y = i / 4;
		int j;
		switch (abs(rotation) % 4)
		{
		case 0:
			j = y * 4 + x;
			break;
		case 1:
			j = 12 - 4 * x + y;
			break;
		case 2:
			j = 15 - x - y * 4;
			break;
		case 3:
			j = 3 - y + x * 4;
			break;
		default:
			j = i;
			break;
		}
		result[i] = tetromino[index % 5][j];
		if (result[i] == '#') result[i] = filler;
		else result[i] = '\0';
	}
	return result;
}

bool checkTetrominoCollision(std::wstring tetromino, int x0, int y0) {
	x0 -= gamePosX;
	y0 -= gamePosY;
	for (int x = x0; x < x0 + 4; x++) {
		for (int y = y0; y < y0 + 4; y++) {
			if (tetromino[(y - y0) * 4 + x - x0] != '\0') {
				if (x < gamePosX-1 || x >= gameSizeX || y < gamePosY-2 || y >= gameSizeY)
					return true;
				else if (gameBuffer[y * gameSizeX + x] != ' ')
					return true;
			}
		}
	}
	return false;
}

void fillGameBuffer() {
	for (int i = 0; i < gameSizeX * gameSizeY; i++)
	gameBuffer += ' ';
}

void addToGameBuffer(std::wstring tetromino, int x0, int y0) {
	updatedLines.clear();
	for (int y = y0; y < y0 + 4; y++) {
		bool updated = false;
		for (int x = x0; x < x0 + 4; x++) {
			int toBuffer = (y - gamePosY) * gameSizeX + x - gamePosX;
			int fromTetromino = (y - y0) * 4 + x - x0;
			if (toBuffer >= 0 && toBuffer < gameSizeX * gameSizeY && x - gamePosX >= 0 && x - gamePosX <= gameSizeX - 1 && tetromino[fromTetromino] != '\0')
			{
				gameBuffer[toBuffer] = tetromino[fromTetromino];
				if (!updated) {
					updated = true;
					updatedLines.push_back(y - gamePosY);
				}
			}
		}
	}
}

void checkUpdatedLines() {
	for (int i = 0; i < updatedLines.size(); i++) {
		std::pair<bool, int> checker = checkClearLine(updatedLines[i]);
		if (checker.first) {
			clearedLines.push_back(checker.second);
			linesCleared++;
			for (int xClear = 0; xClear < gameSizeX; xClear++) {
				gameBuffer[checker.second * gameSizeX + xClear] = '=';
			}
		}
	}
}

int removeClearedLines() {
	int a = 0;
	for (int i = 0; i < clearedLines.size(); i++) {
		moveTetrisLayers(clearedLines[i]);
		a++;
	}
	clearedLines.clear();
	return a;
}

void drawGameBuffer(int x0, int y0) {
	gamePosX = x0;
	gamePosY = y0;
	fillImage(gameBuffer, x0, y0, gameSizeX, gameSizeY);
}

void drawPredictionTetromino() {
	int x = gamePosX + gameSizeX + 2;
	int y = gamePosY;
	drawFrameSolid(0x2588, 0x2588, x, y, x + 6, y + 6);
	fillImage(getTetromino(tetrostatusPred[0], 0, tetrostatusPred[1], fillers[tetrostatusPred[2]]), x + 1, y + 1, 4, 4);
}

void spawnTetromino(int& x, int& y, std::wstring& tetromino) {
	predictTetromino();
	tetromino = getTetromino(tetrostatus[0], 0, tetrostatus[1], fillers[tetrostatus[2]]);
	y = gamePosY+1;
	x = gamePosX + (gameSizeX / 2) - 2;
	if (checkTetrominoCollision(tetromino, x, y)) {
		tetrisLose();
	}
}
void fallTetromino(int& x, int& y, std::wstring& tetromino) {
	if (checkTetrominoCollision(tetromino, x, y + 1)) {
		addToGameBuffer(tetromino, x, y);
		spawnTetromino(x, y, tetromino);
	}
	else
		y = y + 1;
}

void moveTetromino(int& x, int& y, std::pair<int, int> direction, std::wstring& tetromino) {
	if (direction.second < 0) direction.second = 0;
	if (!checkTetrominoCollision(tetromino, x + direction.first, y + direction.second)) {
		x += direction.first;
		y += direction.second;
	}
}

void predictTetromino()
{
	tetrostatus[0] = tetrostatusPred[0];
	tetrostatus[1] = tetrostatusPred[1];
	tetrostatus[2] = tetrostatusPred[2];

	tetrostatusPred[0] = rand() % 5;
	tetrostatusPred[1] = rand() % 2;
	tetrostatusPred[2] = rand() % 3;
}

std::pair<bool, int> checkClearLine(int y) {
	bool connect = true;
	for (int x = 0; x < gameSizeX; x++) {
		if (gameBuffer[y * gameSizeX + x] == ' ')
			connect = false;
	}
	if (connect)
		return std::pair<bool, int> { true, y };
	else return std::pair<bool, int> { false, y };
}

void moveTetrisLayers(int y0) {
	if (y0 < gameSizeY && y0 >= 0)
	for (int y = y0; y > 0; y--) {
		for (int x = 0; x < gameSizeX; x++) {
			if (y - 1 >= 0)
				gameBuffer[y * gameSizeX + x] = gameBuffer[(y - 1) * gameSizeX + x];
		}
	}
}

void tetrisLose() {
	ongoingGame = false;
	int messageX = gamePosX + gameSizeX / 2 - 11 / 2;
	int messageY = gamePosY + gameSizeY / 2 - 2;
	drawLine(0x2550, messageX+1, messageY, messageX + 9, messageY);
	drawLine(0x2550, messageX + 1, messageY+2, messageX + 9, messageY+2);
	showString(L"\u2554\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2557", messageX, messageY);
	showString(L"\u255A\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u255D", messageX, messageY+2);
	showString(L"\u2551", messageX, messageY+1); showString(L"\u2551", messageX+10, messageY + 1);
	showString(L"GAME OVER", messageX+1, messageY+1);
}

void levelUp() {
	if (linesCleared >= level * 10) {
		linesCleared = 0;
		level++;
	}
}

std::pair<std::wstring, int> showScoreMessage(int action, int data) {
	std::pair<std::wstring, int> message;
	int messageLength;
	switch (action) {
	case 0: // CLEARING LINES; DATA IS AMOUNT OF LINES CLEARED
		switch (data) {
		case 1:
			message.first = L"SINGLE!";
			message.second = 100 * level;
			break;
		case 2:
			message.first = L"DOUBLE!";
			message.second = 300 * level;
			break;
		case 3:
			message.first = L"TRIPLE!";
			message.second = 500 * level;
			break;
		case 4:
			message.first = L"TETRIS!";
			message.second = 800 * level;
			break;
		}
	case 1: //TSPIN 
		break;
	}
	messageLength = message.first.size();
	int messageX = gamePosX + (gameSizeX/2) - (messageLength/2) - 2; // - (messageLength / 2);
	int messageY = gamePosY + 2;
	drawFrameWCorners(messageX, messageY, messageX + messageLength+1, messageY+3);
	showString(message.first, messageX+1, messageY+1);
	showString(L"+" + std::to_wstring(message.second), messageX + 2, messageY + 2);
	return message;
}