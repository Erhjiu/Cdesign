#include "menu.h"
#include "FilterBar.h"
#include <easyx.h>

// ������
void GameLauncherUI::DrawMainView()
{
	setbkcolor(theme.background);
	cleardevice();

	settextcolor(theme.text);
	settextstyle(28, 0, _T("΢���ź�"));
	outtextxy(50, 30, _T("��Ϸ������"));
	settextstyle(14, 0, _T("΢���ź�"));
	outtextxy(0, 50, _T("ɸѡ"));
	//FilterBar filterBar;
	//filterBar.Draw();
	
}
