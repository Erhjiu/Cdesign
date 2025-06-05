#pragma once
#include "data.h"
#include <easyx.h>
#include <vector>

class addButton
{
private:
	int x, y, width, height;

public:
	addButton() : x(0), y(0), width(50), height(50) {};
	addButton(int x, int y, int width, int height)
		: x(x), y(y), width(width), height(height) {}
	void Draw(const UITheme &theme);
	bool checkClick(int mouseX, int mouseY);
};