#pragma once
#include "menu.h"
#include <easyx.h>
#include <vector>

using namespace std;

class dropDown
{
private:
	int x, y, width, height;
	bool isOpen = false;
	vector<string> options = {"Íø¸ñ","ÁÐ±í","3D"};
	int selectedIndex = 0;
public:
	dropDown(int x, int y, int width, int height) : x(x), y(y), width(width), height(height) {}
	void Draw(const UITheme& theme);
	void clickDropDown(int mouseX, int mouseY,dropDown& dropDownMenu);
	void Toggle()
	{
		isOpen = !isOpen;
	}
	void SelectOption(int index)
	{
		if (index >= 0 && index < options.size())
			selectedIndex = index;
		isOpen = false;
	}
};