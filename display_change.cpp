#include <stdio.h>
#include <easyx.h>
#include "menu.h"
#include "data.h"
#include <string>

int sign = 1;
int set = 0;
int page = 0;
int max_page;
int game_num;
GameInfo games[100];

// ��ͼģʽö��
enum ViewMode
{
	GRID_VIEW,
	LIST_VIEW
};

// �б���ͼ�ĳ���
namespace ListLayout
{
	const int ITEM_HEIGHT = 40;
	const int START_X = 50;
	const int START_Y = 100;
	const int LIST_WIDTH = 900;
	const int ITEMS_PER_PAGE = 16;
}

void lista(int set)
{
	cleardevice();
	char info[64];
	// �����б�
	int row = 16;
	if (page == max_page)
	{
		row = game_num - (page - 1) * 6;
	}
	setlinecolor(RGB(65, 70, 85));
	setlinestyle(PS_SOLID, 2);
	for (int i = set; i < row + 1; i++)
	{
		line(0, i * 40 + 50, 400, i * 40 + 50);
	}
	// �����Ϸ�ǳƺ��������
	settextcolor(RGB(240, 240, 240));
	settextstyle(36, 0, "�����ź�");
	for (int i = set; i < row; i++)
	{
		sprintf_s(info, "%s", games[i].title);
		outtextxy(20, i * 40 + 52, info);
		free(info);
		sprintf_s(info, "����: %d��", games[i].playCount);
		outtextxy(250, i * 40 + 52, info);
	}
}
void tablenet(int set)
{
	cleardevice();
	// ���Ʊ��,����ǳƺʹ���
	IMAGE img_cover;
	const int cardWidth = 220;
	const int cardHeight = 280;
	const int spacing = 30;
	const int startX = 50;
	const int startY = 100;
	char info[64];
	settextstyle(28, 0, "�����ź�");
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (set + i * 3 + j > game_num - 1)
			{
				break;
			}
			roundrect(50 + j * (spacing + cardWidth), 100 + i * (spacing + cardHeight), 50 + cardWidth + j * (spacing + cardWidth), 100 + cardHeight + i * (spacing + cardHeight), 10, 10);
			sprintf_s(info, "%s", games[set + i * 3 + j].exePath);
			loadimage(&img_cover, info);
			free(info);
			putimage(50 + j * (30 + cardWidth), 100 + i * (30 + cardHeight), &img_cover);
			settextcolor(RGB(240, 240, 240));
			setbkmode(TRANSPARENT);
			sprintf_s(info, "%s", games[set + i * 3 + j].title);
			outtextxy(50 + j * (spacing + cardWidth) + 10, 100 + i * (spacing + cardHeight) + cardHeight * 0.7, info);
			free(info);
			settextcolor(RGB(180, 180, 180));
			sprintf_s(info, "����: %d��", games[set + i * 3 + j].playCount);
			outtextxy(50 + j * (spacing + cardWidth) + 10, 100 + i * (spacing + cardHeight) + cardHeight * 0.8, info);
			free(info);
		}
	}
}
void change_button()
{
	// ���ư�ť
	setlinecolor(RGB(240, 240, 240));
	setlinestyle(PS_SOLID, 1);
	setfillcolor(RGB(160, 160, 160));
	fillroundrect(800, 30, 830, 50, 10, 10);
	line(815, 30, 815, 50);
	line(800, 40, 815, 40);
	for (int i = 1; i <= 2; i++)
	{
		line(800 + i * 5, 30, 800 + i * 5, 50);
	}
	for (int i = 1; i <= 3; i++)
	{
		line(815, 30 + 5 * i, 830, 30 + 5 * i);
	}
	// �ռ�����źŲ����ݣ�ͬʱ�ж��ź��Ƿ�����ָ������
	ExMessage msg = {0};
	while (1)
	{
		if (peekmessage(&msg, EX_MOUSE) && msg.message == WM_LBUTTONDOWN)
		{
			if (msg.x < 815 && msg.x > 800 && msg.y < 50 && msg.y > 30)
			{
				sign = 1;
			}
			else if (msg.x < 830 && msg.x > 815 && msg.y < 50 && msg.y > 30)
			{
				sign = 0;
			}
		}
	}
}
void display_change()
{
	// ��ȡ����
	// ͳ����Ϸ����
	if (sign)
	{
		if (game_num % 6 == 0)
		{
			max_page = game_num / 6;
		}
		else
		{
			max_page = game_num / 6 + 1;
		}
		tablenet(set);
	}
	else if (!sign)
	{
		if (game_num % 16 == 0)
		{
			max_page = game_num / 16;
		}
		else
		{
			max_page = game_num / 16 + 1;
		}
		lista(set);
	}
}

