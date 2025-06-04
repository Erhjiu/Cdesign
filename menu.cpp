#include "menu.h"
#include "dropDown.h"
#include "add.h"
#include "json.hpp"
#include <iostream>
#include <vector>
#include <easyx.h>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <shellapi.h>
#include "FilterBar.h"
const string savePath = "games.json";
using namespace std;
using json = nlohmann::json;

inline void GameLauncherUI::DrawRoundRect(int x1, int y1, int x2, int y2, int radius, COLORREF fill, COLORREF outline)
{
    setfillcolor(fill);
    fillroundrect(x1, y1, x2, y2, radius, radius);
    setlinecolor(outline);
    roundrect(x1, y1, x2, y2, radius, radius);
}

GameLauncherUI::GameLauncherUI()
{
    initgraph(1000, 800);
    // ��ʼ����Ա����
    selectedIndex = -1;
    hoveredIndex = -1;
    showDetails = false;
    addBtn = nullptr;
    currentPage = 0;
    pageSize = 6;
    filterBar = nullptr;

    // ����ʾ������
    LoadSampleData();

    // ����FilterBar����
    filterBar = new FilterBar(*this);
}

GameLauncherUI::~GameLauncherUI()
{
    // ������Դ
    if (addBtn)
    {
        delete addBtn;
        addBtn = nullptr;
    }

    if (filterBar)
    {
        delete filterBar;
        filterBar = nullptr;
    }

    // �������
    games.clear();
    allGames.clear();

    // ����״̬
    selectedIndex = -1;
    hoveredIndex = -1;
    showDetails = false;
    currentPage = 0;
    pageSize = 6;
    closegraph();
}


