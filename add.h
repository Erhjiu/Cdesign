//ÃÌº””Œœ∑
#pragma once
#include "menu.h"
#include <easyx.h>

class addButton {
private:
	int x, y, width, height;
	bool isOpen = false;
public:
	addButton(int x, int y, int width, int height)
		: x(x), y(y), width(width), height(height) {}
	void Draw(const UITheme& theme);
	void clickAddButton(int mouseX, int mouseY);
	void addDetailPanel(bool isOpen, const GameInfo& game, const UITheme& theme);
	void addGame(vector<GameInfo> games);
};