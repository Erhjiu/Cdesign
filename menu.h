#pragma once
#include "data.h"
#include "add.h"
#include <easyx.h>
#include <vector>
#include <string>
#include <ctime>
#include <iostream>

// ǰ������
class FilterBar;

using namespace std;

class GameLauncherUI
{
private:
	vector<GameInfo> games;
	vector<GameInfo> allGames;
	UITheme theme;
	// ɸѡ��
	string filter = "ȫ��";
	// ѡ��ʽ
	int selectedIndex = -1;
	int hoveredIndex = -1;
	bool showDetails = false;
	addButton *addBtn;
	size_t currentPage = 0;
	size_t pageSize = 6;
	FilterBar *filterBar; // ���FilterBarָ���Ա

	void LoadSampleData();
	vector<GameInfo> GetPage(size_t pageIndex);
	bool LoadGames();
	friend class FilterBar;

public:
	// ����Բ�Ǿ���
	inline void DrawRoundRect(int x1, int y1, int x2, int y2, int radius, COLORREF fill, COLORREF outline);
	void DrawGameCard(const GameInfo &game, int x, int y, int width, int height, bool isHovered, bool isSelected);
	// ɸѡ
	void categroy();
	vector<GameInfo> GetCurrentPage();
	size_t pagePlus(size_t currentPage);
	size_t pageSub(size_t currentPage);

	// �����������
	void DrawDetailPanel(const GameInfo &game, const UITheme &theme);
	// ��������ͼ
	void DrawMainView();

	void askGameInfo();

	GameLauncherUI();
	~GameLauncherUI();
	void run();
};
