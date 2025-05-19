#pragma once
#include <graphics.h>
#include <vector>
#include <string>
#include <ctime>
using namespace std;

struct GameInfo {
	string id;
	string title;
	string coverPath;//����
	string exePath;//��Ϸ·��
	time_t lastPlayed = 0;//�ϴ�����ʱ��
	int playCount = 0;//�������
	vector<string> tags;//��ǩ
	string GetLastPlayedStr()const {
		if (playCount == 0)return "��δ���";
		char buf[64];
		tm timeinfo;
		localtime_s(&timeinfo, &lastPlayed);
		strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &timeinfo);
		return buf;
	}
};
// UI��ɫ����
struct UITheme {
	COLORREF background = RGB(40, 42, 54);
	COLORREF cardBackground = RGB(65, 70, 85);
	COLORREF cardHover = RGB(85, 90, 105);
	COLORREF cardSelected = RGB(100, 110, 130);
	COLORREF text = RGB(240, 240, 240);
	COLORREF textSecondary = RGB(180, 180, 180);
	COLORREF accent = RGB(100, 180, 255);
};

class GameLauncherUI {
	public :
	vector<GameInfo> games;
	UITheme theme;
	int selectedIndex = -1;
	int hoveredIndex = -1;
	bool showDetails = false;

	// ����Բ�Ǿ���
	void DrawRoundRect(int x1, int y1, int x2, int y2, int radius, COLORREF fill, COLORREF outline) {
		setfillcolor(fill);
		fillroundrect(x1, y1, x2, y2, radius, radius);
		setlinecolor(outline);
		roundrect(x1, y1, x2, y2, radius, radius);
	}
	// ������Ϸ��Ƭ
	//1:����(Ĭ��)
	//2:�б�
	//3��3D
	
	//����
	//1:����(Ĭ��)
	//2:�б�
	//3��3D

	//ɸѡ

	//�����Ϸ

	//�����������
	void DrawDetailPanel(const GameInfo& game, const UITheme& theme);
	//����������
	void DrawMainView(){}


	GameLauncherUI() {
		// ��ʼ����Ϸ�б�
		games.push_back({ "1", "Game 1", "cover1.jpg", "game1.exe", 0, 0, { "Action", "Adventure" } });
	}
	void run() {}
	
};
