#include <stdio.h>
#include <graphics.h>
#include "CatMouse_Button.h"

#define BUTTON_PRESSED_DIS 10	//鼠标悬浮于按钮之上时，原按钮图片向右、下方向各平移此距离以贴合3D按下效果

Button::Button(int(*_p)(), LPCTSTR _pictureName, int _x_coordinate, int _y_coordinate,int* _judge,  int _width, int _height) {	//_p为每个button的special function
	x_coordinate = _x_coordinate;
	y_coordinate = _y_coordinate;
	width = _width;
	height = _height;
	pictureName = _pictureName;
	p = _p;
	judge = _judge;
	button = IMAGE(width, height);
	loadimage(&button, pictureName, width, height);
}

void Button::outputButton() {
	putimage(x_coordinate, y_coordinate, &button);
}

void Button::mouseTouch(MOUSEMSG myMouse) {
	//struct MOUSEMSG myMouse;

	//myMouse = GetMouseMsg();
	if ( myMouse.x > x_coordinate && myMouse.x < (x_coordinate + width)
		&& myMouse.y > y_coordinate && myMouse.y < (y_coordinate + height) ) {	//限制范围

		rectangle(x_coordinate, y_coordinate, (x_coordinate + width), (y_coordinate + height));
		//FlushBatchDraw();

		if (myMouse.mkLButton == true) {
			if (judge == NULL) (*p)();
			else *judge = (*p)();
		}
	}

}

