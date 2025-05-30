#include "menu.h"
#include "add.h"
#include "FilterBar.h"
#include <easyx.h>

void GameLauncherUI::DrawGameCard(const GameInfo& game, int x, int y, int width, int height, bool isHovered, bool isSelected) {
    // ��Ƭ����
    COLORREF bg = isSelected ? theme.cardSelected :
        isHovered ? theme.cardHover : theme.cardBackground;
    DrawRoundRect(x, y, x + width, y + height, 10, bg, RGB(90, 95, 110));

    // ����ͼƬ
    int coverHeight = height * 0.7;
	/*for (GameInfo& game : games) {
		IMAGE coverImage;
		loadimage(&coverImage, game.coverPath.c_str(), width - 10, coverHeight - 10);
		putimage(x + 5, y + 5, &coverImage);
	}*/
    setfillcolor(RGB(255, 255, 255));
    fillrectangle(x + 5, y + 5, x + width - 5, y + coverHeight - 5);

    // ��Ϸ����
    settextcolor(theme.text);
    setbkmode(TRANSPARENT);
    RECT titleRect = { x + 10, y + coverHeight, x + width - 10, y + height - 30 };
    drawtext(game.title.c_str(), &titleRect, DT_LEFT | DT_TOP | DT_WORDBREAK);

    // ��Ϸ��Ϣ
    settextcolor(theme.textSecondary);
    char info[64];
    sprintf_s(info, "����: %d��", game.playCount);
    outtextxy(x + 10, y + height - 25, info);
}

// ������
void GameLauncherUI::DrawMainView()
{
	setbkcolor(theme.background);
	cleardevice();

	settextcolor(theme.text);
	settextstyle(28, 0, _T("΢���ź�"));
	outtextxy(50, 30, _T("��Ϸ������"));
	//settextstyle(14, 0, _T("΢���ź�"));
	//outtextxy(0, 50, _T("ɸѡ"));
	//FilterBar filterBar;
	//filterBar.Draw();
    // ��Ϸ����
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

    // ɸѡ��ǩ (�򻯰�)
    settextstyle(14, 0, "΢���ź�");
    outtextxy(50, getheight() - 50, "ɸѡ: ȫ�� ð�� ���� RPG ģ�� ����");
	// ��Ӱ�ť
	addBtn->Draw(theme);
}
