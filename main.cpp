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
	constexpr double COVER_HEIGHT_RATIO = 0.7; // 封面图片高度占卡片高度的比例
}

// 圆角矩形
void DrawRoundRect(int x1, int y1, int x2, int y2, int radius, COLORREF fill, COLORREF outline)
{
	setfillcolor(fill);
	fillroundrect(x1, y1, x2, y2, radius, radius);
	setlinecolor(outline);
	roundrect(x1, y1, x2, y2, radius, radius);
}

// 绘制详情面板
void DrawDetailPanel(const GameInfo &game)
{
	int panelWidth = 400;
	int panelHeight = 500;
	int x = (getwidth() - panelWidth) / 2;
	int y = (getheight() - panelHeight) / 2;
	// 背景面板
	DrawRoundRect(x, y, x + panelWidth, y + panelHeight, 15, theme.cardBackground, theme.accent);

	// 标题
	settextcolor(theme.text);
	settextstyle(24, 0, "微软雅黑");
	outtextxy(x + 20, y + 20, game.title.c_str());

	// 封面大图
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
	// 游戏信息
	settextstyle(20, 0, "微软雅黑");
	std::ostringstream info;
	info << "上次游玩: " << game.GetLastPlayedStr() << "\n"
		 << "游玩次数: " << game.playCount << "\n"
		 << "标签: ";

	for (const auto &tag : game.tags)
	{
		info << tag << " ";
	}

	RECT infoRect = {x + 20, y + 280, x + panelWidth - 20, y + 380};
	drawtext(info.str().c_str(), &infoRect, DT_LEFT | DT_TOP | DT_WORDBREAK);

	// 按钮
	DrawRoundRect(x + 100, y + panelHeight - 60, x + 300, y + panelHeight - 20, 10, theme.accent, theme.accent);
	settextcolor(RGB(255, 255, 255));
	outtextxy(x + 170, y + panelHeight - 50, "启动游戏");

	// 关闭按钮
	setlinecolor(theme.textSecondary);
	line(x + panelWidth - 30, y + 20, x + panelWidth - 20, y + 30);
	line(x + panelWidth - 20, y + 20, x + panelWidth - 30, y + 30);
}

