#include <Windows.h>
#include <graphics.h>
#include <stdio.h>
#include "CatMouse_Button.h"
#include <math.h>
#include <Mmsystem.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#pragma comment(lib,"winmm.lib")

#define GRAPH_WIDTH 1800
#define GRAPH_HEIGHT 1200
#define BUTTON_GAP 150
#define SPEAKER_SIZE 60
#define JERRY_GRAPH_SIZE 60
#define TOM_GRAPH_SIZE 100
#define PLAYER_SPEED 10
#define RANDOM_MOUSE_SPEED 3
#define EASY_MOUSE_RUN_SPEED 5 
#define MOUSE_TOTAL_NUMBER 5

typedef struct mouse {
	int x_coordinate;
	int y_coordinate;
	int width;
	int height;
	int speed;
	IMAGE* frame1;
	IMAGE* frame1_logic;
	IMAGE* frame2;
	IMAGE* frame2_logic;
	IMAGE* frame3;
	IMAGE* frame3_logic;
	struct mouse* nextPtr;
}Mouse;
typedef Mouse* MousePtr;
typedef Mouse** MouseDoublePtr;

typedef struct cat
{
	int x_coordinate;
	int y_coordinate;
	int width;
	int height;
	int speed;
}Cat;
typedef Cat* CatPtr;

void menu();
int game_running();
int sets_running();
int rank_running();
int music_running();
int mute_running();
int quit_running();

void mouseInitialLink(int number, MousePtr * micePtr);
int isEmpty_forMouseLink(MousePtr goodday);
void mouseNodeFree(MouseDoublePtr* mouseKing, MouseDoublePtr* unluckOne);
void outputMouseImage(const Mouse mouse, IMAGE* pic, IMAGE* pic_logic);
void mouseRandomRun(MousePtr it);
void mouseLineEscape(MousePtr mouse, const Cat cat);
void catControl(CatPtr it);
void catCatch(MousePtr* headMice, MousePtr* unluckOne, const Cat cat);

int main(void) {
	initgraph(GRAPH_WIDTH, GRAPH_HEIGHT);
	
	menu();

	closegraph();
	return 1;
}

void menu() {
	int continueOrNot = 1;
	int playMusicOrNot = 1;
	struct MOUSEMSG myMouse;
	setlinecolor(LIGHTMAGENTA);
	setlinestyle(PS_SOLID, 4);

	mciSendString(L"open .\\musics\\test.mp3", NULL, 0, 0);
	mciSendString(L"play .\\musics\\test.mp3 repeat", NULL, 0, 0);

	BeginBatchDraw();

	IMAGE map(GRAPH_WIDTH, GRAPH_HEIGHT);
	loadimage(&map, L".\\pictures\\map_grassland.jpg", GRAPH_WIDTH, GRAPH_HEIGHT);

	//IMAGE title(600, 300);
	//settextcolor(CYAN);
	//

	Button b1(game_running, L".\\pictures\\start_button.png", (GRAPH_WIDTH-CLASSIC_BUTTON_WID)/2, (GRAPH_HEIGHT - CLASSIC_BUTTON_HEI) / 2);
	Button b2(sets_running, L".\\pictures\\setting_button.png", (GRAPH_WIDTH - CLASSIC_BUTTON_WID) / 2, (GRAPH_HEIGHT - CLASSIC_BUTTON_HEI) / 2 + BUTTON_GAP);
	Button b3(rank_running, L".\\pictures\\rank_button.png", (GRAPH_WIDTH - CLASSIC_BUTTON_WID) / 2, (GRAPH_HEIGHT - CLASSIC_BUTTON_HEI) / 2 + BUTTON_GAP*2);
	Button sound_button(music_running, L".\\pictures\\sound_button.jpg", 30, 30, &playMusicOrNot, SPEAKER_SIZE, SPEAKER_SIZE);
	Button mute_button(mute_running, L".\\pictures\\mute_button.png", 30, 30, &playMusicOrNot, SPEAKER_SIZE, SPEAKER_SIZE);
	Button b5(quit_running, L".\\pictures\\start_button.png", (GRAPH_WIDTH - CLASSIC_BUTTON_WID) / 2, (GRAPH_HEIGHT - CLASSIC_BUTTON_HEI) / 2 + BUTTON_GAP*3, &continueOrNot);

	while (1) {
		putimage(0, 0, &map);
		//outtext("catch");

		b1.outputButton();
		b2.outputButton();
		b3.outputButton();
		if (playMusicOrNot == 1) {
			sound_button.outputButton();
		}
		else {
			mute_button.outputButton();
		}
		b5.outputButton();

		//FlushBatchDraw();
		myMouse = GetMouseMsg();
		b1.mouseTouch(myMouse);
		b2.mouseTouch(myMouse);
		b3.mouseTouch(myMouse);
		if (playMusicOrNot == 1) {
			sound_button.mouseTouch(myMouse);
		}
		else {
			mute_button.mouseTouch(myMouse);
		}
		b5.mouseTouch(myMouse);

		if (continueOrNot == 0) break;

		FlushBatchDraw();
	}

	EndBatchDraw();
}

