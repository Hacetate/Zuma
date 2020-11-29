#include<iostream>
#include<stack>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <graphics.h>
#include <conio.h>
#include "kdtree.h"
#include "ball.h"
#include "MySort.h"
#include <crtdbg.h>//add this header file to detect memory leaks

using namespace std;

#define WINDOWWITH 600
#define WINDOWHEIGHT 600
#define DEF_NUM_PTS 50
double speed = 4.0;
double K = 0.9958;
int swing = 0;
int point = 0;
int T = 9999999;
#define π 3.141592653589793

typedef struct user
{
	char* name;
	int score;

}user;


COLORREF ballColorTable[] = { BLUE, GREEN, RED, YELLOW, MAGENTA, BROWN };


//绘制碰撞球
void drawColBall(DataType* b, float x, float y)
{
	b->x = x;
	b->y = y;
	setcolor(ballColorTable[b->c]);
	setfillcolor(ballColorTable[b->c]);
	solidcircle(b->x, b->y, BALLRADIUS);
}

//中序遍历kdtree
static void InOrderTraverse(kdnode* T)
{
	if (T)
	{
		InOrderTraverse(T->left);
		setcolor(ballColorTable[T->data.c]);
		setfillcolor(ballColorTable[T->data.c]);
		solidcircle(T->data.x, T->data.y, BALLRADIUS);
		InOrderTraverse(T->right);
	}
}

static float rd(void) {
	return (float)(BALLRADIUS + rand() % (WINDOWHEIGHT - 2 * BALLRADIUS));
}

float ballmoveX(float x, DataType b) {
	x += b.v * cos(b.dir);
	return x;
}

float ballmoveY(float y, DataType b) {
	y -= b.v * sin(b.dir);
	return y;
}

void ballmoving(DataType& cball) {
	cball.v *= K;
	drawColBall(&cball, cball.x = ballmoveX(cball.x, cball), cball.y = ballmoveY(cball.y, cball));
}

void ballcollision(DataType& dynamicball, DataType& staticball)
{
	staticball.dir = atan2(dynamicball.y - staticball.y, staticball.x - dynamicball.x);
	if (staticball.dir < dynamicball.dir) {
		staticball.v = fabs((dynamicball.v) * cos(staticball.dir - dynamicball.dir));
		dynamicball.v *= fabs(sin(staticball.dir - dynamicball.dir));
		dynamicball.dir = staticball.dir + π / 2;
	}
	else {
		staticball.v = fabs((dynamicball.v) * cos(staticball.dir - dynamicball.dir));
		dynamicball.v *= fabs(sin(staticball.dir - dynamicball.dir));
		dynamicball.dir = staticball.dir - π / 2;
	}


}

DataType From_root_To_movingroot(DataType rootball, Node* movingroot)
{
	DataType* ball;
	ball = (DataType*)malloc(sizeof(DataType));
	ball->c = rootball.c;
	ball->dir = rootball.dir;
	ball->v = rootball.v;
	ball->x = rootball.x;
	ball->y = rootball.y;
	return *ball;
}

