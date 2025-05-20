#pragma once
#include "menu.h"
#include <easyx.h>
#include <vector>


class FilterBar
{
private :
	vector<string> categories = { "全部" };
	string currentFilter = "全部";

public:
	void Draw();
};
