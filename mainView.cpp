#include "menu.h"
#include "add.h"
#include "FilterBar.h"
#include <easyx.h>

void GameLauncherUI::DrawGameCard(const GameInfo& game, int x, int y, int width, int height, bool isHovered, bool isSelected) {
    // 卡片背景
    COLORREF bg = isSelected ? theme.cardSelected :
        isHovered ? theme.cardHover : theme.cardBackground;
    DrawRoundRect(x, y, x + width, y + height, 10, bg, RGB(90, 95, 110));

    // 封面图片 (示例中用白色矩形代替)
    int coverHeight = height * 0.7;
    setfillcolor(RGB(255, 255, 255));
    fillrectangle(x + 5, y + 5, x + width - 5, y + coverHeight - 5);

    // 游戏标题
    settextcolor(theme.text);
    setbkmode(TRANSPARENT);
    RECT titleRect = { x + 10, y + coverHeight, x + width - 10, y + height - 30 };
    drawtext(game.title.c_str(), &titleRect, DT_LEFT | DT_TOP | DT_WORDBREAK);

    // 游戏信息
    settextcolor(theme.textSecondary);
    char info[64];
    sprintf_s(info, "游玩: %d次", game.playCount);
    outtextxy(x + 10, y + height - 25, info);
}

// 主界面
void GameLauncherUI::DrawMainView()
{
	setbkcolor(theme.background);
	cleardevice();

	settextcolor(theme.text);
	settextstyle(28, 0, _T("微软雅黑"));
	outtextxy(50, 30, _T("游戏启动器"));
	//settextstyle(14, 0, _T("微软雅黑"));
	//outtextxy(0, 50, _T("筛选"));
	//FilterBar filterBar;
	//filterBar.Draw();
    // 游戏网格
    const int cols = 3;
    const int cardWidth = 220;
    const int cardHeight = 280;
    const int spacing = 30;
    const int startX = 50;
    const int startY = 100;

    for (int i = 0; i < games.size(); i++) {
        int row = i / cols;
        int col = i % cols;
        int x = startX + col * (cardWidth + spacing);
        int y = startY + row * (cardHeight + spacing);

        DrawGameCard(games[i], x, y, cardWidth, cardHeight,
            i == hoveredIndex, i == selectedIndex);
    }
	ExMessage msg = getmessage();
	addButton addBtn(getwidth() - 70, getheight() - 70, 50, 50);
	bool isAddHovered = addBtn.checkHover(msg.x,msg.y);
	addBtn.Draw(theme, isAddHovered);

    // 筛选标签 (简化版)
    settextstyle(14, 0, "微软雅黑");
    outtextxy(50, getheight() - 50, "筛选: 全部 冒险 竞速 RPG 模拟 动作");
}
