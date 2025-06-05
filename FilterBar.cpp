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
	allGames.clear(); //���֮ǰ������
	ifstream ifs(savePath, ios::binary);
	if (!ifs.is_open())
	{
		// cout <<"�޷�����Ϸ�����ļ��������ļ������ڻ�·������" << endl;
		return false;
	}
	try
	{
		ifs.seekg(0, ios::end);
		if (ifs.tellg() == 0)
		{
			// cout << "��Ϸ�����ļ�Ϊ�գ��������Ϸ��" << endl;
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
		cout << "JSON��������: " << e.what() << endl;
	}
	catch (const json::type_error &e)
	{
		cout << "JSON���ʹ���: " << e.what() << endl;
	}
	catch (const std::exception &e)
	{
		cout << "��������: " << e.what() << endl;
	}
	ifs.close();
	return false;
}
void FilterBar::LoadSampleData()
{
	if (!LoadGames())
	{
		cout << "������Ϸ����ʧ��" << endl;
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
	// ��map�е�����ת��Ϊvector<pair<string, int>>���Ա�����
	vector<pair<string, int>> Vec(countTags.begin(), countTags.end());

	// ���ճ��ִ����Ӵ�С����
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
	filterGames.clear(); // �����֮ǰ��ɸѡ���

	if (target == "ALL")
	{
		filterGames = allGames; // �����ALL��ֱ��ʹ��������Ϸ
		return;					// ֱ�ӷ��أ���ִ�к���Ĵ���
	}

	// �������ALL������ݱ�ǩɸѡ
	vector<int> gameIndices;
	for (int i = 0; i < allGames.size(); i++)
	{
		for (const auto &tag : allGames[i].tags)
		{
			if (tag == target)
			{
				gameIndices.push_back(i);
				break; // �ҵ�һ��ƥ��ı�ǩ�Ϳ����ˣ������ظ����
			}
		}
	}

	// �������������Ϸ��ɸѡ���
	for (int index : gameIndices)
	{
		filterGames.push_back(allGames[index]);
	}
}
