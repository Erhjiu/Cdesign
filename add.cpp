#include "add.h"
#include <iostream>
#include <vector>
using namespace std;
static bool isOpen = false;

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

GameInfo addButton::askGameInfo() {
	cout << "��������Ϸid��" << endl;
	string id;
	cin >> id;
	cout << "��������Ϸ���ƣ�" << endl;
	string title;
	cin >> title;
	cout << "��������Ϸ����·����" << endl;
	string coverPath;
	cin >> coverPath;
	cout << "��������Ϸ·����" << endl;
	string exePath;
	cin >> exePath; 
	cout << "���������������" << endl;
	int playCount;
	cin >> playCount;
	cout << "��������Ϸ��ǩ��" << endl;
	vector<string> tags;
	string temptags;
	string temp;
	cin >> temptags;
	for (char c : temptags) {
		if (c == ',') {
			tags.push_back(temp);
			temp.clear();
		}
		else {
			temp += c;
		}
	}
	if (!temp.empty()) {
		tags.push_back(temp);
	}
	GameInfo gameinfo;
	gameinfo.id = id;
	gameinfo.title = title;
	gameinfo.coverPath = coverPath;
	gameinfo.exePath = exePath;
	gameinfo.playCount = playCount;
	gameinfo.tags = tags;
	return gameinfo;
}

void addButton::addDetailPanel(bool isOpen,  const UITheme& theme) {

}