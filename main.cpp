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
#include <crtdbg.h>//add this header file to detect memory leaks

using namespace std;

#define WINDOWWITH 600
#define WINDOWHEIGHT 600
#define DEF_NUM_PTS 50
double speed = 4.0;
double K = 0.9958;
int swing = 0;
#define �� 3.141592653589793


COLORREF ballColorTable[] = { BLUE, GREEN, RED, YELLOW, MAGENTA, BROWN };


//������ײ��
void drawColBall(DataType* b, float x, float y)
{
	b->x = x;
	b->y = y;
	setcolor(ballColorTable[b->c]);
	setfillcolor(ballColorTable[b->c]);
	solidcircle(b->x, b->y, BALLRADIUS);
}

//�������kdtree
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
		dynamicball.dir = staticball.dir + �� / 2;
	}
	else {
		staticball.v = fabs((dynamicball.v) * cos(staticball.dir - dynamicball.dir));
		dynamicball.v *= fabs(sin(staticball.dir - dynamicball.dir));
		dynamicball.dir = staticball.dir - �� / 2;
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
		//�����ײ���Ƿ񳬳���Χ
		if (p->data.x > WINDOWWITH|| p->data.x <0 )	p->data.dir = �� - p->data.dir;
		if (p->data.y > WINDOWHEIGHT  || p->data.y < 0)	p->data.dir *= -1;

		p = p->next;
	}
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
time_t _times(time_t t1, time_t t2, bool& newball, int times, kdnode* root,float ��)
{
	if (difftime(t2, t1) > double(100))
		moveroot(root, ��);
	if (difftime(t2, t1) > double(3000 / (times + 1))) {
		newball = true;
		return t2;
	}
	return t1;
}

int main()
{
	int mode,ballspeed, fri, times=1, happy = 0;
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
\n�Ⲩ�����Ⲩ�ǿ��������֣������Ե�������(�����ڵĲ�����Χֻ�ǽ���///�ߺ������)\n(�Ƽ��淨��10 ��-100 ��100 ������꣬��ɣ����Ǻܿ�ͻ����hhh)\n\n");
	printf("\n����0��������ģʽ����������ֱ�ӿ�ʼ��Ϸ(������)\n");
	scanf_s("%d", &mode);
	if (mode == 0) {
		printf("������ٶ�\n������(1~100)��");
		scanf_s("%d", &ballspeed);
		speed += (0.05) * ballspeed;
		printf("��Ħ��ϵ��\n������(-100~100,����Խ��Ħ����Խ��,�����Ա任)��");
		scanf_s("%d", &fri);
		K -= fri * (0.00004);
		printf("�۷������ˢ���ٶ�\n������(1~100����Խ��,��ˢ�µ�Խ��)��");
		scanf_s("%d", &times);
		printf("�������������ҡ������\n������(1~3����������0)��");
		scanf_s("%d", &swing);
		printf("\n\n����������ģʽ������1(�����ã�����ģʽ����0)��");
		scanf_s("%d", &happy);
		if (happy == 1)swing = 0;
	}
	//��ʼ����ͼ����
	initgraph(WINDOWWITH, WINDOWHEIGHT);
	setbkcolor(BLACK);
	cleardevice();
	//��ĸ���
	int num_pts = DEF_NUM_PTS;
	//ָ��kdtree
	struct kdnode* root = NULL;
	//������̬������
	Node* movingroot;
	movingroot = CreateEmptyList();
	//��������С����
	DataType NearestBall;
	float dis;
	//������
	DataType Ball;
	srand(time(0));

	//���򻭳�����kdtree��(����������ֵ�Բ�ص�)
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

	//����ײ��
	DataType cball;
	cball = Createcball();
	drawColBall(&cball, WINDOWWITH / 2, WINDOWHEIGHT);
	float r = 1;

	//�¼�ѭ����������ꡢʱ���¼�
	MOUSEMSG m;
	bool ballMoving = true;
	bool initial = true;
	time_t t0 = clock(), t;
	float �� = �� / 2;

	while (root)
	{
		//moveroot(root, ��);
		�� += �� / 20;
		t = clock();
		t0 = _times(t0, t, ballMoving, times, root, ��);
		BeginBatchDraw();
		// ��ȡһ�������Ϣ
		if (MouseHit())
		{
			m = GetMouseMsg();
			switch (m.uMsg)
			{
			case WM_MOUSEMOVE:
				break;

			case WM_LBUTTONDOWN:
				//��������˶�������ʼ�˶�
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
				if (!_CrtDumpMemoryLeaks())
				{
					cout << "�Ѿ�������Ϸ" << endl;
				}
				system("PAUSE");
				return 0;	// ������Ҽ��˳�����
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

		Sleep(10);
		EndBatchDraw();
	}
	closegraph();
	kdtree_destroy(root);
	if (!_CrtDumpMemoryLeaks())
	{
		cout << "�Ѿ�������Ϸ" << endl;
	}
	system("PAUSE");

	return 0;
}
