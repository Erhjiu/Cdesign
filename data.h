#pragma once
#include "json.hpp"
#include <easyx.h>
#include <vector>
#include <string>
#include <ctime>
#include <fstream>
#include <codecvt>
#include <locale>

using json = nlohmann::json;
using namespace std;

struct GameInfo
{
	int id;
	string title;
	string coverPath;	   // 封面
	string exePath;		   // 游戏路径
	time_t lastPlayed = 0; // 上次游玩时间
	int playCount = 0;	   // 游玩次数
	vector<string> tags;   // 标签

	string GetLastPlayedStr() const
	{
		if (playCount == 0)
			return "从未玩过";
		char buf[64];
		tm timeinfo;
		localtime_s(&timeinfo, &lastPlayed);
		strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &timeinfo);
		return buf;
	}
};
// UI颜色主题
struct UITheme
{
	COLORREF background = RGB(40, 42, 54);
	COLORREF cardBackground = RGB(65, 70, 85);
	COLORREF cardHover = RGB(85, 90, 105);
	COLORREF cardSelected = RGB(100, 110, 130);
	COLORREF text = RGB(240, 240, 240);
	COLORREF textSecondary = RGB(180, 180, 180);
	COLORREF accent = RGB(100, 180, 255);
	COLORREF add = RGB(255, 255, 255);
};

inline void to_json(json &j, const GameInfo &game)
{
	j = json{
		{"id", game.id},
		{"title", game.title},
		{"coverPath", game.coverPath},
		{"exePath", game.exePath},
		{"lastPlayed", game.lastPlayed},
		{"playCount", game.playCount},
		{"tags", game.tags}};
}

inline void from_json(const json &j, GameInfo &game)
{
	j.at("id").get_to(game.id);
	j.at("title").get_to(game.title);
	j.at("coverPath").get_to(game.coverPath);
	j.at("exePath").get_to(game.exePath);
	j.at("lastPlayed").get_to(game.lastPlayed);
	j.at("playCount").get_to(game.playCount);
	j.at("tags").get_to(game.tags);
}
