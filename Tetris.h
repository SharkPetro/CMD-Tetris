#pragma once
#include "General.h"
#include <string>
extern bool ongoingGame;
extern int tetrominosSpawned;
extern int linesCleared;
extern int level;
extern int score;
extern int gameSizeX;
extern int gameSizeY;
extern int gamePosX;
extern int gamePosY;
extern wchar_t fillers[5];
extern int tetrostatus[3];
extern std::vector<int> updatedLines;
extern std::vector<int> clearedLines;
extern std::wstring gameBuffer;

void tetrisDrawFrame(int offsetX = 0, int offsetY = 0);
std::wstring getTetromino(int index, int rotation, bool inversion, wchar_t filler);
bool checkTetrominoCollision(std::wstring tetromino, int x, int y);
void fillGameBuffer();
void addToGameBuffer(std::wstring tetromino, int x, int y);
void checkUpdatedLines();
int removeClearedLines();
void drawGameBuffer(int x0, int y0);
void drawPredictionTetromino();
void spawnTetromino(int& x, int& y, std::wstring& tetromino);
void fallTetromino(int& x, int& y, std::wstring& tetromino);
void moveTetromino(int& x, int& y, std::pair<int, int> direction, std::wstring& tetromino);
void predictTetromino();
std::pair<bool, int> checkClearLine(int y);
void moveTetrisLayers(int y);
void tetrisLose();

void levelUp();
std::pair<std::wstring, int> showScoreMessage(int action, int data);