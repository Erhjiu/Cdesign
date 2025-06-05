#include "add.h"

const string savePath = "games.json";
using namespace std;
using json = nlohmann::json;


void addButton::askGameInfo()
{
	id++;
	cout << "请输入游戏名称：" << endl;
	string title;
	cin >> title;
	cout << "请输入游戏封面路径：" << endl;
	string coverPath;
	cin >> coverPath;
	cout << "请输入游戏路径：" << endl;
	string exePath;
	cin >> exePath;
	int playCount = 0;
	cout << "请输入游戏标签：" << endl;
	vector<string> tags;
	string temptags;
	string temp;
	cin >> temptags;
	for (char c : temptags)
	{
		if (c == ',')
		{
			tags.push_back(temp);
			temp.clear();
		}
		else
		{
			temp += c;
		}
	}
	if (!temp.empty())
	{
		tags.push_back(temp);
	}
	GameInfo gameinfo;
	gameinfo.id = id;
	gameinfo.title = title;
	gameinfo.coverPath = coverPath;
	gameinfo.exePath = exePath;
	gameinfo.playCount = playCount;
	gameinfo.tags = tags;

	json allGames = json::array();
	ifstream ifs(savePath);
	if (ifs.is_open())
	{
		try
		{
			ifs >> allGames;
		}
		catch (...)
		{
			// 文件内容不是有效JSON，创建新数组
			allGames = json::array();
		}
		ifs.close();
	}
	json jGame = {
		{"id", gameinfo.id},
		{"title", gameinfo.title},
		{"coverPath", gameinfo.coverPath},
		{"exePath", gameinfo.exePath},
		{"lastPlayed", gameinfo.lastPlayed},
		{"playCount", gameinfo.playCount},
		{"tags", gameinfo.tags} };

	allGames.push_back(jGame);

	ofstream ofs(savePath);
	if (ofs.is_open())
	{
		ofs << allGames.dump(4); // 美化输出
		ofs.close();
	}
}