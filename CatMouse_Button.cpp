#include <stdio.h>
#include <graphics.h>
#include "CatMouse_Button.h"

#define BUTTON_PRESSED_DIS 10	//��������ڰ�ť֮��ʱ��ԭ��ťͼƬ���ҡ��·����ƽ�ƴ˾���������3D����Ч��

Button::Button(int(*_p)(), LPCTSTR _pictureName, int _x_coordinate, int _y_coordinate,int* _judge,  int _width, int _height) {	//_pΪÿ��button��special function
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
		&& myMouse.y > y_coordinate && myMouse.y < (y_coordinate + height) ) {	//���Ʒ�Χ

		rectangle(x_coordinate, y_coordinate, (x_coordinate + width), (y_coordinate + height));
		//FlushBatchDraw();

		if (myMouse.mkLButton == true) {
			*judge = (*p)();
		}
	}

}
