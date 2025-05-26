#include "menu.h"
#include "dropDown.h"
#include "add.h"
#include <easyx.h>
#include <algorithm>
#include <sstream>

using namespace std;

void GameLauncherUI::DrawDetailPanel(const GameInfo &game, const UITheme &theme)
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
    setfillcolor(RGB(rand() % 155 + 100, rand() % 155 + 100, rand() % 155 + 100));
    fillrectangle(x + 20, y + 60, x + panelWidth - 20, y + 60 + coverHeight);

    // ��Ϸ��Ϣ
    settextstyle(20, 0, "΢���ź�");
    std::ostringstream info;
    info << "�ϴ�����: " << game.GetLastPlayedStr() << "\n"
        << "�������: " << game.playCount << "\n"
        << "��ǩ: ";

    for (const auto& tag : game.tags) {
        info << tag << " ";
    }

    RECT infoRect = { x + 20, y + 280, x + panelWidth - 20, y + 380 };
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

void GameLauncherUI::run()
{
    initgraph(1000, 800);
    BeginBatchDraw();

    while (true) {
        addButton add;
        ExMessage msg;
        while (peekmessage(&msg, EX_MOUSE | EX_KEY)) {
            // ������ͣ״̬
            hoveredIndex = -1;
            const int cols = 3;
            const int cardWidth = 220;
            const int cardHeight = 280;
            const int spacing = 30;
            const int startX = 50;
            const int startY = 100;
     
            // ��������ͣ/���
            if (msg.message == WM_MOUSEMOVE || msg.message == WM_LBUTTONDOWN) {
                for (int i = 0; i < games.size(); i++) {
                    int row = i / cols;
                    int col = i % cols;
                    int x = startX + col * (cardWidth + spacing);
                    int y = startY + row * (cardHeight + spacing);

                    if (msg.x >= x && msg.x <= x + cardWidth &&
                        msg.y >= y && msg.y <= y + cardHeight) {
                        hoveredIndex = i;

                        // ����ѡ����Ϸ
                        if (msg.message == WM_LBUTTONDOWN) {
                            selectedIndex = i;

                            // ˫�����
                            static POINT lastClickPos = { 0, 0 };
                            static DWORD lastClickTime = 0;
                            DWORD currentTime = GetTickCount();

                            // ����Ƿ�����ͬλ����ʱ������
                            bool isDoubleClick = (currentTime - lastClickTime < 300) &&
                                (abs(msg.x - lastClickPos.x) < 10) &&
                                (abs(msg.y - lastClickPos.y) < 10);

                            if (isDoubleClick) {
                                showDetails = true;
                            }

                            lastClickTime = currentTime;
                            lastClickPos = { msg.x, msg.y };
                        }
                    }
                }
            }

            // ����������彻��
            if (showDetails && msg.message == WM_LBUTTONDOWN) {
                int panelWidth = 400, panelHeight = 500;
                int panelX = (getwidth() - panelWidth) / 2;
                int panelY = (getheight() - panelHeight) / 2;

                // �رհ�ť (���Ͻ�X)
                if (msg.x >= panelX + panelWidth - 30 && msg.x <= panelX + panelWidth - 10 &&
                    msg.y >= panelY + 10 && msg.y <= panelY + 30) {
                    showDetails = false;
                }
                // ������ť
                else if (msg.x >= panelX + 100 && msg.x <= panelX + 300 &&
                    msg.y >= panelY + panelHeight - 60 && msg.y <= panelY + panelHeight - 20) {
                    MessageBox(GetHWnd(), "��Ϸ������...", games[selectedIndex].title.c_str(), MB_OK);
                }
            }

            // ESC������
            if (msg.message == WM_KEYDOWN && msg.vkcode == VK_ESCAPE) {
                if (showDetails) showDetails = false;
                else break;
            }
            add.addRun(games);
        }

        // ��Ⱦ�߼�
        cleardevice();
        DrawMainView();
        if (showDetails && selectedIndex >= 0) {
            DrawDetailPanel(games[selectedIndex],theme);
        }
        FlushBatchDraw();

        // С���ӳٷ�ֹCPUռ�ù���
        static DWORD lastTime = GetTickCount();
        DWORD currentTime = GetTickCount();
        if (currentTime - lastTime < 10) {
            Sleep(10 - (currentTime - lastTime)); // ~100FPS����
        }
        lastTime = currentTime;
    }

    EndBatchDraw();
    closegraph();
}
