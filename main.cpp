#include "data.h"
#include "json.hpp"

#include "add.h"
#include "FilterBar.h"
#include "Library.h"

#include <algorithm>
#include <easyx.h>
#include <fstream>
#include <iostream>
#include <shellapi.h>
#include <sstream>
#include <vector>

UITheme theme;

const string savePath = "games.json";
using namespace std;
using json = nlohmann::json;

namespace CardLayout
{
	constexpr int COLS = 3;
	constexpr int CARD_WIDTH = 220;
	constexpr int CARD_HEIGHT = 280;
	constexpr int SPACING = 30;
	constexpr int START_X = 50;
	constexpr int START_Y = 100;
	constexpr double COVER_HEIGHT_RATIO = 0.7; // ����ͼƬ�߶�ռ��Ƭ�߶ȵı���
}

// Բ�Ǿ���
void DrawRoundRect(int x1, int y1, int x2, int y2, int radius, COLORREF fill, COLORREF outline)
{
	setfillcolor(fill);
	fillroundrect(x1, y1, x2, y2, radius, radius);
	setlinecolor(outline);
	roundrect(x1, y1, x2, y2, radius, radius);
}

// �����������
void DrawDetailPanel(const GameInfo &game)
{
	int panelWidth = 400;
	int panelHeight = 500;
	int x = (getwidth() - panelWidth) / 2;
	int y = (getheight() - panelHeight) / 2;
	// �������
	DrawRoundRect(x, y, x + panelWidth, y + panelHeight, 15, theme.cardBackground, theme.accent);

	// ����
	settextcolor(theme.text);
	settextstyle(24, 0, "΢���ź�");
	outtextxy(x + 20, y + 20, game.title.c_str());

	// �����ͼ
	int coverHeight = 200;
	IMAGE coverImage;
	if (game.coverPath.empty())
	{
		setfillcolor(theme.cardBackground);
		fillrectangle(x + 5, y + 60, x + panelWidth - 5, y + 60 + coverHeight);
	}
	else
	{
		coverImage = IMAGE();
	}
	loadimage(&coverImage, game.coverPath.c_str(), panelWidth - 10, coverHeight, true);
	putimage(x + 5, y + 60, &coverImage);
	// ��Ϸ��Ϣ
	settextstyle(20, 0, "΢���ź�");
	std::ostringstream info;
	info << "�ϴ�����: " << game.GetLastPlayedStr() << "\n"
		 << "�������: " << game.playCount << "\n"
		 << "��ǩ: ";

	for (const auto &tag : game.tags)
	{
		info << tag << " ";
	}

	RECT infoRect = {x + 20, y + 280, x + panelWidth - 20, y + 380};
	drawtext(info.str().c_str(), &infoRect, DT_LEFT | DT_TOP | DT_WORDBREAK);

	// ��ť
	DrawRoundRect(x + 100, y + panelHeight - 60, x + 300, y + panelHeight - 20, 10, theme.accent, theme.accent);
	settextcolor(RGB(255, 255, 255));
	outtextxy(x + 170, y + panelHeight - 50, "������Ϸ");

	// �رհ�ť
	setlinecolor(theme.textSecondary);
	line(x + panelWidth - 30, y + 20, x + panelWidth - 20, y + 30);
	line(x + panelWidth - 20, y + 20, x + panelWidth - 30, y + 30);
}

// ����ɸѡ��
void DrawFilterBar(FilterBar &filterBar)
{
	vector<string> categories = filterBar.loadTags();
	settextstyle(16, 0, _T("Arial"));
	settextcolor(RGB(0, 0, 0));
	for (size_t i = 0; i < categories.size(); ++i)
	{
		COLORREF currentColor;
		if (i == filterBar.tagIndex) // ֱ�ӱȽ�����ֵ
		{
			currentColor = RGB(100, 150, 255); // ������ɫ
		}
		else
		{
			currentColor = (RGB(200, 200, 200));
		}
		DrawRoundRect(filterBar.x + i * (filterBar.width + 10), filterBar.y, filterBar.x + i * (filterBar.width + 10) + filterBar.width, filterBar.y + filterBar.height, 10, currentColor, currentColor);
		settextcolor(RGB(0, 0, 0));
		outtextxy(filterBar.x + i * (filterBar.width + 10) + 5, filterBar.y + 15, categories[i].c_str());
	}
}

