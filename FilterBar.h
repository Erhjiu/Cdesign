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
    int height = 50;
	int tagIndex = 0;
	vector<string> currentTags;
	vector<string> loadTags();
	vector<GameInfo> filterGames;
	void getFilterGames( string target);
	bool LoadGames();
	FilterBar() {
		LoadSampleData();
	}
};
