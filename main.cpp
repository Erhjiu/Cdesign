#include <easyx.h>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <shellapi.h>
#include <sstream>
#include <vector>

#include "menu.h"
#include "data.h"
#include "json.hpp"
#include "add.h"
#include "FilterBar.h"
#include "Library.h"
#include "display_change.h"

using namespace std;
using json = nlohmann::json;

// 全局变量
UITheme theme;
ViewMode currentViewMode = GRID_VIEW;
const string savePath = "games.json";

// 网格视图布局常量
namespace CardLayout
{
	constexpr int COLS = 3;
	constexpr int CARD_WIDTH = 220;
	constexpr int CARD_HEIGHT = 280;
	constexpr int SPACING = 30;
	constexpr int START_X = 50;
	constexpr int START_Y = 100;
	constexpr double COVER_HEIGHT_RATIO = 0.7;
}

// 检查点击区域
bool checkClick(int mouseX, int mouseY, int x, int y, int width, int height)
{
	return mouseX >= x && mouseX <= x + width &&
		   mouseY >= y && mouseY <= y + height;
}

// 检查是否为双击
bool IsDoubleClick()
{
	static POINT lastClickPos = {0, 0};
	static DWORD lastClickTime = 0;
	DWORD currentTime = GetTickCount();

	POINT currentPos;
	GetCursorPos(&currentPos);

	bool isDoubleClick = (currentTime - lastClickTime < 300) &&
						 (abs(currentPos.x - lastClickPos.x) < 10) &&
						 (abs(currentPos.y - lastClickPos.y) < 10);

	lastClickTime = currentTime;
	lastClickPos = currentPos;

	return isDoubleClick;
}

// 绘制圆角矩形
void DrawRoundRect(int x1, int y1, int x2, int y2, int radius, COLORREF fill, COLORREF outline)
{
	setfillcolor(fill);
	fillroundrect(x1, y1, x2, y2, radius, radius);
	setlinecolor(outline);
	roundrect(x1, y1, x2, y2, radius, radius);
}

// 绘制筛选栏
void DrawFilterBar(FilterBar &filterBar)
{
	vector<string> categories = filterBar.loadTags();
	settextstyle(16, 0, _T("Arial"));
	settextcolor(RGB(0, 0, 0));

	for (size_t i = 0; i < categories.size(); ++i)
	{
		COLORREF currentColor = (i == filterBar.tagIndex) ? RGB(100, 150, 255) : RGB(200, 200, 200);

		DrawRoundRect(
			filterBar.x + i * (filterBar.width + 10),
			filterBar.y,
			filterBar.x + i * (filterBar.width + 10) + filterBar.width,
			filterBar.y + filterBar.height,
			10,
			currentColor,
			currentColor);

		settextcolor(RGB(0, 0, 0));
		outtextxy(
			filterBar.x + i * (filterBar.width + 10) + 5,
			filterBar.y + 15,
			categories[i].c_str());
	}
}

// 绘制添加按钮
void DrawAddButton(addButton &addBtn)
{
	// 绘制圆形背景
	setfillcolor(RGB(100, 180, 255)); // 使用柔和的蓝色
	setlinecolor(RGB(220, 220, 220)); // 浅灰色边框
	setlinestyle(PS_SOLID, 1);		  // 1像素线宽
	fillcircle(addBtn.x + addBtn.width / 2, addBtn.y + addBtn.height / 2, 25);

	// 绘制加号
	setlinestyle(PS_SOLID, 2); // 2像素线宽的加号
	setlinecolor(WHITE);	   // 白色加号

	// 水平线
	line(addBtn.x + addBtn.width / 4 + 5,
		 addBtn.y + addBtn.height / 2,
		 addBtn.x + addBtn.width * 3 / 4 - 5,
		 addBtn.y + addBtn.height / 2);

	// 垂直线
	line(addBtn.x + addBtn.width / 2,
		 addBtn.y + addBtn.height / 4 + 5,
		 addBtn.x + addBtn.width / 2,
		 addBtn.y + addBtn.height * 3 / 4 - 5);
}

