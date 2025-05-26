#pragma once
#include <easyx.h>
#include <vector>
#include <string>
#include <ctime>
using namespace std;

struct GameInfo
{
	string id;
	string title;
	string coverPath;	   // ����
	string exePath;		   // ��Ϸ·��
	time_t lastPlayed = 0;     // �ϴ�����ʱ��
	int playCount = 0;	       // �������
	vector<string> tags;   // ��ǩ

	string GetLastPlayedStr() const
	{
		if (playCount == 0)
			return "��δ���";
		char buf[64];
		tm timeinfo;
		localtime_s(&timeinfo, &lastPlayed);
		strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &timeinfo);
		return buf;
	}
};
// UI��ɫ����
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

class GameLauncherUI
{
private:
	vector<GameInfo> games;
	UITheme theme;
	// ������
	string filter = "ȫ��";
	// ���з�ʽ
	int selectedIndex = -1;
	int hoveredIndex = -1;
	bool showDetails = false;

	void LoadSampleData() {
		games.push_back({ "1", "��Ϸ1", "cover1.jpg", "game1.exe", time(nullptr), 5, {"����", "ð��"} });
		games.push_back({ "2", "��Ϸ2", "cover2.jpg", "game2.exe", time(nullptr), 3, {"��ɫ����"} });
		games.push_back({ "3", "��Ϸ3", "cover3.jpg", "game3.exe", time(nullptr), 10, {"����"} });
		games.push_back({ "4", "��Ϸ4", "cover4.jpg", "game4.exe", time(nullptr), 0, {"����"} });
		games.push_back({ "5", "��Ϸ5", "cover5.jpg", "game5.exe", time(nullptr), 7, {"����"} });
	}


	// ����Բ�Ǿ���
	inline void DrawRoundRect(int x1, int y1, int x2, int y2, int radius, COLORREF fill, COLORREF outline) {
		setfillcolor(fill);
		fillroundrect(x1, y1, x2, y2, radius, radius);
		setlinecolor(outline);
		roundrect(x1, y1, x2, y2, radius, radius);
	}
public:
	void DrawGameCard(const GameInfo& game, int x, int y, int width, int height, bool isHovered, bool isSelected);
	//ɸѡ
	void categroy();
	//�����������
	void DrawDetailPanel(const GameInfo& game, const UITheme& theme);
	//����������
	void DrawMainView();

	GameLauncherUI(){
		LoadSampleData();
	}
	void run();
};
