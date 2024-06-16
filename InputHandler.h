#pragma once
#include <utility>

extern std::pair<int, int> LStickInput;
extern std::pair<int, int> RStickInput;

extern std::pair<int, int> LPadInput;
extern std::pair<int, int> RPadInput;

void setAxis(char l, char r, char u, char d, std::pair<int, int> &axis);

bool getKey(char k);
bool getKeyType(char k);