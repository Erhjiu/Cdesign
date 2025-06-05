#include "Library.h"
#include "add.h"
#include <iostream>
#include <vector>
#include <easyx.h>
#include <algorithm>


vector<GameInfo> GameLauncherUI::GetPage(size_t pageIndex)
{
    size_t start = pageIndex * pageSize;
    size_t end = start + pageSize;
    if (start >= filterBar.filterGames.size())
    {
        return vector<GameInfo>(); // 返回空向量
    }
    end = min(end, filterBar.filterGames.size());
    return vector<GameInfo>(filterBar.filterGames.begin() + start, filterBar.filterGames.begin() + end);
}

vector<GameInfo> GameLauncherUI::GetCurrentPage()
{
    return GetPage(currentPage);
}

size_t GameLauncherUI::pagePlus(size_t currentPage)
{
    if (currentPage * pageSize + pageSize < filterBar.filterGames.size())
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


