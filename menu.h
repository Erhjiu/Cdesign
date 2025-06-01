#pragma once
#include"data.h"
#include "add.h"
#include <easyx.h>
#include <vector>
#include <string>
#include <ctime>
#include <iostream>

using namespace std;

class GameLauncherUI
{
private:
	vector<GameInfo> games;
	vector<GameInfo> allGames;
	UITheme theme;
	// 过滤器
	string filter = "全部";
	// 排列方式
	int selectedIndex = -1;
	int hoveredIndex = -1;
	bool showDetails = false;
	addButton* addBtn;
	size_t currentPage = 0;
	size_t pageSize = 6;

	void LoadSampleData();
	vector<GameInfo> GetPage(size_t pageIndex);
	bool LoadGames();
	// 绘制圆角矩形
	inline void DrawRoundRect(int x1, int y1, int x2, int y2, int radius, COLORREF fill, COLORREF outline);
public:
	friend void to_json(json& j, const GameInfo& game);
	friend void from_json(const json& j, GameInfo& game);
	void DrawGameCard(const GameInfo& game, int x, int y, int width, int height, bool isHovered, bool isSelected);
	//筛选
	void categroy();
	vector<GameInfo> GetCurrentPage();
	size_t pagePlus(size_t currentPage);
	size_t pageSub(size_t currentPage);

	//绘制详情面板
	void DrawDetailPanel(const GameInfo& game, const UITheme& theme);
	//绘制主界面
	void DrawMainView();

	void askGameInfo();

	GameLauncherUI(){
		LoadSampleData();
	}
	~GameLauncherUI() {
		if (addBtn) {
			delete addBtn;
			addBtn = nullptr;
		}
		games.clear();
		allGames.clear();
		selectedIndex = -1;
		hoveredIndex = -1;
		showDetails = false;
		currentPage = 0;
		pageSize = 6;
	}
	void run();
};
