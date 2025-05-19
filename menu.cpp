#include "menu.h"
#include <graphics.h>
#include <sstream>


using namespace std;



// 修复后的代码
    void GameLauncherUI::DrawDetailPanel(const GameInfo& game, const UITheme& theme) {
    int panelWidth = 400;
    int panelHeight = 500;
    int x = (getwidth() - panelWidth) / 2;
    int y = (getheight() - panelHeight) / 2;

    // 背景面板
    solidroundrect(x, y, x + panelHeight, y + panelWidth, theme.cardBackground, theme.accent);

    // 标题
    settextcolor(theme.text);
    settextstyle(20, 0, _T("微软雅黑"));
	outtextxy(x + 20, y + 20, game.title.c_str());

    //封面大图
	int coverWidth = 200;
	setfillcolor(RGB(rand() % 155 + 100, rand() % 155 + 100, rand() % 155 + 100));
    fillrectangle(x + 20, y + 60, x + panelWidth - 20, y + 60 + coverWidth);

    //游戏信息
	settextstyle(14, 0, _T("微软雅黑"));
    ostringstream info;
    info << "上次游玩时间: " << game.GetLastPlayedStr() << "\n"
        << "游玩次数: " << game.playCount << "\n"
        << "标签: ";
	for (const auto& tag : game.tags) {
			info << tag << " ";
		}
     RECT infoRect = { x + 20,y + 20,x + panelWidth - 20,y + 380 };
	 drawtext(info.str().c_str(), &infoRect, DT_LEFT | DT_WORDBREAK | DT_SINGLELINE);

     //按钮
     DrawRoundRect(x + 100, y + panelHeight - 60, x + 300, y + panelHeight - 20, 10, theme.accent, theme.accent);
	 settextcolor(RGB(255, 255, 255));
	 outtextxy(x + 170, y + panelHeight - 50, _T("启动游戏"));

     //关闭按钮
	 setlinecolor(theme.textSecondary);
	 line(x + panelWidth - 30, y + 20, x + panelWidth - 20, y + 30);
	 line(x + panelWidth - 20, y + 20, x + panelWidth - 30, y + 30);
}