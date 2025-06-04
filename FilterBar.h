#pragma once
#include <easyx.h>
#include <vector>
#include <string>

class GameLauncherUI;

using namespace std;
class FilterBar
{
private:
	vector<std::string> categories;
	string currentFilter = "È«²¿";
	void loadTags(GameLauncherUI &a);
	void Draw();
	void output();
public:
	FilterBar(GameLauncherUI &a)
	{
		loadTags(a);
		Draw();
		output();
	}
	void handleMouseClick(int x, int y);
};