int game_running() {
	srand(time(NULL));
	int score = 0;
	int choice = 0;
	int mouseNumber = MOUSE_TOTAL_NUMBER;
	
	MousePtr mouseKing = NULL;
	mouseInitialLink(mouseNumber, &mouseKing);	//老鼠初始化完成

	Cat player_cat;
	player_cat.width = TOM_GRAPH_SIZE;
	player_cat.height = TOM_GRAPH_SIZE;
	player_cat.x_coordinate = (GRAPH_WIDTH - TOM_GRAPH_SIZE) / 2;
	player_cat.y_coordinate = (GRAPH_HEIGHT - TOM_GRAPH_SIZE) / 2;
	player_cat.speed = PLAYER_SPEED;
	IMAGE cat1(player_cat.width, player_cat.height);
	loadimage(&cat1, L".\\pictures\\cat.jpg", player_cat.width, player_cat.height);
	IMAGE cat1_logic(player_cat.width, player_cat.height);
	loadimage(&cat1_logic, L".\\pictures\\cat_logic.png", player_cat.width, player_cat.height);	//猫初始化完成

	IMAGE map(GRAPH_WIDTH, GRAPH_HEIGHT);
	loadimage(&map, L".\\pictures\\map_grassland.jpg", GRAPH_WIDTH, GRAPH_HEIGHT);

	/***************初始化完成***************/

	/****************进入游戏界面绘图*****************/
	cleardevice();
	BeginBatchDraw();
	while (1) {
		choice++;
		MousePtr parallel = mouseKing;

		putimage(0, 0, &map);
		putimage(player_cat.x_coordinate, player_cat.y_coordinate, &cat1_logic, NOTSRCERASE);
		putimage(player_cat.x_coordinate, player_cat.y_coordinate, &cat1, SRCINVERT);
		
		while (parallel != NULL) {
			switch ((choice / 10) % 3 + 1)
			//switch(1)
			{
			case 1:
				outputMouseImage(*parallel, (parallel->frame1), (parallel->frame1_logic));
				break;
			case 2:
				outputMouseImage(*parallel, (parallel->frame2), (parallel->frame2_logic));
				break;
			case 3:
				outputMouseImage(*parallel, (parallel->frame3), (parallel->frame3_logic));
				break;
			default:
				break;
			}

			mouseRandomRun(parallel);
			mouseLineEscape(parallel, player_cat);
			catCatch(&mouseKing, &parallel, player_cat);

			if (parallel == NULL) break;
			else parallel = parallel->nextPtr;
			
		}
		FlushBatchDraw();
		catControl(&player_cat);
		Sleep(30);
		if (choice > 1000) choice = 0;
	}

	cleardevice();

	return 0;
}
int sets_running() {
	while (1) {
		cleardevice();
		circle(200, 200, 50);
		FlushBatchDraw();
	}

	return 0;
}
int rank_running() {
	while (1) {
		cleardevice();
		circle(200, 200, 50);
		FlushBatchDraw();

	}

	return 0;
}
int music_running() {
	mciSendString(L"stop .\\musics\\test.mp3", NULL, 0, 0);

	return 0;
}
int mute_running() {
	mciSendString(L"resume .\\musics\\test.mp3", NULL, 0, 0);

	return 1;
}
int quit_running() {
	return 0;
}

