#include "add.h"

const string savePath = "games.json";
using namespace std;
using json = nlohmann::json;

void addButton::askGameInfo()
{
	id++;
	cout << "��������Ϸ���ƣ�" << endl;
	string title;
	cin >> title;
	cout << "��������Ϸ����·����" << endl;
	string coverPath;
	cin >> coverPath;
	cout << "��������Ϸ·����" << endl;
	string exePath;
	cin >> exePath;
	int playCount = 0;
	cout << "��������Ϸ��ǩ��" << endl;
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
			// �ļ����ݲ�����ЧJSON������������
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
		{"tags", gameinfo.tags}};

	allGames.push_back(jGame);

	ofstream ofs(savePath);
	if (ofs.is_open())
	{
		ofs << allGames.dump(4); // �������
		ofs.close();
	}
}