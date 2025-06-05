#pragma once
#include "data.h"
#include "add.h"
#include <easyx.h>
#include <vector>
#include <string>
#include <ctime>
#include <iostream>

// 前向声明
class FilterBar;

using namespace std;

class GameLauncherUI
{
private:
	vector<GameInfo> games;
	vector<GameInfo> allGames;
	UITheme theme;
	// 筛选器
	string filter = "全部";
	// 选择方式
	int selectedIndex = -1;
	int hoveredIndex = -1;
	bool showDetails = false;
	addButton *addBtn;
	size_t currentPage = 0;
	size_t pageSize = 6;
	FilterBar *filterBar; // 添加FilterBar指针成员

	void LoadSampleData();
	vector<GameInfo> GetPage(size_t pageIndex);
	bool LoadGames();
	friend class FilterBar;

public:
	// 绘制圆角矩形
	inline void DrawRoundRect(int x1, int y1, int x2, int y2, int radius, COLORREF fill, COLORREF outline);
	void DrawGameCard(const GameInfo &game, int x, int y, int width, int height, bool isHovered, bool isSelected);
	// 筛选
	void categroy();
	vector<GameInfo> GetCurrentPage();
	size_t pagePlus(size_t currentPage);
	size_t pageSub(size_t currentPage);

	// 绘制详情面板
	void DrawDetailPanel(const GameInfo &game, const UITheme &theme);
	// 绘制主视图
	void DrawMainView();

	void askGameInfo();

	GameLauncherUI();
	~GameLauncherUI();
	void run();
};