// ������Ӱ�ť
void DrawAddButton(addButton &addBtn)
{
	setfillcolor(theme.add);
	setlinecolor(RGB(100, 100, 100));
	solidcircle(addBtn.x + addBtn.width / 2, addBtn.y + addBtn.height / 2, 25);

	setlinestyle(PS_SOLID, 3);
	setlinecolor(RGB(0, 0, 0));

	line(addBtn.x + addBtn.width / 4, addBtn.y + addBtn.height / 2, addBtn.x + addBtn.width * 3 / 4, addBtn.y + addBtn.height / 2);
	line(addBtn.x + addBtn.width / 2, addBtn.y + addBtn.height / 4, addBtn.x + addBtn.width / 2, addBtn.y + addBtn.height * 3 / 4);
}
// ������Ϸ��Ƭ
void DrawGameCard(const GameInfo &game, int x, int y, int width, int height, bool isHovered, bool isSelected)
{
	// 1. ���ƿ�Ƭ����
	COLORREF backgroundColor = isSelected ? theme.cardSelected : isHovered ? theme.cardHover
																		   : theme.cardBackground;
	DrawRoundRect(x, y, x + width, y + height, 10, backgroundColor, RGB(90, 95, 110));

	// 2. ������Ϸ����
	int coverHeight = height * 0.6;
	IMAGE coverImage;
	if (game.coverPath.empty())
	{
		setfillcolor(WHITE);
		fillrectangle(x + 5, y + 5, x + width - 5, y + coverHeight - 5);
	}
	else
	{
		coverImage = IMAGE();
		loadimage(&coverImage, game.coverPath.c_str(), width - 10, coverHeight, true);
		putimage(x + 5, y + 5, &coverImage);
	}
	// 3. ������Ϸ����
	int titleHeight = coverHeight + 10;
	settextcolor(theme.text);
	setbkmode(TRANSPARENT);
	RECT titleRect = {
		x + 15,
		y + titleHeight,
		x + width - 10,
		y + height - 30};
	drawtext(game.title.c_str(), &titleRect, DT_LEFT | DT_TOP | DT_WORDBREAK);

	// 4. ������Ϸ��Ϣ
	settextcolor(theme.textSecondary);
	char playCountInfo[64];
	sprintf_s(playCountInfo, "����: %d��", game.playCount);
	outtextxy(x + 15, y + height - 35, playCountInfo);
}

// ������
void DrawMainView(vector<GameInfo> games, int selectedIndex, int hoveredIndex, FilterBar *filterBar, addButton *addBtn)
{
	// 1. ���û�������
	setbkcolor(theme.background);
	cleardevice();

	// 2. ���Ʊ���
	settextcolor(theme.text);
	settextstyle(28, 0, _T("΢���ź�"));
	outtextxy(50, 30, _T("��Ϸ������"));

	// 3. ������Ϸ����
	for (size_t i = 0; i < games.size(); i++)
	{
		const int row = i / CardLayout::COLS;
		const int col = i % CardLayout::COLS;
		const int cardX = CardLayout::START_X + col * (CardLayout::CARD_WIDTH + CardLayout::SPACING);
		const int cardY = CardLayout::START_Y + row * (CardLayout::CARD_HEIGHT + CardLayout::SPACING);

		DrawGameCard(
			games[i],
			cardX, cardY,
			CardLayout::CARD_WIDTH,
			CardLayout::CARD_HEIGHT,
			i == hoveredIndex,
			i == selectedIndex);
	}
	DrawFilterBar(*filterBar);
	DrawAddButton(*addBtn);
}

