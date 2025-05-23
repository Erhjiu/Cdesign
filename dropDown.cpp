#include "dropDown.h"
#include "menu.h"
#include <easyx.h>
void dropDown::Draw(const UITheme& theme)
{
	// 绘制下拉框背景
	setfillcolor(theme.cardBackground);
	fillrectangle(x, y, x + width, y + height);
	// 绘制下拉框边框
	setlinecolor(theme.text);
	rectangle(x, y, x + width, y + height);
	// 绘制选中的选项
	settextcolor(theme.text);
	settextstyle(14, 0, _T("微软雅黑"));
	outtextxy(x + 10, y + 10, options[selectedIndex].c_str());
	// 如果下拉框打开，绘制选项列表
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

//鼠标点击
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