void mouseInitialLink(int number, MousePtr* micePtr) {
	MousePtr parallelPtr = *micePtr;
	srand(time(NULL));
	int type = 0;

	for (int i = 0; i < number; i++) {
		MousePtr newPtr = (MousePtr)malloc(sizeof(Mouse));

		if (newPtr != NULL) {
			//type = rand() % 3;
			if (type == 0) {
				newPtr->width = JERRY_GRAPH_SIZE;
				newPtr->height = JERRY_GRAPH_SIZE;
				newPtr->x_coordinate = rand() % GRAPH_WIDTH / 3;
				newPtr->y_coordinate = rand() % GRAPH_WIDTH / 3;
				newPtr->speed = RANDOM_MOUSE_SPEED;

				newPtr->frame1 = new IMAGE(newPtr->width, newPtr->height);
				loadimage((newPtr->frame1), L".\\pictures\\mouse_1.jpg", newPtr->width, newPtr->height);
				newPtr->frame1_logic = new IMAGE(newPtr->width, newPtr->height);
				loadimage((newPtr->frame1_logic), L".\\pictures\\mouse_1_logic.png", newPtr->width, newPtr->height);

				newPtr->frame2 = new IMAGE(newPtr->width, newPtr->height);;
				loadimage((newPtr->frame2), L".\\pictures\\mouse_2.jpg", newPtr->width, newPtr->height);
				newPtr->frame2_logic = new IMAGE(newPtr->width, newPtr->height);
				loadimage((newPtr->frame2_logic), L".\\pictures\\mouse_2_logic.png", newPtr->width, newPtr->height);

				newPtr->frame3 = new IMAGE(newPtr->width, newPtr->height);
				loadimage((newPtr->frame3), L".\\pictures\\mouse_3.jpg", newPtr->width, newPtr->height);
				newPtr->frame3_logic = new IMAGE(newPtr->width, newPtr->height);
				loadimage((newPtr->frame3_logic), L".\\pictures\\mouse_3_logic.png", newPtr->width, newPtr->height);

			}
			/*if (type == 1) {
			}
			if (type == 2) {
			}*/

			newPtr->nextPtr = NULL;

			if (isEmpty_forMouseLink(parallelPtr)) {
				(*micePtr) = newPtr;
				parallelPtr = *micePtr;
			}
			else {
				parallelPtr->nextPtr = newPtr;
				parallelPtr = parallelPtr->nextPtr;

			}
		}
	}
}

int isEmpty_forMouseLink(MousePtr goodday) {
	return goodday == NULL;
}

void outputMouseImage(const Mouse mouse, IMAGE* pic, IMAGE* pic_logic) {
	putimage(mouse.x_coordinate, mouse.y_coordinate, pic_logic, NOTSRCERASE);
	putimage(mouse.x_coordinate, mouse.y_coordinate, pic, SRCINVERT);
}

void mouseRandomRun(MousePtr it) {
	srand( time(NULL) + (int)it);
	it->speed = RANDOM_MOUSE_SPEED;

	it->x_coordinate += (rand() % 3 - 1) * it->speed;
	it->y_coordinate += (rand() % 3 - 1) * it->speed;

	if (it->x_coordinate < 0) {
		it->x_coordinate += (rand() % 2 + 1) * it->speed;
	}
	if (it->x_coordinate > GRAPH_WIDTH - it->width) {
		it->x_coordinate -= (rand() % 2 + 1) * it->speed;
	}
	if (it->y_coordinate < 0) {
		it->y_coordinate += (rand() % 2 + 1) * it->speed;
	}
	if (it->y_coordinate > GRAPH_HEIGHT - it->height) {
		it->y_coordinate -= (rand() % 2 + 1) * it->speed;
	}
}

