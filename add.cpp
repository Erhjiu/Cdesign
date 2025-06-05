#include "add.h"
#include <iostream>
#include <vector>
using namespace std;

void addButton::Draw(const UITheme& theme)
{
	// »æÖÆÔ²½Ç¾ØÐÎ°´Å¥
	setfillcolor(theme.add);
	setlinecolor(RGB(100, 100, 100));  // ±ß¿òÑÕÉ«
	solidcircle(x + width/2, y + height/2,25);

	// »æÖÆ "+" ·ûºÅ
	setlinestyle(PS_SOLID, 3);         // ÏßÌõ´ÖÏ¸
	setlinecolor(RGB(0, 0, 0));        // ·ûºÅÑÕÉ«

	// ºáÏß
	line(x + width / 4, y + height / 2, x + width * 3 / 4, y + height / 2);
	// ÊúÏß
	line(x + width / 2, y + height / 4, x + width / 2, y + height * 3 / 4);
}

bool addButton::checkClick(int mouseX, int mouseY) {
	return mouseX >= x && mouseX <= x + width &&
		mouseY >= y && mouseY <= y + height;
}

