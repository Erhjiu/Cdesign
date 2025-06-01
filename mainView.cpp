#include "menu.h"
#include "add.h"
#include "FilterBar.h"
#include <easyx.h>

// 游戏卡片布局常量
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

void GameLauncherUI::DrawGameCard(const GameInfo& game, int x, int y, int width, int height, bool isHovered, bool isSelected)
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
	int titleHeight = coverHeight+10;
        settextcolor(theme.text);
        setbkmode(TRANSPARENT);
        RECT titleRect = {
            x + 15,
            y + titleHeight,
            x + width - 10,
            y + height - 30 };
        drawtext(game.title.c_str(), &titleRect, DT_LEFT | DT_TOP | DT_WORDBREAK);

        // 4. 绘制游戏信息
        settextcolor(theme.textSecondary);
        char playCountInfo[64];
        sprintf_s(playCountInfo, "游玩: %d次", game.playCount);
        outtextxy(x + 15, y + height - 35, playCountInfo);
}
void GameLauncherUI::DrawMainView()
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

    // 4. 绘制底部筛选标签
    settextstyle(14, 0, _T("微软雅黑"));
    outtextxy(50, getheight() - 50, _T("筛选: 全部 冒险 竞速 RPG 模拟 动作"));

    // 5. 绘制添加按钮
    addBtn->Draw(theme);
}
