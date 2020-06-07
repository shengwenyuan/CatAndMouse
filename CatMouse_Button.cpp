#include <stdio.h>
#include <graphics.h>
#include "CatMouse_Button.h"

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

		if (myMouse.mkLButton == true) {
			if (judge == NULL) (*p)();
			else *judge = (*p)();
		}
	}

}