void GameLauncherUI::askGameInfo()
{
    cout << "��������Ϸid:" << endl;
    string id;
    cin >> id;
    cout << "��������Ϸ���ƣ�" << endl;
    string title;
    cin >> title;
    cout << "��������Ϸ����·����" << endl;
    string coverPath;
    cin >> coverPath;
    cout << "��������Ϸ·����" << endl;
    string exePath;
    cin >> exePath;
    int playCount = 0;
    cout << "��������Ϸ��ǩ��" << endl;
    vector<string> tags;
    string temptags;
    string temp;
    cin >> temptags;
    for (char c : temptags)
    {
        if (c == ',')
        {
            tags.push_back(temp);
            temp.clear();
        }
        else
        {
            temp += c;
        }
    }
    if (!temp.empty())
    {
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
    if (ifs.is_open())
    {
        try
        {
            ifs >> allGames;
        }
        catch (...)
        {
            // �ļ����ݲ�����ЧJSON������������
            allGames = json::array();
        }
        ifs.close();
    }
    json jGame = {
        {"id", gameinfo.id},
        {"title", gameinfo.title},
        {"coverPath", gameinfo.coverPath},
        {"exePath", gameinfo.exePath},
        {"lastPlayed", gameinfo.lastPlayed},
        {"playCount", gameinfo.playCount},
        {"tags", gameinfo.tags}};

    allGames.push_back(jGame);

    ofstream ofs(savePath);
    if (ofs.is_open())
    {
        ofs << allGames.dump(4); // �������
        ofs.close();
    }
}

bool GameLauncherUI::LoadGames()
{
    allGames.clear(); // ���֮ǰ������
    ifstream ifs(savePath, ios::binary);
    if (!ifs.is_open())
    {
        cout << "�޷�����Ϸ�����ļ��������ļ������ڻ�·������" << endl;
        return false;
    }
    try
    {
        ifs.seekg(0, ios::end);
        if (ifs.tellg() == 0)
        {
            cout << "��Ϸ�����ļ�Ϊ�գ��������Ϸ��" << endl;
            ifs.close();
        }
        ifs.seekg(0);
        json j;
        ifs >> j;
        allGames = j.get<vector<GameInfo>>();
        return true;
    }
    catch (const json::parse_error &e)
    {
        cout << "JSON��������: " << e.what() << endl;
    }
    catch (const json::type_error &e)
    {
        cout << "JSON���ʹ���: " << e.what() << endl;
    }
    catch (const std::exception &e)
    {
        cout << "��������: " << e.what() << endl;
    }
    ifs.close();
    return false;
}

vector<GameInfo> GameLauncherUI::GetPage(size_t pageIndex)
{
    size_t start = pageIndex * pageSize;
    size_t end = start + pageSize;
    if (start >= allGames.size())
    {
        return vector<GameInfo>(); // ���ؿ�����
    }
    end = min(end, allGames.size());
    return vector<GameInfo>(allGames.begin() + start, allGames.begin() + end);
}

vector<GameInfo> GameLauncherUI::GetCurrentPage()
{
    return GetPage(currentPage);
}

size_t GameLauncherUI::pagePlus(size_t currentPage)
{
    if (currentPage * pageSize + pageSize < allGames.size())
    {
        return currentPage + 1;
    }
    return currentPage;
}
size_t GameLauncherUI::pageSub(size_t currentPage)
{
    if (currentPage > 0)
    {
        return currentPage - 1;
    }
    return currentPage;
}

void GameLauncherUI::LoadSampleData()
{
    if (!LoadGames())
    {
        cout << "������Ϸ����ʧ��" << endl;
        allGames = {};
    }
    games = GetCurrentPage();
    addBtn = new addButton(930, 730, 50, 50);
}

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
    IMAGE coverImage;
    if (game.coverPath.empty())
    {
        setfillcolor(theme.cardBackground);
        fillrectangle(x + 5, y + 60, x + panelWidth - 5, y + 60 + coverHeight);
    }
    else
    {
        coverImage = IMAGE();
    }
    loadimage(&coverImage, game.coverPath.c_str(), panelWidth - 10, coverHeight, true);
    putimage(x + 5, y + 60, &coverImage);
    // ��Ϸ��Ϣ
    settextstyle(20, 0, "΢���ź�");
    std::ostringstream info;
    info << "�ϴ�����: " << game.GetLastPlayedStr() << "\n"
         << "�������: " << game.playCount << "\n"
         << "��ǩ: ";

    for (const auto &tag : game.tags)
    {
        info << tag << " ";
    }

    RECT infoRect = {x + 20, y + 280, x + panelWidth - 20, y + 380};
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

    BeginBatchDraw();

    while (true)
    {
        ExMessage msg;
        while (peekmessage(&msg, EX_MOUSE | EX_KEY))
        {
            // ������ͣ״̬
            hoveredIndex = -1;
            const int cols = 3;
            const int cardWidth = 220;
            const int cardHeight = 280;
            const int spacing = 30;
            const int startX = 50;
            const int startY = 100;

            // ��������ͣ/���
            if (msg.message == WM_MOUSEMOVE || msg.message == WM_LBUTTONDOWN)
            {
                for (int i = 0; i < games.size(); i++)
                {
                    int row = i / cols;
                    int col = i % cols;
                    int x = startX + col * (cardWidth + spacing);
                    int y = startY + row * (cardHeight + spacing);

                    if (msg.x >= x && msg.x <= x + cardWidth &&
                        msg.y >= y && msg.y <= y + cardHeight)
                    {
                        hoveredIndex = i;

                        // ˫��ѡ����Ϸ
                        if (msg.message == WM_LBUTTONDOWN)
                        {
                            // ˫�����
                            static POINT lastClickPos = {0, 0};
                            static DWORD lastClickTime = 0;
                            DWORD currentTime = GetTickCount();

                            // ����Ƿ�����ͬλ����ʱ������
                            bool isDoubleClick = (currentTime - lastClickTime < 300) &&
                                                 (abs(msg.x - lastClickPos.x) < 10) &&
                                                 (abs(msg.y - lastClickPos.y) < 10);

                            if (isDoubleClick)
                            {
                                showDetails = true;
                                selectedIndex = i;
                            }
                            lastClickTime = currentTime;
                            lastClickPos = {msg.x, msg.y};
                        }
                    }
                }
            }

            // ����������彻��
            if (showDetails && msg.message == WM_LBUTTONDOWN)
            {
                int panelWidth = 400, panelHeight = 500;
                int panelX = (getwidth() - panelWidth) / 2;
                int panelY = (getheight() - panelHeight) / 2;

                // �رհ�ť (���Ͻ�X)
                if (msg.x >= panelX + panelWidth - 30 && msg.x <= panelX + panelWidth - 10 &&
                    msg.y >= panelY + 10 && msg.y <= panelY + 30)
                {
                    showDetails = false;
                    selectedIndex = -1; // ���ѡ��״̬
                }
                // ������ť
                else if (msg.x >= panelX + 100 && msg.x <= panelX + 300 &&
                         msg.y >= panelY + panelHeight - 60 && msg.y <= panelY + panelHeight - 20)
                {
                    MessageBox(GetHWnd(), "��Ϸ������...", games[selectedIndex].title.c_str(), MB_OK);
                    string gamePath = games[selectedIndex].exePath;
                    HINSTANCE result = ShellExecute(NULL, "open", gamePath.c_str(), NULL, NULL, SW_SHOWNORMAL);
                    if ((int)result > 32)
                    {
                        ifstream ifs(savePath, ios::binary);
                        json j;
                        ifs >> j;
                        ifs.close();
                        // ������Ϸ����
                        j[selectedIndex]["lastPlayed"] = time(NULL);
                        j[selectedIndex]["playCount"] = j[selectedIndex]["playCount"].get<int>() + 1;
                        ofstream ofs(savePath, ios::binary);
                        ofs << j.dump(4); // �������
                        ofs.close();
                        games[selectedIndex].lastPlayed = time(NULL);
                        games[selectedIndex].playCount++;
                    }
                    else
                    {
                        MessageBox(GetHWnd(), "�޷�������Ϸ������·���Ƿ���ȷ��", "����", MB_OK | MB_ICONERROR);
                    }
                    showDetails = false; // ������Ϸ��ر��������
                }
            }
            else if (showDetails)
            {
                continue;
            }

            // ESC������
            if (msg.message == WM_KEYDOWN && msg.vkcode == VK_ESCAPE)
            {
                if (showDetails)
                    showDetails = false;
                else
                    break;
            }
        }

        if (addBtn->checkClick(msg.x, msg.y) && msg.message == WM_LBUTTONDOWN)
        {
            askGameInfo();
            LoadGames();              // ���¼�����Ϸ����
            games = GetCurrentPage(); // ���µ�ǰҳ����Ϸ�б�
        }
        if (msg.message == WM_KEYDOWN && msg.vkcode == VK_RIGHT)
        {
            currentPage = pagePlus(currentPage);
            games = GetCurrentPage();
        }
        if (msg.message == WM_KEYDOWN && msg.vkcode == VK_LEFT)
        {
            currentPage = pageSub(currentPage);
            games = GetCurrentPage();
        }
        // ��Ⱦ�߼�
        cleardevice();
        DrawMainView();
        if (showDetails && selectedIndex >= 0)
        {
            DrawDetailPanel(games[selectedIndex], theme);
        }
        FlushBatchDraw();

        // С���ӳٷ�ֹCPUռ�ù���
        static DWORD lastTime = GetTickCount();
        DWORD currentTime = GetTickCount();
        if (currentTime - lastTime < 10)
        {
            Sleep(10 - (currentTime - lastTime)); // ~100FPS����
        }
        lastTime = currentTime;
    }

    EndBatchDraw();

}

