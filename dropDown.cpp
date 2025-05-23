#include "dropDown.h"
#include "menu.h"
#include <easyx.h>
void dropDown::Draw(const UITheme& theme)
{
	// ���������򱳾�
	setfillcolor(theme.cardBackground);
	fillrectangle(x, y, x + width, y + height);
	// ����������߿�
	setlinecolor(theme.text);
	rectangle(x, y, x + width, y + height);
	// ����ѡ�е�ѡ��
	settextcolor(theme.text);
	settextstyle(14, 0, _T("΢���ź�"));
	outtextxy(x + 10, y + 10, options[selectedIndex].c_str());
	// ���������򿪣�����ѡ���б�
	if (isOpen)
	{
		for (size_t i = 0; i < options.size(); ++i)
		{
			int optionY = y + height + i * height;
			setfillcolor(theme.cardBackground);
			fillrectangle(x, optionY, x + width, optionY + height);
			settextcolor(theme.text);
			outtextxy(x + 10, optionY + 10, options[i].c_str());
			if (i == selectedIndex)
			{
				setfillcolor(theme.cardSelected);
				fillrectangle(x, optionY, x + width, optionY + height);
			}
		}
	}
}

//�����
void dropDown::clickDropDown(int mouseX, int mouseY, dropDown& dropDownMenu)
{
	if (mouseX >= dropDownMenu.x && mouseX <= dropDownMenu.x + dropDownMenu.width && mouseY >= dropDownMenu.y && mouseY <= dropDownMenu.y + dropDownMenu.height) {
		dropDownMenu.Toggle();
	}
	if(dropDownMenu.isOpen){
		for (size_t i = 0; i < dropDownMenu.options.size(); ++i)
		{
			int optionY = dropDownMenu.y + dropDownMenu.height + i * dropDownMenu.height;
			if (mouseX >= dropDownMenu.x && mouseX <= dropDownMenu.x + dropDownMenu.width && mouseY >= optionY && mouseY <= optionY + dropDownMenu.height) {
				dropDownMenu.SelectOption(i);
				break;
			}
		}
	}
	if (mouseX < dropDownMenu.x || mouseX > dropDownMenu.x + dropDownMenu.width || mouseY < dropDownMenu.y || mouseY > dropDownMenu.y + dropDownMenu.height) {
		dropDownMenu.isOpen = false;
	}
}