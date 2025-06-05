#include <stdio.h>
#include <easyx.h>
#include "menu.h"
int sign = 1;
int set = 0;
int page = 1;
int max_page;
int game_num;
GameInfo games[100];
void lista(int set) {
	cleardevice();
	char info[64];
	//�����б�
	int row = 16;
	if (page == max_page) {
		row = game_num - (page - 1) * 6;
	}
	setlinecolor(RGB(65, 70, 85));
	setlinestyle(PS_SOLID, 2);
	for (int i = set; i < row + 1; i++) {
		line(0,i*40+50,400,i*40+50);
	}
	//�����Ϸ�ǳƺ��������
	settextcolor(RGB(240, 240, 240));
	settextstyle(36, 0, "�����ź�");
	for (int i = set; i < row; i++) {
		sprintf_s(info, "%s", games[i].title);
		outtextxy(20, i * 40 + 52, info);
		free(info);
		sprintf_s(info, "����: %d��", games[i].playCount);
		outtextxy(250, i * 40 + 52,info);
	}
	
}
void tablenet(int set) {
	cleardevice();
	//���Ʊ��,����ǳƺʹ���
	IMAGE img_cover;
	const int cardWidth = 220;
	const int cardHeight = 280;
	const int spacing = 30;
	const int startX = 50;
	const int startY = 100;
	char info[64];
	settextstyle(28, 0, "�����ź�");
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 3; j++) {
				if (set + i * 3 + j > game_num - 1) {
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
void change_button() {
	//���ư�ť
	setlinecolor(RGB(240,240,240));
	setlinestyle(PS_SOLID, 1);
	setfillcolor(RGB(160, 160, 160));
	fillroundrect(800, 30, 830, 50, 10, 10);
	line(815, 30, 815, 50);
	line(800, 40, 815, 40);
	for (int i = 1; i <= 2; i++) {
		line(800 + i * 5, 30, 800 + i * 5, 50);
	}
	for (int i = 1; i <= 3; i++) {
		line(815, 30 + 5 * i, 830, 30 + 5 * i);
	}
	//�ռ�����źŲ����ݣ�ͬʱ�ж��ź��Ƿ�����ָ������
	ExMessage msg = { 0 };
	while (1) {
		if (peekmessage(&msg, EX_MOUSE)&& msg.message == WM_LBUTTONDOWN) {
			if (msg.x < 815 && msg.x>800 && msg.y < 50 && msg.y>30) {
				sign = 1;
			}
			else if (msg.x < 830 && msg.x>815 && msg.y < 50 && msg.y>30) {
				sign = 0;
			}
		}
	}
	
}
void display_change() {
	//��ȡ����
	//ͳ����Ϸ����
	if (sign) {
		if (game_num % 6 == 0) {
			max_page = game_num / 6;
		}
		else {
			max_page = game_num / 6 + 1;
		}
		tablenet(set);
	}
	else if(!sign){
		if (game_num % 16 == 0) {
			max_page = game_num / 16;
		}
		else {
			max_page = game_num / 16 + 1;
		}
		lista(set);
	}
}
//�ж��Ƿ�ҳ������¼ҳ����ˢ�����
void page_up_or_down() {
	ExMessage msg = { 0 };
	while (1) {
		if (peekmessage(&msg, EX_KEY)) {
			if (msg.message == WM_KEYDOWN) {
				if (msg.vkcode == VK_LEFT) {
					page--;
					if (page <= 0) {
						break;
					}
					if (sign) {
						set = set - 6;
					}
					else if (!sign) {
						set = set - 16;
					}
				}
				else if (msg.vkcode == VK_RIGHT) {
					page++;
					if (page > max_page) {
						break;
					}
					if (sign) {
						set = set + 6;
					}
					else if (!sign) {
						set = set + 16;
					}
				}
			}
		}
		display_change();
	}
}