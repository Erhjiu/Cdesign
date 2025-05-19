#pragma once
#include <graphics.h>
#include <vector>
#include <string>
#include <ctime>
using namespace std;

struct GameInfo {
	string id;
	string title;
	string coverPath;//封面
	string exePath;//游戏路径
	time_t lastPlayed = 0;//上次游玩时间
	int playCount = 0;//游玩次数
	vector<string> tags;//标签
	string GetLastPlayedStr()const {
		if (playCount == 0)return "从未玩过";
		char buf[64];
		tm timeinfo;
		localtime_s(&timeinfo, &lastPlayed);
		strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &timeinfo);
		return buf;
	}
};
// UI颜色主题
struct UITheme {
	COLORREF background = RGB(40, 42, 54);
	COLORREF cardBackground = RGB(65, 70, 85);
	COLORREF cardHover = RGB(85, 90, 105);
	COLORREF cardSelected = RGB(100, 110, 130);
	COLORREF text = RGB(240, 240, 240);
	COLORREF textSecondary = RGB(180, 180, 180);
	COLORREF accent = RGB(100, 180, 255);
};

class GameLauncherUI {
	public :
	vector<GameInfo> games;
	UITheme theme;
	int selectedIndex = -1;
	int hoveredIndex = -1;
	bool showDetails = false;

	// 绘制圆角矩形
	void DrawRoundRect(int x1, int y1, int x2, int y2, int radius, COLORREF fill, COLORREF outline) {
		setfillcolor(fill);
		fillroundrect(x1, y1, x2, y2, radius, radius);
		setlinecolor(outline);
		roundrect(x1, y1, x2, y2, radius, radius);
	}
	// 绘制游戏卡片
	//1:网格(默认)
	//2:列表
	//3：3D
	
	//排列
	//1:网格(默认)
	//2:列表
	//3：3D

	//筛选

	//添加游戏

	//绘制详情面板
	void DrawDetailPanel(const GameInfo& game, const UITheme& theme);
	//绘制主界面
	void DrawMainView(){}


	GameLauncherUI() {
		// 初始化游戏列表
		games.push_back({ "1", "Game 1", "cover1.jpg", "game1.exe", 0, 0, { "Action", "Adventure" } });
	}
	void run() {}
	
};