void moving(Node* movingroot, kdnode* root) {
	int point0=0;
	DataType NearestBall, newball;
	float dis;
	Node* p = movingroot->next, * q = p;
	for (int i = 0; p != NULL; i++)
	{
		ballmoving(p->data);
		searchNearest(root, p->data, NearestBall, dis);
		if (p->data.v < 0.3)
		{
			root = kdnode_insert(root, p->data);
			p = p->next;
			ListDelete(movingroot, i);
			i--;
			continue;
		}
		if (dis <= 2 * BALLRADIUS)
		{
			if (p->data.c == NearestBall.c)
			{
				root = kdnode_delete(root, NearestBall);
				q = p;
				p = p->next;
				ListDelete(movingroot, i);
				i--;
				point0++;
				continue;
			}
			else
			{
				newball = From_root_To_movingroot(NearestBall, movingroot);
				ListInsert(movingroot, 0, newball);
				i++;
				root = kdnode_delete(root, NearestBall);
				ballcollision(p->data, movingroot->next->data);
				p = p->next;
				continue;
			}
		}
		//检测碰撞球是否超出范围
		if (p->data.x > WINDOWWITH|| p->data.x <0 )	p->data.dir = π - p->data.dir;
		if (p->data.y > WINDOWHEIGHT  || p->data.y < 0)	p->data.dir *= -1;

		p = p->next;
	}
	point += pow(2, point0)-1;
}
DataType Createcball()
{
	DataType* cball;
	cball = (DataType*)malloc(sizeof(DataType));
	(*cball).c = rand() % 6;
	(*cball).v = speed;
	return *cball;
}
void moveroot(kdnode* root, double t)
{
	if (root == NULL) return;
	moveroot(root->left, t);
	root->data.x += swing * cos(t) ;
	root->data.y += swing * sin(t) ;
	moveroot(root->right, t);
}
time_t _times(time_t t1, time_t t2, bool& newball, int times, kdnode* root,float θ)
{
	if (difftime(t2, t1) > double(100))
		moveroot(root, θ);
	if (difftime(t2, t1) > double(2000 / (times + 1))) {
		newball = true;
		T--;
		return t2;
	}
	return t1;
}

