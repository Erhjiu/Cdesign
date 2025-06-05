#pragma once
#include <easyx.h>
#include <vector>
#include <string>

class GameLauncherUI;

using namespace std;
class FilterBar
{
private:
	string currentFilter = "全部";
	void output();

public:
	vector<string> loadTags(GameLauncherUI &a);
	vector<int> filter(GameLauncherUI &a, string target);
	FilterBar(GameLauncherUI &a)
	{
		vector<int> b = filter(a, currentFilter);
	}
	void Draw(GameLauncherUI& a);
	void handleMouseClick(int x, int y);
};
