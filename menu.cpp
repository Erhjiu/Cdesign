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

    // 背景面板
    DrawRoundRect(x, y, x + panelWidth, y + panelHeight, 15, theme.cardBackground, theme.accent);

    // 标题
    settextcolor(theme.text);
    settextstyle(24, 0, "微软雅黑");
    outtextxy(x + 20, y + 20, game.title.c_str());

    // 封面大图
    int coverHeight = 200;
    setfillcolor(RGB(rand() % 155 + 100, rand() % 155 + 100, rand() % 155 + 100));
    fillrectangle(x + 20, y + 60, x + panelWidth - 20, y + 60 + coverHeight);

    // 游戏信息
    settextstyle(20, 0, "微软雅黑");
    std::ostringstream info;
    info << "上次游玩: " << game.GetLastPlayedStr() << "\n"
        << "游玩次数: " << game.playCount << "\n"
        << "标签: ";

    for (const auto& tag : game.tags) {
        info << tag << " ";
    }

    RECT infoRect = { x + 20, y + 280, x + panelWidth - 20, y + 380 };
    drawtext(info.str().c_str(), &infoRect, DT_LEFT | DT_TOP | DT_WORDBREAK);

    // 按钮
    DrawRoundRect(x + 100, y + panelHeight - 60, x + 300, y + panelHeight - 20, 10, theme.accent, theme.accent);
    settextcolor(RGB(255, 255, 255));
    outtextxy(x + 170, y + panelHeight - 50, "启动游戏");

    // 关闭按钮
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
            // 更新悬停状态
            hoveredIndex = -1;
            const int cols = 3;
            const int cardWidth = 220;
            const int cardHeight = 280;
            const int spacing = 30;
            const int startX = 50;
            const int startY = 100;
     
            // 检查鼠标悬停/点击
            if (msg.message == WM_MOUSEMOVE || msg.message == WM_LBUTTONDOWN) {
                for (int i = 0; i < games.size(); i++) {
                    int row = i / cols;
                    int col = i % cols;
                    int x = startX + col * (cardWidth + spacing);
                    int y = startY + row * (cardHeight + spacing);

                    if (msg.x >= x && msg.x <= x + cardWidth &&
                        msg.y >= y && msg.y <= y + cardHeight) {
                        hoveredIndex = i;

                        // 单击选择游戏
                        if (msg.message == WM_LBUTTONDOWN) {
                            selectedIndex = i;

                            // 双击检测
                            static POINT lastClickPos = { 0, 0 };
                            static DWORD lastClickTime = 0;
                            DWORD currentTime = GetTickCount();

                            // 检查是否在相同位置且时间间隔短
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

            // 处理详情面板交互
            if (showDetails && msg.message == WM_LBUTTONDOWN) {
                int panelWidth = 400, panelHeight = 500;
                int panelX = (getwidth() - panelWidth) / 2;
                int panelY = (getheight() - panelHeight) / 2;

                // 关闭按钮 (右上角X)
                if (msg.x >= panelX + panelWidth - 30 && msg.x <= panelX + panelWidth - 10 &&
                    msg.y >= panelY + 10 && msg.y <= panelY + 30) {
                    showDetails = false;
                }
                // 启动按钮
                else if (msg.x >= panelX + 100 && msg.x <= panelX + 300 &&
                    msg.y >= panelY + panelHeight - 60 && msg.y <= panelY + panelHeight - 20) {
                    MessageBox(GetHWnd(), "游戏启动中...", games[selectedIndex].title.c_str(), MB_OK);
                }
            }

            // ESC键处理
            if (msg.message == WM_KEYDOWN && msg.vkcode == VK_ESCAPE) {
                if (showDetails) showDetails = false;
                else break;
            }
            add.addRun(games);
        }

        // 渲染逻辑
        cleardevice();
        DrawMainView();
        if (showDetails && selectedIndex >= 0) {
            DrawDetailPanel(games[selectedIndex],theme);
        }
        FlushBatchDraw();

        // 小幅延迟防止CPU占用过高
        static DWORD lastTime = GetTickCount();
        DWORD currentTime = GetTickCount();
        if (currentTime - lastTime < 10) {
            Sleep(10 - (currentTime - lastTime)); // ~100FPS上限
        }
        lastTime = currentTime;
    }

    EndBatchDraw();
    closegraph();
}
