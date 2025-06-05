#pragma once
#include "data.h"
#include "FilterBar.h"
#include <easyx.h>
#include <vector>
#include <string>
#include <ctime>
#include <iostream>


using namespace std;

class GameLauncherUI
{
private:
	vector<GameInfo> GetPage(size_t pageIndex);
public:
	vector<GameInfo> games;
	size_t currentPage = 0;
	size_t pageSize = 6;
	vector<GameInfo> GetCurrentPage();
	size_t pagePlus(size_t currentPage);
	size_t pageSub(size_t currentPage);
	FilterBar filterBar;
	GameLauncherUI() {
		games = GetCurrentPage();
	}
};
