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
	// ������
	string filter = "ȫ��";
	// ���з�ʽ
	int selectedIndex = -1;
	int hoveredIndex = -1;
	bool showDetails = false;
	addButton* addBtn;
	size_t currentPage = 0;
	size_t pageSize = 6;

	void LoadSampleData();
	vector<GameInfo> GetPage(size_t pageIndex);
	bool LoadGames();
	// ����Բ�Ǿ���
	inline void DrawRoundRect(int x1, int y1, int x2, int y2, int radius, COLORREF fill, COLORREF outline);
public:
	friend void to_json(json& j, const GameInfo& game);
	friend void from_json(const json& j, GameInfo& game);
	void DrawGameCard(const GameInfo& game, int x, int y, int width, int height, bool isHovered, bool isSelected);
	//ɸѡ
	void categroy();
	vector<GameInfo> GetCurrentPage();
	size_t pagePlus(size_t currentPage);
	size_t pageSub(size_t currentPage);

	//�����������
	void DrawDetailPanel(const GameInfo& game, const UITheme& theme);
	//����������
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
