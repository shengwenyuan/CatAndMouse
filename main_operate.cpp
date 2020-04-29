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
#define TITLE_WIDTH (GRAPH_WIDTH / 2)
#define TITLE_HEIGHT (GRAPH_HEIGHT / 5)
#define BUTTON_GAP 150
#define SPEAKER_SIZE 60
#define JERRY_GRAPH_SIZE 60
#define TOM_GRAPH_SIZE 100
#define PLAYER_SPEED 10
#define RANDOM_MOUSE_SPEED 3
#define EASY_MOUSE_RUN_SPEED 5 
#define MOUSE_TOTAL_NUMBER 3
#define MOUSE_ADD_CYCLE 6
#define GAME_TOTAL_TIME 2
#define FONT_SCORE_SIZE 40
#define FONT_GMOVER_SIZE 80
#define FONT_NAME_SIZE 30
#define FONT_RANK_SIZE 40

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

void mouseInitialLink(int number, MousePtr * mouseKing);
void inputInformation(MousePtr * it);
int isEmpty_forMouseLink(MousePtr goodday);
void mouseNodeFree(MouseDoublePtr* mouseKing, MouseDoublePtr* unluckOne);
void outputMouseImage(const Mouse mouse, IMAGE* pic, IMAGE* pic_logic);
void outputScore(int* score);
void mouseRandomRun(MousePtr it);
void mouseLineEscape(MousePtr mouse, const Cat cat);
void catControl(CatPtr it);
void catCatch(MousePtr* headMice, MousePtr* unluckOne, const Cat cat, int* score);
void isAddMouseTime(MousePtr* headMice);
void isEnd(int* signal);
void inputPlayerName(int score);

