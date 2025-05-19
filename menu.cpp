#include "menu.h"
#include <graphics.h>
#include <sstream>


using namespace std;



// �޸���Ĵ���
    void GameLauncherUI::DrawDetailPanel(const GameInfo& game, const UITheme& theme) {
    int panelWidth = 400;
    int panelHeight = 500;
    int x = (getwidth() - panelWidth) / 2;
    int y = (getheight() - panelHeight) / 2;

    // �������
    solidroundrect(x, y, x + panelHeight, y + panelWidth, theme.cardBackground, theme.accent);

    // ����
    settextcolor(theme.text);
    settextstyle(20, 0, _T("΢���ź�"));
	outtextxy(x + 20, y + 20, game.title.c_str());

    //�����ͼ
	int coverWidth = 200;
	setfillcolor(RGB(rand() % 155 + 100, rand() % 155 + 100, rand() % 155 + 100));
    fillrectangle(x + 20, y + 60, x + panelWidth - 20, y + 60 + coverWidth);

    //��Ϸ��Ϣ
	settextstyle(14, 0, _T("΢���ź�"));
    ostringstream info;
    info << "�ϴ�����ʱ��: " << game.GetLastPlayedStr() << "\n"
        << "�������: " << game.playCount << "\n"
        << "��ǩ: ";
	for (const auto& tag : game.tags) {
			info << tag << " ";
		}
     RECT infoRect = { x + 20,y + 20,x + panelWidth - 20,y + 380 };
	 drawtext(info.str().c_str(), &infoRect, DT_LEFT | DT_WORDBREAK | DT_SINGLELINE);

     //��ť
     DrawRoundRect(x + 100, y + panelHeight - 60, x + 300, y + panelHeight - 20, 10, theme.accent, theme.accent);
	 settextcolor(RGB(255, 255, 255));
	 outtextxy(x + 170, y + panelHeight - 50, _T("������Ϸ"));

     //�رհ�ť
	 setlinecolor(theme.textSecondary);
	 line(x + panelWidth - 30, y + 20, x + panelWidth - 20, y + 30);
	 line(x + panelWidth - 20, y + 20, x + panelWidth - 30, y + 30);
}