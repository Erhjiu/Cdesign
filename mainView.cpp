#include "menu.h"
#include "FilterBar.h"
#include <easyx.h>

// 主界面
void GameLauncherUI::DrawMainView()
{
	setbkcolor(theme.background);
	cleardevice();

	settextcolor(theme.text);
	settextstyle(28, 0, _T("微软雅黑"));
	outtextxy(50, 30, _T("游戏启动器"));
	settextstyle(14, 0, _T("微软雅黑"));
	outtextxy(0, 50, _T("筛选"));
	//FilterBar filterBar;
	//filterBar.Draw();
	
}
