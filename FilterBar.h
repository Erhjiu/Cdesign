#pragma once
#include "data.h"
#include <easyx.h>
#include <vector>
#include <string>

using namespace std;
class FilterBar
{
private:
	vector<GameInfo> allGames;
	string currentFilter = "ALL";
	void LoadSampleData();

public:
	int x = 50;
	int y = 750;
	int width = 100;
	int height = 30;
	int tagIndex = 0;
	int currentPage = 1;		  // 添加当前页码
	bool isDoubleClicked = false; // 添加双击状态标记
	vector<string> currentTags;
	vector<string> loadTags();
	vector<GameInfo> filterGames;
	void getFilterGames(string target);
	bool LoadGames();
	FilterBar()
	{
		LoadSampleData();
		getFilterGames("ALL"); // 初始化时加载所有游戏
	}
};
