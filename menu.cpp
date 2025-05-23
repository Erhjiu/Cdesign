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

    // 背景面板
    solidroundrect(x, y, x + panelHeight, y + panelWidth, theme.cardBackground, theme.accent);

    // 标题
    settextcolor(theme.text);
    settextstyle(20, 0, _T("微软雅黑"));
    outtextxy(x + 20, y + 20, game.title.c_str());

    // 封面大图
    int coverWidth = 200;
    setfillcolor(RGB(255, 255, 255)); // !!!待修改，游戏封面
    fillrectangle(x + 20, y + 60, x + panelWidth - 20, y + 60 + coverWidth);

    // 游戏信息
    settextstyle(20, 0, _T("微软雅黑"));
    ostringstream info;
    info << "上次游玩时间: " << game.GetLastPlayedStr() << "\n"
         << "游玩次数: " << game.playCount << "\n"
         << "标签: ";
    for (const auto &tag : game.tags)
    {
        info << tag << " ";
    }
    RECT infoRect = {x + 20, y + 20, x + panelWidth - 20, y + 380};
    drawtext(info.str().c_str(), &infoRect, DT_LEFT | DT_WORDBREAK | DT_SINGLELINE);

    // 按钮
    DrawRoundRect(x + 100, y + panelHeight - 60, x + 300, y + panelHeight - 20, 10, theme.accent, theme.accent);
    settextcolor(RGB(255, 255, 255));
    outtextxy(x + 170, y + panelHeight - 50, _T("启动游戏"));

    // 关闭按钮
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
                        // 双击事件
                        showDetails = true;
                    }
                    lastClickTime = currentTime;
                }
            }
            //检查详情面板关闭按钮
            if (showDetails) {
                int panelWidth = 400, panelHeight = 500;
                int panelX = (getwidth() - panelWidth) / 2;
                int panelY = (getheight() - panelHeight) / 2;

                // 关闭按钮
                if (msg.x >= panelX + panelHeight - 30 && msg.x <= panelX + panelHeight - 20 && msg.y >= panelY + 20 && msg.y <= panelY + 30) {
                    showDetails = false;
                }
                //启动按钮
                if (msg.x >= panelX + 100 && msg.x <= panelX + 300 && msg.y >= panelY + panelHeight - 60 && msg.y <= panelY + panelHeight - 20) {
                    // 启动游戏
                    if (selectedIndex != -1) {
                        // 启动游戏逻辑
                        string exePath = games[selectedIndex].exePath;
                        ShellExecuteA(NULL, "open", exePath.c_str(), NULL, NULL, SW_SHOWNORMAL);
                        outtextxy(panelX + 120, panelY + panelHeight - 50, _T("游戏启动中..."));
                        FlushBatchDraw();
                        Sleep(1000);
                    }
                }
            }
        }
    }
}
