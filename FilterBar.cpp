#include "FilterBar.h"
#include "menu.h"
#include <easyx.h>
#include <vector>
#include <algorithm>
#include <unordered_map>

using namespace std;

void FilterBar::output()
{
	cout << "1";
}

bool compare(const pair<string, int> &a, const pair<string, int> &b)
{
	return a.second > b.second; // 降序排序
}

vector<string> FilterBar::loadTags(class GameLauncherUI &a)
{
	vector<string> categories;
	categories.push_back("ALL");
	unordered_map<string, int> countTags;
	for (const auto &game : a.allGames)
	{
		for (const auto &tag : game.tags)
		{
			auto it = countTags.find(tag);
			if (it == countTags.end())
			{
				countTags[tag] = 1;
			}
			else
			{
				countTags[tag]++;
			}
		}
	}
	// 将map中的内容转换为vector<pair<string, int>>，以便排序
	vector<pair<string, int>> Vec(countTags.begin(), countTags.end());

	// 按照出现次数从大到小排序
	sort(Vec.begin(), Vec.end(), compare);
	int Size = min(Vec.size(), 6);
	for (int i = 0; i < Size; i++)
	{
		pair<string, int> tagPair = Vec[i];
		categories.push_back(tagPair.first);
	}
	return categories;
}

vector<int> FilterBar::filter(GameLauncherUI &a, string target)
{
	vector<int> res;
	vector<vector<string>> tags;
	for (const auto &game : a.allGames)
	{
		tags.push_back(game.tags);
	}
	if (target == "全部")
	{
		for (int i = 0; i < tags.size(); i++)
		{
			res.push_back(i);
		}
		return res;
	}
	for (int i = 0; i < tags.size(); i++)
	{
		for (int j = 0; j < tags[i].size(); j++)
		{
			if (tags[i][j] == target)
			{
				res.push_back(i);
			}
		}
	}
	return res;
}

void FilterBar::Draw(GameLauncherUI &a)
{
	int x = 50;
	int y = 750;
	int width = 100;
	int height = 50;
	vector<string> categories = loadTags(a);
	settextstyle(16, 0, _T("Arial"));
	settextcolor(RGB(0, 0, 0));
	for (size_t i = 0; i < categories.size(); ++i)
	{
		setfillcolor(RGB(200, 200, 200));
		if (currentFilter == categories[i])
		{
			setfillcolor(RGB(100, 150, 255)); // 高亮颜色
		}
		a.DrawRoundRect(x + i * (width + 10), y, x + i * (width + 10) + width, y + height, 10, RGB(65, 70, 85), RGB(65, 70, 85));
		settextcolor(RGB(0, 0, 0));
		outtextxy(x + i * (width + 10) + 5, y + 15, categories[i].c_str());
	}
}