// ������ͼ�л���ť
void DrawViewToggleButton(int x, int y, ViewMode currentMode)
{
	// ���ư�ť����
	setfillcolor(RGB(160, 160, 160));
	setlinecolor(RGB(240, 240, 240));
	fillroundrect(x, y, x + 30, y + 20, 5, 5);

	if (currentMode == GRID_VIEW)
	{
		// ��������ͼ��
		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < 2; j++)
			{
				rectangle(x + 5 + j * 10, y + 5 + i * 10, x + 12 + j * 10, y + 12 + i * 10);
			}
		}
	}
	else
	{
		// �����б�ͼ��
		for (int i = 0; i < 3; i++)
		{
			line(x + 5, y + 5 + i * 6, x + 25, y + 5 + i * 6);
		}
	}
}

// �����б���ͼ�еĵ�����Ŀ
void DrawListItem(const GameInfo &game, int x, int y, bool isHovered, bool isSelected)
{
	// ������Ŀ����
	COLORREF bgColor = isSelected ? RGB(70, 130, 180) : isHovered ? RGB(60, 60, 70)
																  : RGB(45, 45, 55);
	setfillcolor(bgColor);
	solidrectangle(x, y, x + ListLayout::LIST_WIDTH, y + ListLayout::ITEM_HEIGHT);

	// ������Ϸ����
	settextcolor(RGB(240, 240, 240));
	settextstyle(20, 0, "΢���ź�");
	outtextxy(x + 20, y + 10, game.title.c_str());

	// �����������
	char playInfo[32];
	sprintf_s(playInfo, "����: %d��", game.playCount);
	outtextxy(x + ListLayout::LIST_WIDTH - 150, y + 10, playInfo);
}

// �����б���ͼ
void DrawListView(const std::vector<GameInfo> &games, int hoveredIndex, int selectedIndex, int startIndex)
{
	int y = ListLayout::START_Y;

	for (int i = startIndex; i < min(startIndex + ListLayout::ITEMS_PER_PAGE, (int)games.size()); i++)
	{
		DrawListItem(games[i],
					 ListLayout::START_X,
					 y,
					 i == hoveredIndex,
					 i == selectedIndex);
		y += ListLayout::ITEM_HEIGHT;
	}
}

// �����ͼ�л���ť���
bool CheckViewToggleClick(int mouseX, int mouseY)
{
	return mouseX >= 800 && mouseX <= 830 &&
		   mouseY >= 30 && mouseY <= 50;
}

// ��ȡ�б���ͼ�б��������Ŀ����
int GetListItemIndex(int mouseX, int mouseY, int startIndex)
{
	// ����Ƿ����б�������
	if (mouseX < ListLayout::START_X ||
		mouseX > ListLayout::START_X + ListLayout::LIST_WIDTH ||
		mouseY < ListLayout::START_Y)
	{
		return -1;
	}

	// ����������б���ʼλ�õ�ƫ��
	int relativeY = mouseY - ListLayout::START_Y;

	// ���������ǵڼ�����Ŀ
	int clickedItem = relativeY / ListLayout::ITEM_HEIGHT;

	// ����ʵ�ʵ���Ŀ���������ǵ�ǰҳ����ʼ������
	int actualIndex = startIndex + clickedItem;

	// ȷ����������Ч��Χ��
	if (clickedItem >= 0 && clickedItem < ListLayout::ITEMS_PER_PAGE)
	{
		return actualIndex;
	}

	return -1;
}