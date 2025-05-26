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
	time_t lastPlayed = 0;     // 上次游玩时间
	int playCount = 0;	       // 游玩次数
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
	COLORREF add = RGB(255, 255, 255);
};

class GameLauncherUI
{
private:
	vector<GameInfo> games;
	UITheme theme;
	// 过滤器
	string filter = "全部";
	// 排列方式
	int selectedIndex = -1;
	int hoveredIndex = -1;
	bool showDetails = false;

	void LoadSampleData() {
		games.push_back({ "1", "游戏1", "cover1.jpg", "game1.exe", time(nullptr), 5, {"动作", "冒险"} });
		games.push_back({ "2", "游戏2", "cover2.jpg", "game2.exe", time(nullptr), 3, {"角色扮演"} });
		games.push_back({ "3", "游戏3", "cover3.jpg", "game3.exe", time(nullptr), 10, {"策略"} });
		games.push_back({ "4", "游戏4", "cover4.jpg", "game4.exe", time(nullptr), 0, {"休闲"} });
		games.push_back({ "5", "游戏5", "cover5.jpg", "game5.exe", time(nullptr), 7, {"动作"} });
	}


	// 绘制圆角矩形
	inline void DrawRoundRect(int x1, int y1, int x2, int y2, int radius, COLORREF fill, COLORREF outline) {
		setfillcolor(fill);
		fillroundrect(x1, y1, x2, y2, radius, radius);
		setlinecolor(outline);
		roundrect(x1, y1, x2, y2, radius, radius);
	}
public:
	void DrawGameCard(const GameInfo& game, int x, int y, int width, int height, bool isHovered, bool isSelected);
	//筛选
	void categroy();
	//绘制详情面板
	void DrawDetailPanel(const GameInfo& game, const UITheme& theme);
	//绘制主界面
	void DrawMainView();

	GameLauncherUI(){
		LoadSampleData();
	}
	void run();
};
