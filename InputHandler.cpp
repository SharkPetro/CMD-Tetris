#include "InputHandler.h"
#include <Windows.h>

extern std::pair<int, int> LStickInput = {0, 0};
extern std::pair<int, int> RStickInput = { 0, 0 };

extern std::pair<int, int> LPadInput = { 0, 0 };
extern std::pair<int, int> RPadInput = { 0, 0 };

//if (GetAsyncKeyState('W') & 0b1)
void setAxis(char l, char r, char u, char d, std::pair<int, int> &axis) {
	axis.first = 0;
	axis.second = 0;
	if (GetAsyncKeyState(l)) {
		axis.first -= 1;
	}
	if (GetAsyncKeyState(r)) {
		axis.first += 1;
	}
	if (GetAsyncKeyState(u)) {
		axis.second += 1;
	}
	if (GetAsyncKeyState(d)) {
		axis.second -= 1;
	}
}

bool getKey(char k) {
	if (GetAsyncKeyState(k)) {
		return true;
	}
	return false;
}

bool getKeyType(char k) {
	if (GetAsyncKeyState(k) & 0b1) {
		return true;
	}
	return false;
}