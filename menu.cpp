#include "menu.h"
#include "dropDown.h"
#include "add.h"
#include"json.hpp"
#include<iostream>
#include<vector>
#include <easyx.h>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <shellapi.h>
const string savePath = "games.json";
using namespace std;
using json = nlohmann::json;

void GameLauncherUI::askGameInfo() {
    cout << "请输入游戏id：" << endl;
    string id;
    cin >> id;
    cout << "请输入游戏名称：" << endl;
    string title;
    cin >> title;
    cout << "请输入游戏封面路径：" << endl;
    string coverPath;
    cin >> coverPath;
    cout << "请输入游戏路径：" << endl;
    string exePath;
    cin >> exePath;
    int playCount = 0;
    cout << "请输入游戏标签：" << endl;
    vector<string> tags;
    string temptags;
    string temp;
    cin >> temptags;
    for (char c : temptags) {
        if (c == ',') {
            tags.push_back(temp);
            temp.clear();
        }
        else {
            temp += c;
        }
    }
    if (!temp.empty()) {
        tags.push_back(temp);
    }
    GameInfo gameinfo;
    gameinfo.id = id;
    gameinfo.title = title;
    gameinfo.coverPath = coverPath;
    gameinfo.exePath = exePath;
    gameinfo.playCount = playCount;
    gameinfo.tags = tags;

	json allGames = json::array();
    ifstream ifs(savePath);
    if (ifs.is_open()) {
        try {
            ifs >> allGames;
        }
        catch (...) {
            // 文件内容不是有效JSON，创建新数组
            allGames = json::array();
        }
        ifs.close();
    }
    json jGame = {
    { "id", gameinfo.id },
    { "title", gameinfo.title },
    { "coverPath", gameinfo.coverPath },
    { "exePath", gameinfo.exePath },
    { "lastPlayed", gameinfo.lastPlayed },
    { "playCount", gameinfo.playCount },
    { "tags", gameinfo.tags}
    };

	allGames.push_back(jGame);

	ofstream ofs(savePath);
	if (ofs.is_open()) {
		ofs << allGames.dump(4); // 美化输出
		ofs.close();
	}
}

bool GameLauncherUI::LoadGames() {
	allGames.clear(); // 清空之前的数据
    ifstream ifs(savePath,ios::binary);
	if (!ifs.is_open()) {
		cout << "无法打开游戏数据文件，可能文件不存在或路径错误。" << endl;
        return false;
	}
    try {
		ifs.seekg(0, ios::end);
        if (ifs.tellg() == 0) {
            cout << "游戏数据文件为空，请添加游戏。" << endl;
			ifs.close();
        }
		ifs.seekg(0);
        json j;
		ifs >> j;
	    allGames = j.get<vector<GameInfo>>();
        return true;
	}
	catch (const json::parse_error& e) {
		cout << "JSON解析错误: " << e.what() << endl;
	}
	catch (const json::type_error& e) {
		cout << "JSON类型错误: " << e.what() << endl;
	}
    catch (const std::exception& e) {
        cout << "其他错误: " << e.what() << endl;
    }
    ifs.close();
    return false;
}

vector<GameInfo> GameLauncherUI::GetPage(size_t pageIndex) {
	size_t start = pageIndex * pageSize;
	size_t end = start + pageSize;
	if (start >= allGames.size()) {
		return vector<GameInfo>(); // 返回空向量
	}
	end = min(end, allGames.size());
	return vector<GameInfo>(allGames.begin() + start, allGames.begin() + end);
}

vector<GameInfo> GameLauncherUI::GetCurrentPage() {
    return GetPage(currentPage);
}

size_t GameLauncherUI::pagePlus(size_t currentPage) {
	if (currentPage * pageSize + pageSize < allGames.size()) {
		return currentPage + 1;
	}
	return currentPage; // 如果已经是最后一页，则不变
}
size_t GameLauncherUI::pageSub(size_t currentPage) {
	if (currentPage > 0) {
		return currentPage - 1;
	}
	return currentPage; // 如果已经是第一页，则不变
}

void GameLauncherUI::LoadSampleData() {
    // 这里可以加载一些示例数据
    if (!LoadGames()) {
        cout << "加载游戏数据失败" << endl;
        allGames = {};
    }
	// 这里可以加载一些示例数据
	games = GetCurrentPage();
	addBtn = new addButton(930, 730, 50, 50);
}

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
    IMAGE coverImage; // 封面图片对象
	loadimage(&coverImage, game.coverPath.c_str(), panelWidth - 40, coverHeight);
	putimage(x + 20, y + 60, &coverImage);
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
    HWND hmain = initgraph(1000, 800);
    BeginBatchDraw();
    while (true) {
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

                        // 双击选择游戏
                        if (msg.message == WM_LBUTTONDOWN) {
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
                                selectedIndex = i;
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
                        string gamePath = games[selectedIndex].exePath;
                        HINSTANCE result = ShellExecute(NULL, "open", gamePath.c_str(), NULL, NULL, SW_SHOWNORMAL);
                        if ((int)result > 32) {
                            ifstream ifs(savePath, ios::binary);
                            json j;
                            ifs >> j;
                            ifs.close();
                            // 更新游戏数据
                            j[selectedIndex + 1]["lastPlayed"] = time(NULL);
                            j[selectedIndex + 1]["playCount"] = j[selectedIndex]["playCount"].get<int>() + 1;
                            ofstream ofs(savePath, ios::binary);
                            ofs << j.dump(4); // 美化输出
                            ofs.close();
                            games[selectedIndex].lastPlayed = time(NULL);
                            games[selectedIndex].playCount++;
                        }
                        else {
                            MessageBox(GetHWnd(), "无法启动游戏，请检查路径是否正确。", "错误", MB_OK | MB_ICONERROR);
                        }
						showDetails = false; // 启动游戏后关闭详情面板
                       
                    }
                }
                else if (showDetails)
                {
                    continue;
                }

                // ESC键处理
                if (msg.message == WM_KEYDOWN && msg.vkcode == VK_ESCAPE) {
                    if (showDetails) showDetails = false;
                    else break;
                }
            }

            if (addBtn->checkClick(msg.x, msg.y) && msg.message == WM_LBUTTONDOWN) {
                askGameInfo();
                LoadGames(); // 重新加载游戏数据
                games = GetCurrentPage(); // 更新当前页面游戏列表
            }
            if (msg.message == WM_KEYDOWN && msg.vkcode == VK_RIGHT) {
                currentPage = pagePlus(currentPage);
                games = GetCurrentPage();
            }
            if (msg.message == WM_KEYDOWN && msg.vkcode == VK_LEFT) {
                currentPage = pageSub(currentPage);
                games = GetCurrentPage();
            }

        // 渲染逻辑
        cleardevice();
        DrawMainView();
        if (showDetails && selectedIndex >= 0) {
            DrawDetailPanel(games[selectedIndex], theme);
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
