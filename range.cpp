#include"menu.h"

//主界面
void GameLauncherUI::DrawMainView()
{
	setbkcolor(theme.background);
	cleardevice();

	settextcolor(theme.text);
	settextstyle(28, 0, _T("微软雅黑"));
	outtextxy(50, 30, _T("游戏启动器"));



}
