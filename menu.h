#pragma once
#include <easyx.h>
#include <vector>
#include <string>
#include <ctime>
using namespace std;

struct GameInfo
{
	string id;
	string title;
	string coverPath;	   // 封面
	string exePath;		   // 游戏路径
	time_t lastPlayed = 0; // 上次游玩时间
	int playCount = 0;	   // 游玩次数
	vector<string> tags;   // 标签
	string GetLastPlayedStr() const
	{
		if (playCount == 0)
			return "从未玩过";
		char buf[64];
		tm timeinfo;
		localtime_s(&timeinfo, &lastPlayed);
		strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &timeinfo);
		return buf;
	}
};
// UI颜色主题
struct UITheme
{
	COLORREF background = RGB(40, 42, 54);
	COLORREF cardBackground = RGB(65, 70, 85);
	COLORREF cardHover = RGB(85, 90, 105);
	COLORREF cardSelected = RGB(100, 110, 130);
	COLORREF text = RGB(240, 240, 240);
	COLORREF textSecondary = RGB(180, 180, 180);
	COLORREF accent = RGB(100, 180, 255);
};

class GameLauncherUI
{
private:
	vector<GameInfo> games;
	UITheme theme;
	// 过滤器
	string filter = "全部";
	// 排列方式
	int layout = 1; // 1:网格, 2:列表, 3:3D
	int selectedIndex = -1;
	int hoveredIndex = -1;
	bool showDetails = false;

	// 绘制圆角矩形
	inline void DrawRoundRect(int x1, int y1, int x2, int y2, int radius, COLORREF fill, COLORREF outline) {
		setfillcolor(fill);
		fillroundrect(x1, y1, x2, y2, radius, radius);
		setlinecolor(outline);
		roundrect(x1, y1, x2, y2, radius, radius);
	}

	// 获取鼠标悬停的游戏索引（目前是默认网格，需要后期更改）
	inline int GetHoveredIndex(int mouseX, int mouseY) {
		const int cols = 3;
		const int cardWith = 220, cardHeight = 280;
		const int spacing = 30;
		const int startX = 50, startY = 100;

		for (int i = 0; i < games.size(); i++) {
			int row = i / cols;
			int col = i % cols;
			int x = startX + col * (cardWith + spacing);
			int y = startY + row * (cardHeight + spacing);

			if (mouseX >= x && mouseX <= x + cardWith && mouseY >= y && mouseY <= y + cardHeight) {
				return i;
			}
		}
		return -1;
	}
public:
	//筛选
	void categroy();
	//绘制详情面板
	void DrawDetailPanel(const GameInfo& game, const UITheme& theme);
	//绘制主界面
	void DrawMainView();

	GameLauncherUI() {
		// 初始化游戏信息
		GameInfo game1 = { "1", "Game 1", "cover1.jpg", "game1.exe", time(0), 5, {"Action", "Adventure"} };
		GameInfo game2 = { "2", "Game 2", "cover2.jpg", "game2.exe", time(0), 3, {"Puzzle", "Strategy"} };
		games.push_back(game1);
		games.push_back(game2);
	}
	void run();
	
};