int main()
{
	int mode,ballspeed, fri, times=1, happy = 0,checkpoint=2,part=1;
	TCHAR name[100];
	TCHAR s1[] = _T("请输入用户名");
	InputBox(name, 100,s1);


	printf("///////////////////////////////////////////////////////////////////////////////////\n\
///////////////////////////////////////////////////////////////////////////////////\n\
/////*************//////**/////////**//////**///////**////////////****/////////////\n\
//////////////**////////**/////////**//////***/////***///////////**///**///////////\n\
/////////////**/////////**/////////**//////**/**///**/*/////////**/////**//////////\n\
////////////**//////////**/////////**/////**///**/**///**///////**/////**//////////\n\
///////////**///////////**/////////**/////**////***////**///////**/////**//////////\n\
//////////**////////////**/////////**/////**////***////**//////***********/////////\n\
/////////**/////////////**/////////**////**/////***/////**/////**///////**/////////\n\
////////**//////////////**/////////**////**/////////////**/////**///////**/////////\n\
//////***///////////////**/////////**////**/////////////**/////**///////**/////////\n\
////*************////////***********/////**/////////////**/////**///////**/////////\n\
///////////////////////////////////////////////////////////////////////////////////\n\
\n这波啊，这波是开心消消乐，在一次发射中若能同时消除多个球则分数更多\n");
	printf("\n输入0进入自定义模式，输入其他进入挑战模式\n");
	scanf_s("%d", &mode);
	if (mode == 0) {
		printf("\n您可以调整参数(括号内的参数范围只是建议///芜湖，起飞)\n(推荐玩法①10 ②-100 ③100 连点鼠标，起飞，但是很快就会崩溃hhh)\n\n");
			printf("①球的速度\n请输入(1~100)：");
			scanf_s("%d", &ballspeed);
			speed += (0.05) * ballspeed;
			printf("②摩擦系数\n请输入(-100~100,数据越大摩擦力越大,非线性变换)：");
			scanf_s("%d", &fri);
			K -= fri * (0.00004);
			printf("③发射球的刷新速度\n请输入(1~100数据越大,球刷新的越快)：");
			scanf_s("%d", &times);
			printf("④如果你想让球摇摆起来\n请输入(1~3否则请输入0)：");
			scanf_s("%d", &swing);
			printf("\n\n若想玩隐藏模式请输入1(娱乐用，正常模式输入0)：");
			scanf_s("%d", &happy);
			if (happy == 1)swing = 0;
	}
	else {
		T = 100;
	}
	//初始化绘图窗口
	initgraph(WINDOWWITH, WINDOWHEIGHT);
	setbkcolor(BLACK);
	cleardevice();
	//球的个数
	int num_pts = DEF_NUM_PTS;
	//指向kdtree
	struct kdnode* root = NULL;
	//创建动态球数组
	Node* movingroot;
	movingroot = CreateEmptyList();
	//最近球和最小距离
	DataType NearestBall;
	float dis;
	//插入球
	DataType Ball;
	srand(time(0));

	//把球画出来插kdtree上(避免随机出现的圆重叠)
	for (int i = 0; i < num_pts; ++i) {
		Ball.x = rd(), Ball.y = rd(), Ball.c = rand() % 6;
		while (i > 0)
		{
			searchNearest(root, Ball, NearestBall, dis);
			if (dis >= 2 * BALLRADIUS)break;
			Ball.x = rd(), Ball.y = rd();
		}
		root = kdnode_insert(root, Ball);
		setcolor(ballColorTable[Ball.c]);
		setfillcolor(ballColorTable[Ball.c]);
		solidcircle(Ball.x, Ball.y, BALLRADIUS);
	}

	//画碰撞球
	DataType cball;
	cball = Createcball();
	drawColBall(&cball, WINDOWWITH / 2, WINDOWHEIGHT);
	float r = 1;

	//事件循环，处理鼠标、时间事件
	MOUSEMSG m;
	bool ballMoving = true;
	bool initial = true;
	time_t t0 = clock(), t;
	float θ = π / 2;

	while (root)
	{
		if (point >= checkpoint) {
			part++;
			T += part*4;
			checkpoint *= 2;
			if (part > 2)swing = 1;
			if (part > 4)swing = 2;
			if (part > 6)swing = 3;
			for (int i = 0; i < 10; ++i) {
				Ball.x = rd(), Ball.y = rd(), Ball.c = rand() % 6;
				while (i > 0)
				{
					searchNearest(root, Ball, NearestBall, dis);
					if (dis >= 2 * BALLRADIUS)break;
					Ball.x = rd(), Ball.y = rd();
				}
				root = kdnode_insert(root, Ball);
				setcolor(ballColorTable[Ball.c]);
				setfillcolor(ballColorTable[Ball.c]);
				solidcircle(Ball.x, Ball.y, BALLRADIUS);
			}
	       }
		if (T < 0) {





		}
		θ += π / 20;
		t = clock();
		t0 = _times(t0, t, ballMoving, times, root, θ);
		BeginBatchDraw();
		// 获取一条鼠标消息
		if (MouseHit())
		{
			m = GetMouseMsg();
			switch (m.uMsg)
			{
			case WM_MOUSEMOVE:
				break;

			case WM_LBUTTONDOWN:
				//更新球的运动方向，球开始运动
				if (ballMoving)
				{
					r = sqrt(((m.x - WINDOWWITH / 2)) * (m.x - WINDOWWITH / 2) + (m.y - WINDOWHEIGHT) * (m.y - WINDOWHEIGHT));
					cball.dir = atan2(WINDOWHEIGHT - m.y, m.x - WINDOWWITH / 2);
					ListInsert(movingroot, 0, cball);
					initial = false;
				}
				break;

			case WM_RBUTTONUP:
				closegraph();
				kdtree_destroy(root);
				if (true)
				{
					FILE* fp;
					errno_t err;
				    err = fopen_s(&fp,"GameRanking.txt","w");
					if (fp == NULL)
						printf("fail to open the file! \n");
					else
					{
						
						fclose(fp);
					}
					cout << "已经结束游戏" << endl;
				}
				system("PAUSE");
				return 0;	// 按鼠标右键退出程序
			}
		}
		if (!happy)
			cleardevice();
		if (!initial && ballMoving)
		{
			cleardevice();
			InOrderTraverse(root);
			cball = Createcball();
			initial = true;
			ballMoving = false;
		}
		if (ballMoving)	drawColBall(&cball, WINDOWWITH / 2, WINDOWHEIGHT);
		moving(movingroot, root);
		InOrderTraverse(root);
		TCHAR s[100];
		HWND hWnd = GetHWnd();
		_stprintf_s(s, _T("第%d关:%d/%d                   剩余时间:%d                   用户名："), part,point,checkpoint,T);
		_tcscat_s(s, name);
		SetWindowText(hWnd, s);
		Sleep(10);
		EndBatchDraw();
	}
	closegraph();
	kdtree_destroy(root);
	if (!_CrtDumpMemoryLeaks())
	{
		cout << "已经结束游戏" << endl;
	}
	system("PAUSE");
	
		
	return 0;
}
