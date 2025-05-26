#include "add.h"
#include <iostream>
#include <vector>
using namespace std;

void addButton::Draw(const UITheme& theme,bool isHovered)
{
	// ����Բ�Ǿ��ΰ�ť
	COLORREF bg = isHovered ? theme.cardHover : theme.add; // �����ͣʱ����ɫ
	setfillcolor(bg);
	setlinecolor(RGB(100, 100, 100));  // �߿���ɫ
	fillroundrect(x, y, x + width, y + height, 10, 10);

	// ���� "+" ����
	setlinestyle(PS_SOLID, 3);         // ������ϸ
	setlinecolor(RGB(0, 0, 0));        // ������ɫ

	// ����
	line(x + width / 4, y + height / 2, x + width * 3 / 4, y + height / 2);
	// ����
	line(x + width / 2, y + height / 4, x + width / 2, y + height * 3 / 4);
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

void addButton::addRun(vector<GameInfo>& games) {
	ExMessage msg;
	if (peekmessage(&msg, EX_MOUSE)) {
		bool isHovered = (msg.x >= x && msg.x <= x + width && msg.y >= y && msg.y <= y + height);
		if (isHovered && msg.message == WM_LBUTTONDOWN) {
			GameInfo newGame = askGameInfo();
			if (!newGame.title.empty()) {
				games.push_back(newGame);
				cout << "��Ϸ�����: " << newGame.title << endl;
			}
		}
	}
}

bool addButton::checkHover(int mouseX, int mouseY) {
	return mouseX >= x && mouseX <= x + width &&
		mouseY >= y && mouseY <= y + height;
}