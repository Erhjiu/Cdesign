#pragma once
#include <easyx.h>
#include <vector>
#include <string>

class GameLauncherUI;

using namespace std;
class FilterBar
{
private:
	string currentFilter = "È«²¿";
	void Draw(GameLauncherUI& a);
	void output();
public:
	vector<string> loadTags(GameLauncherUI &a);
	vector<int> filter(GameLauncherUI& a, string target);
	FilterBar(GameLauncherUI &a)
	{
		Draw(a);
		output();
		vector<int> b = filter(a, currentFilter);
	}
	void handleMouseClick(int x, int y);
};
