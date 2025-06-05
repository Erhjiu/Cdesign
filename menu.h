#pragma once
#include <easyx.h>
#include <string>
#include <vector>
#include "data.h"

// 绘制相关函数声明
void DrawFilterBar(class FilterBar &filterBar);
void DrawAddButton(class addButton &addBtn);
bool checkClick(int mouseX, int mouseY, int x, int y, int width, int height);

// 辅助函数
bool IsDoubleClick();