void descendDwap(char* name[100], int* score[100], int length);

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

	mciSendString("open .\\musics\\test.mp3", NULL, 0, 0);
	mciSendString("play .\\musics\\test.mp3 repeat", NULL, 0, 0);

	BeginBatchDraw();

	IMAGE map(GRAPH_WIDTH, GRAPH_HEIGHT);
	loadimage(&map, ".\\pictures\\map_grassland.jpg", GRAPH_WIDTH, GRAPH_HEIGHT);

	IMAGE title(TITLE_WIDTH, TITLE_HEIGHT);
	loadimage(&title, ".\\pictures\\title.png", TITLE_WIDTH, TITLE_HEIGHT);
	IMAGE title_logic(TITLE_WIDTH, TITLE_HEIGHT);
	loadimage(&title_logic, ".\\pictures\\title_logic.png", TITLE_WIDTH, TITLE_HEIGHT);

	Button b1(game_running, ".\\pictures\\start_button.png", (GRAPH_WIDTH-CLASSIC_BUTTON_WID)/2, (GRAPH_HEIGHT - CLASSIC_BUTTON_HEI) / 2);
	Button b2(sets_running, ".\\pictures\\setting_button.png", (GRAPH_WIDTH - CLASSIC_BUTTON_WID) / 2, (GRAPH_HEIGHT - CLASSIC_BUTTON_HEI) / 2 + BUTTON_GAP);
	Button b3(rank_running, ".\\pictures\\rank_button.png", (GRAPH_WIDTH - CLASSIC_BUTTON_WID) / 2, (GRAPH_HEIGHT - CLASSIC_BUTTON_HEI) / 2 + BUTTON_GAP*2);
	Button sound_button(music_running, ".\\pictures\\sound_button.jpg", 30, 30, &playMusicOrNot, SPEAKER_SIZE, SPEAKER_SIZE);
	Button mute_button(mute_running, ".\\pictures\\mute_button.png", 30, 30, &playMusicOrNot, SPEAKER_SIZE, SPEAKER_SIZE);
	Button b5(quit_running, ".\\pictures\\start_button.png", (GRAPH_WIDTH - CLASSIC_BUTTON_WID) / 2, (GRAPH_HEIGHT - CLASSIC_BUTTON_HEI) / 2 + BUTTON_GAP*3, &continueOrNot);

	while (1) {
		putimage(0, 0, &map);
		putimage((GRAPH_WIDTH - TITLE_WIDTH) / 2, GRAPH_HEIGHT / 6, &title_logic, NOTSRCERASE);
		putimage((GRAPH_WIDTH - TITLE_WIDTH) / 2, GRAPH_HEIGHT / 6, &title, SRCINVERT);

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
	int score = 0;	//玩家得分
	int choice = 0;	//控制打印不同帧
	time_t startTime = time(NULL);
	time_t nodeTime = startTime;
	int continueSignal = 1;	//游戏是否结束的信号
	MousePtr parallel;
	
	MousePtr mouseKing = NULL;
	mouseInitialLink(MOUSE_TOTAL_NUMBER, &mouseKing);	//老鼠初始化完成

	Cat player_cat;
	player_cat.width = TOM_GRAPH_SIZE;
	player_cat.height = TOM_GRAPH_SIZE;
	player_cat.x_coordinate = (GRAPH_WIDTH - TOM_GRAPH_SIZE) / 2;
	player_cat.y_coordinate = (GRAPH_HEIGHT - TOM_GRAPH_SIZE) / 2;
	player_cat.speed = PLAYER_SPEED;
	IMAGE cat1(player_cat.width, player_cat.height);
	loadimage(&cat1, ".\\pictures\\cat.jpg", player_cat.width, player_cat.height);
	IMAGE cat1_logic(player_cat.width, player_cat.height);
	loadimage(&cat1_logic, ".\\pictures\\cat_logic.png", player_cat.width, player_cat.height);	//猫初始化完成

	IMAGE map(GRAPH_WIDTH, GRAPH_HEIGHT);
	loadimage(&map, ".\\pictures\\map_grassland.jpg", GRAPH_WIDTH, GRAPH_HEIGHT);	//地图加载完成

	
	/***************初始化完成***************/

	/****************进入游戏界面绘图*****************/
	cleardevice();
	BeginBatchDraw();
	while (continueSignal) {
		choice++;
		if (choice > 1000) choice = 0;
		parallel = mouseKing;

		putimage(0, 0, &map);
		putimage(player_cat.x_coordinate, player_cat.y_coordinate, &cat1_logic, NOTSRCERASE);
		putimage(player_cat.x_coordinate, player_cat.y_coordinate, &cat1, SRCINVERT);
		setbkcolor(DARKGRAY);
		settextcolor(YELLOW);
		settextstyle(FONT_SCORE_SIZE, FONT_SCORE_SIZE, "consolas");
		outtextxy(GRAPH_WIDTH - 400, 200, "score:");
		
		while (parallel != NULL) {
			switch ((choice / 10) % 3 + 1)
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
			catCatch(&mouseKing, &parallel, player_cat, &score);

			if (parallel == NULL) break;  //老鼠被吃光了
			else parallel = parallel->nextPtr;
			
		}

		outputScore(&score);
		if ((time(NULL) - nodeTime) > MOUSE_ADD_CYCLE) {
			isAddMouseTime(&mouseKing);
			nodeTime = time(NULL);
		}
		if ((time(NULL) - startTime) > GAME_TOTAL_TIME) isEnd(&continueSignal);
		FlushBatchDraw();
		catControl(&player_cat);

		Sleep(30);
	}


	inputPlayerName(score);
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
	FILE* sfPtr; //score file pointer
	int score;
	char name[20];
	int* allScore[100];
	char* allName[100];
	char str[20];
	int count = 0;
	IMAGE map(GRAPH_WIDTH, GRAPH_HEIGHT);


	if (fopen_s(&sfPtr, "score.txt", "r") != NULL) {
		
		do {
			fscanf_s(sfPtr, "%s %d", name, &score);
			allName[count] = name;
			allScore[count] = &score;
			count++;
		}while (!feof(sfPtr));

		descendDwap(allName, allScore, count-1);	//降幂整理文件数据

		for (int i = 0; i < 10; i++) {
			sprintf_s(str, "%d", *(allScore[i]));
			setbkcolor(LIGHTMAGENTA);
			settextcolor(LIGHTCYAN);
			settextstyle(FONT_RANK_SIZE, FONT_RANK_SIZE, "consolas");
			outtextxy((GRAPH_WIDTH / 3), (GRAPH_HEIGHT / 13) * (i + 1), *(allName[i]));
			settextstyle(FONT_RANK_SIZE, FONT_RANK_SIZE, "consolas");
			outtextxy(GRAPH_WIDTH / 3 + FONT_RANK_SIZE * 11, (GRAPH_HEIGHT / 13) * (i + 1), str);
			FlushBatchDraw();
		}

		

		fclose(sfPtr);
	}
	return 0;
}
int music_running() {
	mciSendString("stop .\\musics\\test.mp3", NULL, 0, 0);

	return 0;
}
int mute_running() {
	mciSendString("resume .\\musics\\test.mp3", NULL, 0, 0);

	return 1;
}
int quit_running() {
	return 0;
}

void mouseInitialLink(int number, MousePtr* mouseKing) {
	MousePtr parallelPtr = *mouseKing;

	for (int i = 0; i < number; i++) {
		MousePtr newPtr = (MousePtr)malloc(sizeof(Mouse));

		if (newPtr != NULL) {
			inputInformation(&newPtr);

			newPtr->nextPtr = NULL;

			if (isEmpty_forMouseLink(parallelPtr)) {
				(*mouseKing) = newPtr;
				parallelPtr = *mouseKing;
			}
			else {
				parallelPtr->nextPtr = newPtr;
				parallelPtr = parallelPtr->nextPtr;

			}
		}
	}
}

