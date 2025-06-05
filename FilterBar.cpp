#include "FilterBar.h"
#include "Library.h"
#include "json.hpp"
#include <easyx.h>
#include <vector>
#include <algorithm>
#include <unordered_map>

using namespace std;
const string savePath = "games.json";
using namespace std;
using json = nlohmann::json;

bool compare(const pair<string, int> &a, const pair<string, int> &b)
{
	return a.second > b.second; 
}

bool FilterBar::LoadGames()
{
	allGames.clear(); //清空之前的数据
	ifstream ifs(savePath, ios::binary);
	if (!ifs.is_open())
	{
		// cout <<"无法打开游戏数据文件，可能文件不存在或路径错误" << endl;
		return false;
	}
	try
	{
		ifs.seekg(0, ios::end);
		if (ifs.tellg() == 0)
		{
			// cout << "游戏数据文件为空，请添加游戏。" << endl;
			ifs.close();
			return false;
		}
		ifs.seekg(0);
		json j;
		ifs >> j;
		allGames = j.get<vector<GameInfo>>();
		return true;
	}
	catch (const json::parse_error &e)
	{
		cout << "JSON解析错误: " << e.what() << endl;
	}
	catch (const json::type_error &e)
	{
		cout << "JSON类型错误: " << e.what() << endl;
	}
	catch (const std::exception &e)
	{
		cout << "其他错误: " << e.what() << endl;
	}
	ifs.close();
	return false;
}
void FilterBar::LoadSampleData()
{
	if (!LoadGames())
	{
		cout << "加载游戏数据失败" << endl;
		allGames = {};
	}
}

vector<string> FilterBar::loadTags()
{
	vector<string> categories;
	categories.push_back("ALL");
	unordered_map<string, int> countTags;
	for (const auto &game : allGames)
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
	int Size = min(Vec.size(), 5);
	for (int i = 0; i < Size; i++)
	{
		pair<string, int> tagPair = Vec[i];
		categories.push_back(tagPair.first);
	}
	return categories;
}

void FilterBar::getFilterGames(string target)
{
	filterGames.clear(); // 先清空之前的筛选结果

	if (target == "ALL")
	{
		filterGames = allGames; // 如果是ALL，直接使用所有游戏
		return;					// 直接返回，不执行后面的代码
	}

	// 如果不是ALL，则根据标签筛选
	vector<int> gameIndices;
	for (int i = 0; i < allGames.size(); i++)
	{
		for (const auto &tag : allGames[i].tags)
		{
			if (tag == target)
			{
				gameIndices.push_back(i);
				break; // 找到一个匹配的标签就可以了，避免重复添加
			}
		}
	}

	// 根据索引添加游戏到筛选结果
	for (int index : gameIndices)
	{
		filterGames.push_back(allGames[index]);
	}
}
