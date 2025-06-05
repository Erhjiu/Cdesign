#pragma once
#include <vector>
#include "data.h"

// 视图模式枚举
enum ViewMode
{
    GRID_VIEW,
    LIST_VIEW
};

// 列表视图的常量
namespace ListLayout
{
    constexpr int ITEM_HEIGHT = 40;
    constexpr int START_X = 50;
    constexpr int START_Y = 100;
    constexpr int LIST_WIDTH = 900;
    constexpr int ITEMS_PER_PAGE = 16;
}

// 函数声明
void DrawViewToggleButton(int x, int y, ViewMode currentMode);
void DrawListItem(const GameInfo &game, int x, int y, bool isHovered, bool isSelected);
void DrawListView(const std::vector<GameInfo> &games, int hoveredIndex, int selectedIndex, int startIndex);
bool CheckViewToggleClick(int mouseX, int mouseY);
int GetListItemIndex(int mouseX, int mouseY, int startIndex);
void DrawGameCard(const GameInfo &game, int x, int y, int width, int height, bool isHovered, bool isSelected);