void inputInformation(MousePtr* it) {
	srand(time(NULL));
	int type = rand() % 3 + 1;
	switch (type)
	{
	case 1:
		(*it)->width = JERRY_GRAPH_SIZE;
		(*it)->height = JERRY_GRAPH_SIZE;
		(*it)->x_coordinate = rand() % GRAPH_WIDTH / 3;
		(*it)->y_coordinate = rand() % GRAPH_WIDTH / 3;
		(*it)->speed = RANDOM_MOUSE_SPEED;

		(*it)->frame1 = new IMAGE((*it)->width, (*it)->height);
		loadimage(((*it)->frame1), ".\\pictures\\mouse_1.jpg", (*it)->width, (*it)->height);
		(*it)->frame1_logic = new IMAGE((*it)->width, (*it)->height);
		loadimage(((*it)->frame1_logic), ".\\pictures\\mouse_1_logic.png", (*it)->width, (*it)->height);

		(*it)->frame2 = new IMAGE((*it)->width, (*it)->height);;
		loadimage(((*it)->frame2), ".\\pictures\\mouse_2.jpg", (*it)->width, (*it)->height);
		(*it)->frame2_logic = new IMAGE((*it)->width, (*it)->height);
		loadimage(((*it)->frame2_logic), ".\\pictures\\mouse_2_logic.png", (*it)->width, (*it)->height);

		(*it)->frame3 = new IMAGE((*it)->width, (*it)->height);
		loadimage(((*it)->frame3), ".\\pictures\\mouse_3.jpg", (*it)->width, (*it)->height);
		(*it)->frame3_logic = new IMAGE((*it)->width, (*it)->height);
		loadimage(((*it)->frame3_logic), ".\\pictures\\mouse_3_logic.png", (*it)->width, (*it)->height);

		break;
	case 2:
		(*it)->width = JERRY_GRAPH_SIZE;
		(*it)->height = JERRY_GRAPH_SIZE;
		(*it)->x_coordinate = rand() % GRAPH_WIDTH / 3 + GRAPH_WIDTH / 2;
		(*it)->y_coordinate = rand() % GRAPH_WIDTH / 3;
		(*it)->speed = RANDOM_MOUSE_SPEED;

		(*it)->frame1 = new IMAGE((*it)->width, (*it)->height);
		loadimage(((*it)->frame1), ".\\pictures\\mouse_1.jpg", (*it)->width, (*it)->height);
		(*it)->frame1_logic = new IMAGE((*it)->width, (*it)->height);
		loadimage(((*it)->frame1_logic), ".\\pictures\\mouse_1_logic.png", (*it)->width, (*it)->height);

		(*it)->frame2 = new IMAGE((*it)->width, (*it)->height);;
		loadimage(((*it)->frame2), ".\\pictures\\mouse_2.jpg", (*it)->width, (*it)->height);
		(*it)->frame2_logic = new IMAGE((*it)->width, (*it)->height);
		loadimage(((*it)->frame2_logic), ".\\pictures\\mouse_2_logic.png", (*it)->width, (*it)->height);

		(*it)->frame3 = new IMAGE((*it)->width, (*it)->height);
		loadimage(((*it)->frame3), ".\\pictures\\mouse_3.jpg", (*it)->width, (*it)->height);
		(*it)->frame3_logic = new IMAGE((*it)->width, (*it)->height);
		loadimage(((*it)->frame3_logic), ".\\pictures\\mouse_3_logic.png", (*it)->width, (*it)->height);

		break;
	case 3:
		(*it)->width = JERRY_GRAPH_SIZE;
		(*it)->height = JERRY_GRAPH_SIZE;
		(*it)->x_coordinate = rand() % GRAPH_WIDTH / 3;
		(*it)->y_coordinate = rand() % GRAPH_WIDTH / 3 + GRAPH_WIDTH / 2;
		(*it)->speed = RANDOM_MOUSE_SPEED;

		(*it)->frame1 = new IMAGE((*it)->width, (*it)->height);
		loadimage(((*it)->frame1), ".\\pictures\\mouse_1.jpg", (*it)->width, (*it)->height);
		(*it)->frame1_logic = new IMAGE((*it)->width, (*it)->height);
		loadimage(((*it)->frame1_logic), ".\\pictures\\mouse_1_logic.png", (*it)->width, (*it)->height);

		(*it)->frame2 = new IMAGE((*it)->width, (*it)->height);;
		loadimage(((*it)->frame2), ".\\pictures\\mouse_2.jpg", (*it)->width, (*it)->height);
		(*it)->frame2_logic = new IMAGE((*it)->width, (*it)->height);
		loadimage(((*it)->frame2_logic), ".\\pictures\\mouse_2_logic.png", (*it)->width, (*it)->height);

		(*it)->frame3 = new IMAGE((*it)->width, (*it)->height);
		loadimage(((*it)->frame3), ".\\pictures\\mouse_3.jpg", (*it)->width, (*it)->height);
		(*it)->frame3_logic = new IMAGE((*it)->width, (*it)->height);
		loadimage(((*it)->frame3_logic), ".\\pictures\\mouse_3_logic.png", (*it)->width, (*it)->height);

		break;
	default:
		break;
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

void catCatch(MousePtr* headMice, MousePtr* unluckOne, const Cat cat, int* score) {
	int x_distance = (*unluckOne)->x_coordinate + (*unluckOne)->width / 2 - cat.x_coordinate - cat.width / 2;
	int y_distance = (*unluckOne)->y_coordinate + (*unluckOne)->height / 2 - cat.y_coordinate - cat.height / 2;
	double distance = pow((pow(x_distance, 2) + pow(y_distance, 2)), 0.5);

	if (distance <= 50) {
		mouseNodeFree(&headMice, &unluckOne);
		*score += 10;
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

void outputScore(int* score) {
	char str[5];
	sprintf_s(str, "%d", (*score));

	outtextxy(GRAPH_WIDTH - 400 + FONT_SCORE_SIZE * 6, 200, str);
}

void isAddMouseTime(MousePtr* headMice) {

	MousePtr newPtr = (MousePtr)malloc(sizeof(Mouse));
	inputInformation(&newPtr);
	newPtr->nextPtr = *headMice;

	*headMice = newPtr;

}

void isEnd(int* signal) {

	*signal = 0;
}

void inputPlayerName(int score) {
	char name[12] = {'_', '_' , '_' , '_' , '_',
					'_' , '_' , '_' , '_' , '_','|', '\0' };
	int i = 0;
	
	setbkcolor(YELLOW);
	settextcolor(RED);
	settextstyle(FONT_GMOVER_SIZE, FONT_GMOVER_SIZE, "consolas");
	outtextxy((GRAPH_WIDTH - FONT_GMOVER_SIZE * 10) / 2, (GRAPH_HEIGHT - FONT_GMOVER_SIZE) / 2, "Game over!");
	settextcolor(BLACK);
	settextstyle(FONT_NAME_SIZE, FONT_NAME_SIZE, "consolas");
	outtextxy(GRAPH_WIDTH / 4 + 50, (GRAPH_HEIGHT - FONT_GMOVER_SIZE) / 2 + 100, "Please leave your name: ");
	outtextxy(GRAPH_WIDTH / 4 + 50, (GRAPH_HEIGHT - FONT_GMOVER_SIZE) / 2 + 150, name);
	FlushBatchDraw();

	//fflush(stdin);
	while (_kbhit()) {
		_getch();
	}
	Sleep(1000);	//防止用户仍按方向键
	while (1) {
		if (_kbhit()) {
			name[i] = _getch();	//8 = delete	13 = enter	27 = esc

			if ( name[i] == 27 || (0 == i && name[i] == 13) ) { return; }//当按下ESC时循环，ESC键的键值时27.
			if ( name[i] == 13 ) { break; }

			if (name[i] == 8 && i > 0) {
				i -= 2;
				name[i + 1] = '_';
				if (10 == i + 2) name[i + 2] = '|';
				else name[i + 2] = '_';
			}
			if ((i == 10 && name[i] != 13 && name[i] != 8)) { name[i] = '|'; }
			if (name[i] == 8 && i == 0) { name[i] = '_'; }
			else if (i < 10) { i++; }

			outtextxy(GRAPH_WIDTH / 4 + 50, (GRAPH_HEIGHT - FONT_GMOVER_SIZE) / 2 + 150, name);
			FlushBatchDraw();
		}
	}
	name[i] = '\0';
	//if (name[0] == '\0') return;

	FILE* sfPtr;	//玩家得分记录文件指针: score.dat file pointer
	if ( !fopen_s(&sfPtr, "score.txt", "a+") ) {
			fprintf(sfPtr, "%s %d\n", name, score);

			fclose(sfPtr);
	}

	//fopen_s(&sfPtr, "score.txt", "a+");
	//fprintf(sfPtr, "%s %d\n", name, score);
}

void descendDwap(char* name[100], int* score[100], int length) {
	int tempS;
	char tempN;

	for (int i = 0; i < length - 1; i++) {
		for(int j = i; i < length - 1; i++)
		if ((*score[j]) < (*score[j + 1])) {
			tempS = (*score[j]);
			(*score[j]) = (*score[j + 1]);
			(*score[j + 1]) = tempS;

			tempN = (*name[j]);
			(*name[j]) = (*name[j + 1]);
			(*name[j + 1]) = tempN;
		}
	}
}