bool checkClick(int mouseX, int mouseY, int x, int y, int width, int height)
{
	return mouseX >= x && mouseX <= x + width &&
		   mouseY >= y && mouseY <= y + height;
}

int main()
{
	FilterBar *filterBar;
	filterBar = new FilterBar();

	GameLauncherUI *launcher;
	launcher = new GameLauncherUI();
	launcher->filterBar = *filterBar; // ʹlauncherʹ���ⲿ������filterBar

	addButton *addBtn;
	addBtn = new addButton();

	initgraph(1000, 800);
	BeginBatchDraw();
	int selectedIndex = -1;
	int hoveredIndex = -1;
	bool showDetails = false;

	while (true)
	{
		ExMessage msg;
		while (peekmessage(&msg, EX_MOUSE | EX_KEY))
		{
			// ������ͣ״̬
			hoveredIndex = -1;
			const int cols = 3;
			const int cardWidth = 220;
			const int cardHeight = 280;
			const int spacing = 30;
			const int startX = 50;
			const int startY = 100;
			const int filterCols = 6;
			const int filterStartX = 50;
			const int filterStartY = 750;
			const int filterWidth = 100;
			const int filterHeight = 50;

			// ��������ͣ/���
			if (msg.message == WM_MOUSEMOVE || msg.message == WM_LBUTTONDOWN)
			{
				bool isGameHovered = false;

				// �����Ϸ��Ƭ
				for (int i = 0; i < launcher->games.size(); i++)
				{
					int row = i / cols;
					int col = i % cols;
					int x = startX + col * (cardWidth + spacing);
					int y = startY + row * (cardHeight + spacing);

					if (msg.x >= x && msg.x <= x + cardWidth &&
						msg.y >= y && msg.y <= y + cardHeight)
					{
						hoveredIndex = i;
						isGameHovered = true;

						if (msg.message == WM_LBUTTONDOWN)
						{
							static POINT lastClickPosGame = {0, 0};
							static DWORD lastClickTimeGame = 0;
							DWORD currentTime = GetTickCount();

							bool isDoubleClick = (currentTime - lastClickTimeGame < 300) &&
												 (abs(msg.x - lastClickPosGame.x) < 10) &&
												 (abs(msg.y - lastClickPosGame.y) < 10);

							if (isDoubleClick)
							{
								showDetails = true;
								selectedIndex = i;
							}
							lastClickTimeGame = currentTime;
							lastClickPosGame = {msg.x, msg.y};
						}
					}
				}
			}
			if (msg.message == WM_LBUTTONDOWN)
			{
				// ȷ����ǩ�����ѳ�ʼ��
				vector<string> categories = filterBar->loadTags();
				filterBar->currentTags = categories; // ���浱ǰ��ǩ�б�

				for (int i = 0; i < filterBar->currentTags.size(); i++)
				{
					int filtercol = i % filterCols;
					int filterX = filterStartX + filtercol * (filterWidth+10);
					int filterY = filterStartY;

					// �������Ƿ��ڱ�ǩ������
					if (msg.x >= filterX && msg.x <= filterX + filterWidth &&
						msg.y >= filterY && msg.y <= filterY + filterHeight)
					{
						// ����ѡ���ǩ��ֱ�Ӵ���ɸѡ
						filterBar->tagIndex = i;
						filterBar->filterGames.clear();				  // ���֮ǰ��ɸѡ���
						filterBar->getFilterGames(categories[i]);	  // ����ѡ�еı�ǩɸѡ��Ϸ
						launcher->filterBar = *filterBar;			  // ���� launcher �е� filterBar
						launcher->games = launcher->GetCurrentPage(); // ���µ�ǰҳ����Ϸ�б�
						break;										  // �ҵ�ƥ��ı�ǩ���˳�ѭ��
					}
				}
			}
			// ����������彻��
			if (showDetails && msg.message == WM_LBUTTONDOWN)
			{
				int panelWidth = 400, panelHeight = 500;
				int panelX = (getwidth() - panelWidth) / 2;
				int panelY = (getheight() - panelHeight) / 2;

				// �رհ�ť (���Ͻ�X)
				if (msg.x >= panelX + panelWidth - 30 && msg.x <= panelX + panelWidth - 10 &&
					msg.y >= panelY + 10 && msg.y <= panelY + 30)
				{
					showDetails = false;
					selectedIndex = -1; // ���ѡ��״̬
				}
				// ������ť
				else if (msg.x >= panelX + 100 && msg.x <= panelX + 300 &&
						 msg.y >= panelY + panelHeight - 60 && msg.y <= panelY + panelHeight - 20)
				{
					MessageBox(GetHWnd(), "��Ϸ������...", launcher->games[selectedIndex].title.c_str(), MB_OK);
					string gamePath = launcher->games[selectedIndex].exePath;
					HINSTANCE result = ShellExecute(NULL, "open", gamePath.c_str(), NULL, NULL, SW_SHOWNORMAL);
					if ((int)result > 32)
					{
						ifstream ifs(savePath, ios::binary);
						json j;
						ifs >> j;
						ifs.close();
						// ������Ϸ����
						j[selectedIndex]["lastPlayed"] = time(NULL);
						j[selectedIndex]["playCount"] = j[selectedIndex]["playCount"].get<int>() + 1;
						ofstream ofs(savePath, ios::binary);
						ofs << j.dump(4); // �������
						ofs.close();
						launcher->games[selectedIndex].lastPlayed = time(NULL);
						launcher->games[selectedIndex].playCount++;
					}
					else
					{
						MessageBox(GetHWnd(), "�޷�������Ϸ������·���Ƿ���ȷ��", "����", MB_OK | MB_ICONERROR);
					}
					showDetails = false; // ������Ϸ��ر��������
				}
			}
			else if (showDetails)
			{
				continue;
			}

			// ESC������
			if (msg.message == WM_KEYDOWN && msg.vkcode == VK_ESCAPE)
			{
				if (showDetails)
					showDetails = false;
				else
					break;
			}
		}

		if (checkClick(msg.x, msg.y, addBtn->x, addBtn->y, addBtn->width, addBtn->height) && msg.message == WM_LBUTTONDOWN)
		{
			addBtn->askGameInfo();
			filterBar->LoadGames();						  // ���¼�����Ϸ����
			filterBar->getFilterGames("ALL");			  // ����������Ϸ
			launcher->filterBar = *filterBar;			  // ����launcher�е�filterBar
			launcher->games = launcher->GetCurrentPage(); // ���µ�ǰҳ����Ϸ�б�
		}
		if (msg.message == WM_KEYDOWN && msg.vkcode == VK_RIGHT)
		{
			launcher->currentPage = launcher->pagePlus(launcher->currentPage);
			launcher->games = launcher->GetCurrentPage();
		}
		if (msg.message == WM_KEYDOWN && msg.vkcode == VK_LEFT)
		{
			launcher->currentPage = launcher->pageSub(launcher->currentPage);
			launcher->games = launcher->GetCurrentPage();
		}
		// ��Ⱦ�߼�
		cleardevice();
		DrawMainView(launcher->games, selectedIndex, hoveredIndex, filterBar, addBtn);
		if (showDetails && selectedIndex >= 0)
		{
			DrawDetailPanel(launcher->games[selectedIndex]);
		}
		FlushBatchDraw();

		// С���ӳٷ�ֹCPUռ�ù���
		static DWORD lastTime = GetTickCount();
		DWORD currentTime = GetTickCount();
		if (currentTime - lastTime < 10)
		{
			Sleep(10 - (currentTime - lastTime)); // ~100FPS����
		}
		lastTime = currentTime;
	}

	EndBatchDraw();
	closegraph();
	return 0;
}