#include "menu.h"
#include "add.h"
#include "FilterBar.h"
#include <easyx.h>

// ��Ϸ��Ƭ���ֳ���
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

void GameLauncherUI::DrawGameCard(const GameInfo& game, int x, int y, int width, int height, bool isHovered, bool isSelected)
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
	int titleHeight = coverHeight+10;
        settextcolor(theme.text);
        setbkmode(TRANSPARENT);
        RECT titleRect = {
            x + 15,
            y + titleHeight,
            x + width - 10,
            y + height - 30 };
        drawtext(game.title.c_str(), &titleRect, DT_LEFT | DT_TOP | DT_WORDBREAK);

        // 4. ������Ϸ��Ϣ
        settextcolor(theme.textSecondary);
        char playCountInfo[64];
        sprintf_s(playCountInfo, "����: %d��", game.playCount);
        outtextxy(x + 15, y + height - 35, playCountInfo);
}
void GameLauncherUI::DrawMainView()
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

    // 4. ���Ƶײ�ɸѡ��ǩ
    settextstyle(14, 0, _T("΢���ź�"));
    outtextxy(50, getheight() - 50, _T("ɸѡ: ȫ�� ð�� ���� RPG ģ�� ����"));

    // 5. ������Ӱ�ť
    addBtn->Draw(theme);
}
