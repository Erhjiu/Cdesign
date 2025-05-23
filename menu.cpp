#include "menu.h"
#include "dropDown.h"
#include <easyx.h>
#include <sstream>

using namespace std;

void GameLauncherUI::DrawDetailPanel(const GameInfo &game, const UITheme &theme)
{
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

    // �����ͼ
    int coverWidth = 200;
    setfillcolor(RGB(255, 255, 255)); // !!!���޸ģ���Ϸ����
    fillrectangle(x + 20, y + 60, x + panelWidth - 20, y + 60 + coverWidth);

    // ��Ϸ��Ϣ
    settextstyle(20, 0, _T("΢���ź�"));
    ostringstream info;
    info << "�ϴ�����ʱ��: " << game.GetLastPlayedStr() << "\n"
         << "�������: " << game.playCount << "\n"
         << "��ǩ: ";
    for (const auto &tag : game.tags)
    {
        info << tag << " ";
    }
    RECT infoRect = {x + 20, y + 20, x + panelWidth - 20, y + 380};
    drawtext(info.str().c_str(), &infoRect, DT_LEFT | DT_WORDBREAK | DT_SINGLELINE);

    // ��ť
    DrawRoundRect(x + 100, y + panelHeight - 60, x + 300, y + panelHeight - 20, 10, theme.accent, theme.accent);
    settextcolor(RGB(255, 255, 255));
    outtextxy(x + 170, y + panelHeight - 50, _T("������Ϸ"));

    // �رհ�ť
    setlinecolor(theme.textSecondary);
    line(x + panelWidth - 30, y + 20, x + panelWidth - 20, y + 30);
    line(x + panelWidth - 20, y + 20, x + panelWidth - 30, y + 30);
}

void GameLauncherUI::run()
{
    initgraph(1000, 700);
    BeginBatchDraw();

    while (true) {
        ExMessage msg;
        while (peekmessage(&msg)) {
            msg = getmessage();
            hoveredIndex = GetHoveredIndex(msg.x, msg.y);

            if (msg.message == WM_LBUTTONDOWN) {
                if (hoveredIndex != -1) {
                    selectedIndex = hoveredIndex;

                    static DWORD lastClickTime = 0;
                    DWORD currentTime = GetTickCount();
                    if (currentTime - lastClickTime < 300) {
                        // ˫���¼�
                        showDetails = true;
                    }
                    lastClickTime = currentTime;
                }
            }
            //����������رհ�ť
            if (showDetails) {
                int panelWidth = 400, panelHeight = 500;
                int panelX = (getwidth() - panelWidth) / 2;
                int panelY = (getheight() - panelHeight) / 2;

                // �رհ�ť
                if (msg.x >= panelX + panelHeight - 30 && msg.x <= panelX + panelHeight - 20 && msg.y >= panelY + 20 && msg.y <= panelY + 30) {
                    showDetails = false;
                }
                //������ť
                if (msg.x >= panelX + 100 && msg.x <= panelX + 300 && msg.y >= panelY + panelHeight - 60 && msg.y <= panelY + panelHeight - 20) {
                    // ������Ϸ
                    if (selectedIndex != -1) {
                        // ������Ϸ�߼�
                        string exePath = games[selectedIndex].exePath;
                        ShellExecuteA(NULL, "open", exePath.c_str(), NULL, NULL, SW_SHOWNORMAL);
                        outtextxy(panelX + 120, panelY + panelHeight - 50, _T("��Ϸ������..."));
                        FlushBatchDraw();
                        Sleep(1000);
                    }
                }
            }
        }
    }
}