// 绘制游戏卡片
void DrawGameCard(const GameInfo &game, int x, int y, int width, int height, bool isHovered, bool isSelected)
{
	// 1. 绘制卡片背景
	COLORREF backgroundColor = isSelected ? theme.cardSelected : isHovered ? theme.cardHover
																		   : theme.cardBackground;
	DrawRoundRect(x, y, x + width, y + height, 10, backgroundColor, RGB(90, 95, 110));

	// 2. 绘制游戏封面
	int coverHeight = height * CardLayout::COVER_HEIGHT_RATIO;
	IMAGE coverImage;
	if (game.coverPath.empty())
	{
		setfillcolor(WHITE);
		fillrectangle(x + 5, y + 5, x + width - 5, y + coverHeight - 5);
	}
	else
	{
		loadimage(&coverImage, game.coverPath.c_str(), width - 10, coverHeight, true);
		putimage(x + 5, y + 5, &coverImage);
	}

	// 3. 绘制游戏标题
	settextcolor(theme.text);
	setbkmode(TRANSPARENT);
	settextstyle(20, 0, _T("微软雅黑"));
	RECT titleRect = {x + 15, y + coverHeight + 10, x + width - 10, y + height - 30};
	drawtext(game.title.c_str(), &titleRect, DT_LEFT | DT_TOP | DT_WORDBREAK);

	// 4. 绘制游戏信息
	settextcolor(theme.textSecondary);
	char playCountInfo[64];
	sprintf_s(playCountInfo, "游玩: %d次", game.playCount);
	outtextxy(x + 15, y + height - 25, playCountInfo);
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
	settextstyle(24, 0, _T("微软雅黑"));
	outtextxy(x + 20, y + 20, game.title.c_str());

	// 封面大图
	int coverHeight = 200;
	IMAGE coverImage;
	if (!game.coverPath.empty())
	{
		loadimage(&coverImage, game.coverPath.c_str(), panelWidth - 10, coverHeight, true);
		putimage(x + 5, y + 60, &coverImage);
	}

	// 游戏信息
	settextstyle(20, 0, _T("微软雅黑"));
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

	// 启动按钮
	DrawRoundRect(x + 100, y + panelHeight - 60, x + 300, y + panelHeight - 20, 10,
				  theme.accent, theme.accent);
	settextcolor(WHITE);
	settextstyle(20, 0, _T("微软雅黑"));
	outtextxy(x + 170, y + panelHeight - 50, _T("启动游戏"));

	// 关闭按钮
	setlinecolor(theme.textSecondary);
	line(x + panelWidth - 30, y + 20, x + panelWidth - 20, y + 30);
	line(x + panelWidth - 20, y + 20, x + panelWidth - 30, y + 30);
}

// 主界面绘制
void DrawMainView(vector<GameInfo> &games, int selectedIndex, int hoveredIndex,
				  FilterBar *filterBar, addButton *addBtn)
{
	// 1. 设置基本背景
	setbkcolor(theme.background);
	cleardevice();

	// 2. 绘制标题
	settextcolor(theme.text);
	settextstyle(28, 0, _T("微软雅黑"));
	outtextxy(50, 30, _T("GameLibrary"));

	// 3. 绘制视图切换按钮
	DrawViewToggleButton(800, 30, currentViewMode);

	// 4. 根据当前视图模式绘制内容
	if (currentViewMode == GRID_VIEW)
	{
		// 绘制网格视图
		for (size_t i = 0; i < games.size(); i++)
		{
			const int row = i / CardLayout::COLS;
			const int col = i % CardLayout::COLS;
			const int cardX = CardLayout::START_X + col * (CardLayout::CARD_WIDTH + CardLayout::SPACING);
			const int cardY = CardLayout::START_Y + row * (CardLayout::CARD_HEIGHT + CardLayout::SPACING);

			DrawGameCard(games[i], cardX, cardY, CardLayout::CARD_WIDTH, CardLayout::CARD_HEIGHT,
						 i == hoveredIndex, i == selectedIndex);
		}
	}
	else
	{
		// 绘制列表视图
		DrawListView(games, hoveredIndex, selectedIndex,
					 (filterBar->currentPage - 1) * ListLayout::ITEMS_PER_PAGE);
	}

	// 5. 绘制过滤栏和添加按钮
	DrawFilterBar(*filterBar);
	DrawAddButton(*addBtn);
}

