#include "add.h"
#include <iostream>
#include <vector>
using namespace std;

void addButton::Draw(const UITheme& theme)
{
	// ����Բ�Ǿ��ΰ�ť
	setfillcolor(theme.add);
	setlinecolor(RGB(100, 100, 100));  // �߿���ɫ
	solidcircle(x + width/2, y + height/2,25);

	// ���� "+" ����
	setlinestyle(PS_SOLID, 3);         // ������ϸ
	setlinecolor(RGB(0, 0, 0));        // ������ɫ

	// ����
	line(x + width / 4, y + height / 2, x + width * 3 / 4, y + height / 2);
	// ����
	line(x + width / 2, y + height / 4, x + width / 2, y + height * 3 / 4);
}

bool addButton::checkClick(int mouseX, int mouseY) {
	return mouseX >= x && mouseX <= x + width &&
		mouseY >= y && mouseY <= y + height;
}

