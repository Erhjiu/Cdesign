#pragma once
#include "menu.h"
#include <easyx.h>
#include <vector>


class FilterBar
{
private :
	vector<string> categories = { "ȫ��" };
	string currentFilter = "ȫ��";

public:
	void Draw();
};