// 绘制筛选栏
void DrawFilterBar(FilterBar &filterBar)
{
	vector<string> categories = filterBar.loadTags();
	settextstyle(16, 0, _T("Arial"));
	settextcolor(RGB(0, 0, 0));
	for (size_t i = 0; i < categories.size(); ++i)
	{
		COLORREF currentColor;
		if (i == filterBar.tagIndex) // 直接比较索引值
		{
			currentColor = RGB(100, 150, 255); // 高亮颜色
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

// 绘制添加按钮
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
// 绘制游戏卡片
void DrawGameCard(const GameInfo &game, int x, int y, int width, int height, bool isHovered, bool isSelected)
{
	// 1. 绘制卡片背景
	COLORREF backgroundColor = isSelected ? theme.cardSelected : isHovered ? theme.cardHover
																		   : theme.cardBackground;
	DrawRoundRect(x, y, x + width, y + height, 10, backgroundColor, RGB(90, 95, 110));

	// 2. 绘制游戏封面
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
	// 3. 绘制游戏标题
	int titleHeight = coverHeight + 10;
	settextcolor(theme.text);
	setbkmode(TRANSPARENT);
	RECT titleRect = {
		x + 15,
		y + titleHeight,
		x + width - 10,
		y + height - 30};
	drawtext(game.title.c_str(), &titleRect, DT_LEFT | DT_TOP | DT_WORDBREAK);

	// 4. 绘制游戏信息
	settextcolor(theme.textSecondary);
	char playCountInfo[64];
	sprintf_s(playCountInfo, "游玩: %d次", game.playCount);
	outtextxy(x + 15, y + height - 35, playCountInfo);
}

// 主界面
void DrawMainView(vector<GameInfo> games, int selectedIndex, int hoveredIndex, FilterBar *filterBar, addButton *addBtn)
{
	// 1. 设置基本背景
	setbkcolor(theme.background);
	cleardevice();

	// 2. 绘制标题
	settextcolor(theme.text);
	settextstyle(28, 0, _T("微软雅黑"));
	outtextxy(50, 30, _T("游戏启动器"));

	// 3. 绘制游戏网格
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
	launcher->filterBar = *filterBar; // 使launcher使用外部创建的filterBar

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
			// 更新悬停状态
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

			// 检查鼠标悬停/点击
			if (msg.message == WM_MOUSEMOVE || msg.message == WM_LBUTTONDOWN)
			{
				bool isGameHovered = false;

				// 检测游戏卡片
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
				// 确保标签数组已初始化
				vector<string> categories = filterBar->loadTags();
				filterBar->currentTags = categories; // 保存当前标签列表

				for (int i = 0; i < filterBar->currentTags.size(); i++)
				{
					int filtercol = i % filterCols;
					int filterX = filterStartX + filtercol * (filterWidth+10);
					int filterY = filterStartY;

					// 检查鼠标是否在标签区域内
					if (msg.x >= filterX && msg.x <= filterX + filterWidth &&
						msg.y >= filterY && msg.y <= filterY + filterHeight)
					{
						// 单击选择标签，直接触发筛选
						filterBar->tagIndex = i;
						filterBar->filterGames.clear();				  // 清空之前的筛选结果
						filterBar->getFilterGames(categories[i]);	  // 根据选中的标签筛选游戏
						launcher->filterBar = *filterBar;			  // 更新 launcher 中的 filterBar
						launcher->games = launcher->GetCurrentPage(); // 更新当前页面游戏列表
						break;										  // 找到匹配的标签后退出循环
					}
				}
			}
			// 处理详情面板交互
			if (showDetails && msg.message == WM_LBUTTONDOWN)
			{
				int panelWidth = 400, panelHeight = 500;
				int panelX = (getwidth() - panelWidth) / 2;
				int panelY = (getheight() - panelHeight) / 2;

				// 关闭按钮 (右上角X)
				if (msg.x >= panelX + panelWidth - 30 && msg.x <= panelX + panelWidth - 10 &&
					msg.y >= panelY + 10 && msg.y <= panelY + 30)
				{
					showDetails = false;
					selectedIndex = -1; // 清除选中状态
				}
				// 启动按钮
				else if (msg.x >= panelX + 100 && msg.x <= panelX + 300 &&
						 msg.y >= panelY + panelHeight - 60 && msg.y <= panelY + panelHeight - 20)
				{
					MessageBox(GetHWnd(), "游戏启动中...", launcher->games[selectedIndex].title.c_str(), MB_OK);
					string gamePath = launcher->games[selectedIndex].exePath;
					HINSTANCE result = ShellExecute(NULL, "open", gamePath.c_str(), NULL, NULL, SW_SHOWNORMAL);
					if ((int)result > 32)
					{
						ifstream ifs(savePath, ios::binary);
						json j;
						ifs >> j;
						ifs.close();
						// 更新游戏数据
						j[selectedIndex]["lastPlayed"] = time(NULL);
						j[selectedIndex]["playCount"] = j[selectedIndex]["playCount"].get<int>() + 1;
						ofstream ofs(savePath, ios::binary);
						ofs << j.dump(4); // 美化输出
						ofs.close();
						launcher->games[selectedIndex].lastPlayed = time(NULL);
						launcher->games[selectedIndex].playCount++;
					}
					else
					{
						MessageBox(GetHWnd(), "无法启动游戏，请检查路径是否正确。", "错误", MB_OK | MB_ICONERROR);
					}
					showDetails = false; // 启动游戏后关闭详情面板
				}
			}
			else if (showDetails)
			{
				continue;
			}

			// ESC键处理
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
			filterBar->LoadGames();						  // 重新加载游戏数据
			filterBar->getFilterGames("ALL");			  // 加载所有游戏
			launcher->filterBar = *filterBar;			  // 更新launcher中的filterBar
			launcher->games = launcher->GetCurrentPage(); // 更新当前页面游戏列表
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
		// 渲染逻辑
		cleardevice();
		DrawMainView(launcher->games, selectedIndex, hoveredIndex, filterBar, addBtn);
		if (showDetails && selectedIndex >= 0)
		{
			DrawDetailPanel(launcher->games[selectedIndex]);
		}
		FlushBatchDraw();

		// 小幅延迟防止CPU占用过高
		static DWORD lastTime = GetTickCount();
		DWORD currentTime = GetTickCount();
		if (currentTime - lastTime < 10)
		{
			Sleep(10 - (currentTime - lastTime)); // ~100FPS上限
		}
		lastTime = currentTime;
	}

	EndBatchDraw();
	closegraph();
	return 0;
}