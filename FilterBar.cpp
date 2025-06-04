#include "FilterBar.h"
#include "menu.h"
#include <easyx.h>
#include <vector>

using namespace std; 

void FilterBar::output() {
	cout << "1";
}

vector<string> FilterBar::loadTags(class GameLauncherUI& a)
{
	vector<string> categories;
	categories.push_back("ȫ��");
	for (const auto& game : a.allGames)
	{
		for (const auto& tag : game.tags)
		{
			if (find(categories.begin(), categories.end(), tag) == categories.end())
			{
				categories.push_back(tag);
			}
		}
	}
	return categories;
}

vector<int>  FilterBar::filter(GameLauncherUI& a, string target) {
	vector<int> res;
	vector<vector<string>> tags;
	for (const auto& game : a.allGames) {
		tags.push_back(game.tags);
	}
	if (target == "ȫ��") {
		for (int i = 0; i < tags.size(); i++) {
			res.push_back(i);
		}
		return res;
	}
	for (int i = 0; i < tags.size(); i++) {
		for (int j = 0; j < tags[i].size(); j++) {
			if (tags[i][j] == target) {
				res.push_back(i);
			}
		}
	}
	return res;
}


void FilterBar::Draw(GameLauncherUI& a) {
	int x = 50;
	int y = 750;
	int width = 50;
	int height = 50;
	vector<string> categories = loadTags(a);
	settextstyle(16, 0, _T("΢���ź�"));
	settextcolor(RGB(0, 0, 0));
	for (size_t i = 0; i < categories.size(); ++i) {
		setfillcolor(RGB(200, 200, 200));
		if (currentFilter == categories[i]) {
			setfillcolor(RGB(100, 150, 255)); // ������ɫ
		}
		fillrectangle(x + i * (width + 10), y, x + i * (width + 10) + width, y + height);
		settextcolor(RGB(0, 0, 0));
		outtextxy(x + i * (width + 10) + 5, y + 15, categories[i].c_str());
	}
}


                   