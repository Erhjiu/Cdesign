#include "FilterBar.h"
#include "menu.h"
#include <easyx.h>
#include <vector>

using namespace std; 

void FilterBar::output() {
	cout << "1";
}

void FilterBar::loadTags(class GameLauncherUI& a)
{
	categories.clear();
	categories.push_back("全部");
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
}

void FilterBar::Draw() {
	int x = 50;
	int y = 750;
	int width = 50;
	int height = 50;
	settextstyle(16, 0, _T("微软雅黑"));
	settextcolor(RGB(0, 0, 0));
	for (size_t i = 0; i < categories.size(); ++i) {
		setfillcolor(RGB(200, 200, 200));
		if (currentFilter == categories[i]) {
			setfillcolor(RGB(100, 150, 255)); // 高亮颜色
		}
		fillrectangle(x + i * (width + 10), y, x + i * (width + 10) + width, y + height);
		settextcolor(RGB(0, 0, 0));
		outtextxy(x + i * (width + 10) + 5, y + 15, categories[i].c_str());
	}
}