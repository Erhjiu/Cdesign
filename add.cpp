#include "add.h"

void addButton::Draw(const UITheme& theme)
{
	setfillcolor(theme.add);
	fillrectangle(x, y + height / 3, x+width, y + height * 2 / 3);
	fillrectangle(x + width / 3, y, x + width * 2 / 3, y + height);
}

void addButton::clickAddButton(int mouseX, int mouseY) {
	if (mouseX >= x && mouseX <= x + width && mouseY >= y && mouseY <= y + height) {
		isOpen = !isOpen;
	}
}

void addButton::addDetailPanel(bool isOpen, const GameInfo& game, const UITheme& theme) {

}