// 主函数
int main()
{
	// 初始化组件
	FilterBar *filterBar = new FilterBar();
	GameLauncherUI *launcher = new GameLauncherUI();
	addButton *addBtn = new addButton();
	launcher->filterBar = *filterBar;

	// 初始化图形界面
	initgraph(1000, 800);
	BeginBatchDraw();

	int selectedIndex = -1;
	int hoveredIndex = -1;
	bool showDetails = false;

	// 主循环
	while (true)
	{
		ExMessage msg;
		while (peekmessage(&msg, EX_MOUSE | EX_KEY))
		{
			// 更新悬停状态
			hoveredIndex = -1;

			// 检查视图切换按钮点击
			if (msg.message == WM_LBUTTONDOWN && CheckViewToggleClick(msg.x, msg.y))
			{
				currentViewMode = (currentViewMode == GRID_VIEW) ? LIST_VIEW : GRID_VIEW;
				continue;
			}

			// 处理鼠标事件
			if (msg.message == WM_MOUSEMOVE || msg.message == WM_LBUTTONDOWN)
			{
				if (currentViewMode == GRID_VIEW)
				{
					// 网格视图的鼠标处理
					for (int i = 0; i < launcher->games.size(); i++)
					{
						int row = i / CardLayout::COLS;
						int col = i % CardLayout::COLS;
						int x = CardLayout::START_X + col * (CardLayout::CARD_WIDTH + CardLayout::SPACING);
						int y = CardLayout::START_Y + row * (CardLayout::CARD_HEIGHT + CardLayout::SPACING);

						if (msg.x >= x && msg.x <= x + CardLayout::CARD_WIDTH &&
							msg.y >= y && msg.y <= y + CardLayout::CARD_HEIGHT)
						{
							hoveredIndex = i;
							if (msg.message == WM_LBUTTONDOWN)
							{
								if (IsDoubleClick())
								{
									selectedIndex = i;
									showDetails = true;
								}
							}
							break;
						}
					}
				}
				else
				{
					// 列表视图的鼠标处理
					int itemIndex = GetListItemIndex(msg.x, msg.y,
													 (filterBar->currentPage - 1) * ListLayout::ITEMS_PER_PAGE);

					if (itemIndex >= 0 && itemIndex < launcher->games.size())
					{
						hoveredIndex = itemIndex;
						if (msg.message == WM_LBUTTONDOWN)
						{
							// 单击时不立即选中
							if (IsDoubleClick())
							{
								selectedIndex = itemIndex;
								showDetails = true;
							}
						}
					}
					else
					{
						hoveredIndex = -1; // 鼠标不在任何项目上时清除悬停状态
					}
				}
			}

			// 处理过滤栏点击
			if (msg.message == WM_LBUTTONDOWN)
			{
				vector<string> categories = filterBar->loadTags();
				for (size_t i = 0; i < categories.size(); i++)
				{
					int x = 50 + i * 110;
					int y = 750;
					if (msg.x >= x && msg.x <= x + 100 &&
						msg.y >= y && msg.y <= y + 30)
					{
						filterBar->tagIndex = i;
						filterBar->filterGames.clear();
						filterBar->getFilterGames(categories[i]);
						launcher->filterBar = *filterBar;
						launcher->games = launcher->GetCurrentPage();
						break;
					}
				}
			}

			// 处理添加按钮点击
			if (msg.message == WM_LBUTTONDOWN &&
				checkClick(msg.x, msg.y, addBtn->x, addBtn->y, addBtn->width, addBtn->height))
			{
				addBtn->askGameInfo();
				filterBar->LoadGames();
				filterBar->getFilterGames("ALL");
				launcher->filterBar = *filterBar;
				launcher->games = launcher->GetCurrentPage();
			}

			// 处理翻页
			if (msg.message == WM_KEYDOWN)
			{
				if (msg.vkcode == VK_RIGHT)
				{
					launcher->currentPage = launcher->pagePlus(launcher->currentPage);
					launcher->games = launcher->GetCurrentPage();
				}
				else if (msg.vkcode == VK_LEFT)
				{
					launcher->currentPage = launcher->pageSub(launcher->currentPage);
					launcher->games = launcher->GetCurrentPage();
				}
				else if (msg.vkcode == VK_ESCAPE)
				{
					if (showDetails)
					{
						showDetails = false;
					}
					else
					{
						goto cleanup;
					}
				}
			}

			// 处理详情面板
			if (showDetails && msg.message == WM_LBUTTONDOWN)
			{
				int panelWidth = 400;
				int panelHeight = 500;
				int panelX = (getwidth() - panelWidth) / 2;
				int panelY = (getheight() - panelHeight) / 2;

				// 关闭按钮
				if (msg.x >= panelX + panelWidth - 30 && msg.x <= panelX + panelWidth - 10 &&
					msg.y >= panelY + 10 && msg.y <= panelY + 30)
				{
					showDetails = false;
					selectedIndex = -1;
				}
				// 启动按钮
				else if (msg.x >= panelX + 100 && msg.x <= panelX + 300 &&
						 msg.y >= panelY + panelHeight - 60 && msg.y <= panelY + panelHeight - 20)
				{
					if (selectedIndex >= 0 && selectedIndex < launcher->games.size())
					{
						const GameInfo &game = launcher->games[selectedIndex];
						MessageBox(GetHWnd(), "游戏启动中...", game.title.c_str(), MB_OK);

						HINSTANCE result = ShellExecute(NULL, "open", game.exePath.c_str(),
														NULL, NULL, SW_SHOWNORMAL);
						if ((int)result > 32)
						{
							// 更新游戏数据
							ifstream ifs(savePath);
							json j;
							ifs >> j;
							ifs.close();

							j[selectedIndex]["lastPlayed"] = time(NULL);
							j[selectedIndex]["playCount"] = j[selectedIndex]["playCount"].get<int>() + 1;

							ofstream ofs(savePath);
							ofs << j.dump(4);
							ofs.close();

							launcher->games[selectedIndex].lastPlayed = time(NULL);
							launcher->games[selectedIndex].playCount++;
						}
						else
						{
							MessageBox(GetHWnd(), "无法启动游戏，请检查路径是否正确。",
									   "错误", MB_OK | MB_ICONERROR);
						}
						showDetails = false;
					}
				}
			}
		}

		// 渲染界面
		DrawMainView(launcher->games, selectedIndex, hoveredIndex, filterBar, addBtn);
		if (showDetails && selectedIndex >= 0 && selectedIndex < launcher->games.size())
		{
			DrawDetailPanel(launcher->games[selectedIndex]);
		}
		FlushBatchDraw();

		// 帧率控制
		Sleep(10);
	}

cleanup:
	EndBatchDraw();
	closegraph();
	delete filterBar;
	delete launcher;
	delete addBtn;
	return 0;
}