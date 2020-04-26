#include <graphics.h>

#define CLASSIC_BUTTON_WID 280
#define CLASSIC_BUTTON_HEI 100

class Button
{
public:
	Button(int(*_p)(), LPCTSTR _pictureName, int _x_coordinate, int _y_coordinate, int* _judge = NULL, int _width = CLASSIC_BUTTON_WID, int _height = CLASSIC_BUTTON_HEI);
	void loadButton();
	void outputButton();	//借助easyX的IMAGE类产生按钮图像
	void mouseTouch(MOUSEMSG myMouse);	//检测鼠标状态，并在符合要求时做出反应
	

private:
	int width;
	int height;
	int x_coordinate;
	int y_coordinate;
	LPCTSTR pictureName;
	int(*p)();
	int* judge;
	IMAGE button;
};
