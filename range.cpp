#include"menu.h"

//������
void GameLauncherUI::DrawMainView()
{
	setbkcolor(theme.background);
	cleardevice();

	settextcolor(theme.text);
	settextstyle(28, 0, _T("΢���ź�"));
	outtextxy(50, 30, _T("��Ϸ������"));



}