void mouseLineEscape(MousePtr mouse, const Cat cat) {
	int x_distance = mouse->x_coordinate + mouse->width / 2 - cat.x_coordinate - cat.width / 2;
	int y_distance = mouse->y_coordinate + mouse->height / 2 - cat.y_coordinate - cat.height / 2;
	double distance = pow((pow(x_distance, 2) + pow(y_distance, 2)), 0.5);


	if (distance <= 180) {
		mouse->speed = EASY_MOUSE_RUN_SPEED;
		if (x_distance == 0) {
			if(y_distance > 0) mouse->y_coordinate += mouse->speed;
			else mouse->y_coordinate -= mouse->speed;
		}

		else {
			double absolute_slope = abs(y_distance / x_distance);
			if (x_distance > 0) {
				if (absolute_slope >= 2.414) {
					if (y_distance > 0) mouse->y_coordinate += mouse->speed;
					else mouse->y_coordinate -= mouse->speed;
				}
				if (absolute_slope < 2.414 && absolute_slope > 0.414) {
					mouse->x_coordinate += mouse->speed;
					if (y_distance > 0) mouse->y_coordinate += mouse->speed;
					else mouse->y_coordinate -= mouse->speed;
				}
				if (absolute_slope <= 0.414) mouse->x_coordinate += mouse->speed;
			}

			if (x_distance < 0) {
				if (absolute_slope >= 2.414) {
					if (y_distance > 0) mouse->y_coordinate += mouse->speed;
					else mouse->y_coordinate -= mouse->speed;
				}
				if (absolute_slope < 2.414 && absolute_slope > 0.414) {
					mouse->x_coordinate -= mouse->speed;
					if (y_distance > 0) mouse->y_coordinate += mouse->speed;
					else mouse->y_coordinate -= mouse->speed;
				}
				if (absolute_slope <= 0.414) mouse->x_coordinate -= mouse->speed;
			}
		}

		if (mouse->x_coordinate < 0) {
			mouse->x_coordinate += mouse->speed;
		}
		if (mouse->x_coordinate > GRAPH_WIDTH - mouse->width) {
			mouse->x_coordinate -= mouse->speed;
		}
		if (mouse->y_coordinate < 0) {
			mouse->y_coordinate += mouse->speed;
		}
		if (mouse->y_coordinate > GRAPH_HEIGHT - mouse->height) {
			mouse->y_coordinate -= mouse->speed;
		}

	}
}

void catControl(CatPtr it) {

	if (GetAsyncKeyState(0x41)) {	//A
		it->x_coordinate -= it->speed;
	}
	if (GetAsyncKeyState(0x44)) {	//D
		it->x_coordinate += it->speed;
	}
	if (GetAsyncKeyState(0x53)) {	//S
		it->y_coordinate += it->speed;
	}
	if (GetAsyncKeyState(0x57)) {	//W
		it->y_coordinate -= it->speed;
	}

	if (it->x_coordinate < 0) {
		it->x_coordinate += it->speed;
	}
	if (it->x_coordinate > GRAPH_WIDTH - it->width) {
		it->x_coordinate -= it->speed;
	}
	if (it->y_coordinate < 0) {
		it->y_coordinate += it->speed;
	}
	if (it->y_coordinate > GRAPH_HEIGHT - it->height) {
		it->y_coordinate -= it->speed;
	}
}

void catCatch(MousePtr* headMice, MousePtr* unluckOne, const Cat cat) {
	int x_distance = (*unluckOne)->x_coordinate + (*unluckOne)->width / 2 - cat.x_coordinate - cat.width / 2;
	int y_distance = (*unluckOne)->y_coordinate + (*unluckOne)->height / 2 - cat.y_coordinate - cat.height / 2;
	double distance = pow((pow(x_distance, 2) + pow(y_distance, 2)), 0.5);

	if (distance <= 50) {
		mouseNodeFree(&headMice, &unluckOne);
	}
}

void mouseNodeFree(MouseDoublePtr* mouseKing, MouseDoublePtr* unluckOne) {
	MousePtr parallelPtr = **mouseKing;
	MousePtr tempPtr = **unluckOne;

	if (**mouseKing == **unluckOne) {
		**mouseKing = (**mouseKing)->nextPtr;
		**unluckOne = (**unluckOne)->nextPtr;
		free(parallelPtr);
		return;
	}

	while (parallelPtr->nextPtr != **unluckOne) {
		parallelPtr = parallelPtr->nextPtr;
	}

	parallelPtr->nextPtr = (**unluckOne)->nextPtr;
	//free(tempPtr);
	**unluckOne = parallelPtr;
	free(tempPtr);
}