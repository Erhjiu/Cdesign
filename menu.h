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
	time_t lastPlayed;     // 上次游玩时间
	int playCount;	       // 游玩次数
	vector<string> tags;   // 标签

	// 构造函数
	GameInfo(string id, string title, string coverPath, string exePath, time_t lastPlayed, int playCount, vector<string> tags)
		: id(id), title(title), coverPath(coverPath), exePath(exePath), lastPlayed(lastPlayed), playCount(playCount), tags(tags) {
	}

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
		games.push_back(GameInfo("1", "游戏1", "cover1.jpg", "game1.exe", 0, 0, { "动作", "冒险" }));
		games.push_back(GameInfo("2", "游戏2", "cover2.jpg", "game2.exe", 0, 0, { "角色扮演", "冒险" }));
		games.push_back(GameInfo("3", "游戏3", "cover3.jpg", "game3.exe", 0, 0, { "策略", "模拟" }));
		games.push_back(GameInfo("4", "游戏4", "cover4.jpg", "game4.exe", 0, 0, { "射击", "动作" }));
		games.push_back(GameInfo("5", "游戏5", "cover5.jpg", "game5.exe", 0, 0, { "体育", "竞速" }));
		games.push_back(GameInfo("6", "游戏6", "cover6.jpg", "game6.exe", 0, 0, { "益智", "休闲" }));
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

	GameLauncherUI() {
		LoadSampleData();
	}
	void run();
};
