#include "add.h"
#include <iostream>
#include <vector>
using namespace std;

void addButton::Draw(const UITheme& theme,bool isHovered)
{
	// 绘制圆角矩形按钮
	COLORREF bg = isHovered ? theme.cardHover : theme.add; // 鼠标悬停时的颜色
	setfillcolor(bg);
	setlinecolor(RGB(100, 100, 100));  // 边框颜色
	fillroundrect(x, y, x + width, y + height, 10, 10);

	// 绘制 "+" 符号
	setlinestyle(PS_SOLID, 3);         // 线条粗细
	setlinecolor(RGB(0, 0, 0));        // 符号颜色

	// 横线
	line(x + width / 4, y + height / 2, x + width * 3 / 4, y + height / 2);
	// 竖线
	line(x + width / 2, y + height / 4, x + width / 2, y + height * 3 / 4);
}

GameInfo addButton::askGameInfo() {
	cout << "请输入游戏id：" << endl;
	string id;
	cin >> id;
	cout << "请输入游戏名称：" << endl;
	string title;
	cin >> title;
	cout << "请输入游戏封面路径：" << endl;
	string coverPath;
	cin >> coverPath;
	cout << "请输入游戏路径：" << endl;
	string exePath;
	cin >> exePath; 
	cout << "请输入游玩次数：" << endl;
	int playCount;
	cin >> playCount;
	cout << "请输入游戏标签：" << endl;
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
				cout << "游戏已添加: " << newGame.title << endl;
			}
		}
	}
}

bool addButton::checkHover(int mouseX, int mouseY) {
	return mouseX >= x && mouseX <= x + width &&
		mouseY >= y && mouseY <= y + height;
}