#include<stdio.h>
#include<iostream>
#include<graphics.h>
#include<Windows.h>
#include<mmsystem.h>
#pragma comment(lib,"Winmm.lib")//用于播放wav格式音效g
#include<conio.h>
#include<cstring>
#include<time.h>//时间种子来实现随机数l
#include<thread>//多线程
#include<math.h>
using namespace std;
struct Point {
	int row;
	int col;
};
#define N 15
#define block_width 40
#define margin 30
const int board_width = (N - 1)*block_width+2*margin;
int chessbox[N][N] = { 0 };//值是1则黑，值是2则白
int js1, js2;//角色1、角色2
int check1 = 0, check2 = 0;
int regret_nouse = 0;
int humanScore[N][N] = { 0 };
int aiScore[N][N] = { 0 };
int aiNum=0;
int humanNum=0;
int emptyNum=0;
int AI = 0;
int AI_difficulty;
char chess1[20];//玩家1棋子
char chess2[20];//玩家2棋子
char juese1[20];
char juese2[20];
void sethomepage();
void setgamepage();
void setintropage();
void choosechess();
void choosemode();
void drawchess(int row,int col,int color);
void play();
void AI_play();
void player1();
void player2();
void AI_player2();
void player1win();
void player2win();
void regretplay(int n);
void upgradepage(int n);
void reinitialize();
void calculatehumanScore(int goalx,int goaly);
void calculateaiScore(int goalx,int goaly);
void calculateScore(int row,int col);
void setScore(int goalx,int goaly);
void clearhumanScore(int goalx, int goaly);
void clearaiScore(int goalx, int goaly);
void backmusic();
int checkclick(ExMessage msg);//返回0则没有点在有效位置，返回1则左上，2右上，3左下，4右下
int checkwin(int row,int col,int n);
int evaluateValue(int row, int col);
int Max_Value(int row, int col, int depth,int alpha,int beta);
int Min_Value(int row, int col, int depth,int alpha,int beta);
int now_row1, now_col1, now_row2, now_col2;
int directs[4][2] = { {1,0},{1,1},{0,1},{-1,1} };
Point getMinMaxEvaluate(int depth);
bool musicplay = true;
int main()
{
	PlaySound(TEXT("背景音乐.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	//mciSendString(_T("open D:\\五子棋\\背景音乐.mp3 alias bkmusic"), NULL, 0, NULL);
	//mciSendString(_T("play bkmusic repeat"), NULL, 0, NULL);
	sethomepage();
	_getch();
}
void sethomepage()
{
	initgraph(770, 730);
	HWND hwnd = GetHWnd();
	MoveWindow(hwnd, 370, 30, 770, 730, TRUE);
	IMAGE img;
	loadimage(&img, _T("D:\\五子棋\\主页.png"),770, 730);
	putimage(0,0,&img);
	ExMessage m;
	while (1)
	{
		m = getmessage(EM_MOUSE);
		if (m.message == WM_LBUTTONDOWN)
		{
			if (m.x > 260 && m.y > 250 && m.x < 480 && m.y < 310)
			{
				choosemode();
				choosechess();
				setgamepage();
				play();
			}
			if (m.x > 260 && m.y > 340 && m.x < 480 && m.y < 400)
			{
				setintropage();
			}
			if(m.x>260&&m.y>430&&m.x<480&&m.y<490)
			{ 
				closegraph();
				exit(0);
			}
		}
	}
	_getch();
}
void choosechess()
{
	initgraph(770, 730);
	IMAGE img;
	loadimage(&img, _T("D:\\五子棋\\选择角色.png"), 770, 730);
	putimage(0, 0, &img);
	setcolor(BLACK);
	ExMessage m;
	while (1)
	{
		if (check1 != 0 && check2 != 0)
			break;
		m = getmessage(EM_MOUSE);
		if (m.message == WM_LBUTTONDOWN)
		{
			if (check1 == 0)
			{
				if (m.y > 140 && m.y < 300)
				{
					if (m.x > 25 && m.x < 135&&js2!=1)
					{
						js1 = 1;
					}
					if (m.x > 145 && m.x < 250&&js2!=2)
					{
						js1 = 2;
					}
					if (m.x > 263 && m.x < 370&&js2!=3)
					{
						js1 = 3;
					}
					if (m.x > 383 && m.x < 490&&js2!=4)
					{
						js1 = 4;
					}
					if (m.x > 503 && m.x < 605&&js2!=5)
					{
						js1 = 5;
					}
					if (m.x > 627 && m.x < 745&&js2!=6)
					{
						js1 = 6;
					}
					if (js1 != 0)
					{
						setbkmode(TRANSPARENT);
						settextcolor(BLACK);
						settextstyle(27, 27, _T("Consolas"));
						outtextxy(65 + 120 * (js1 - 1), 310, "√");
						outtextxy(65 + 120 * (js1 - 1), 690, "x");
						check1 = 1;
					}
				}
			}
			if (check2 == 0)
			{
				if (m.y > 520 && m.y < 680)
				{
					if (m.x > 25 && m.x < 135&&js1!=1)
					{
						js2 = 1;
					}
					if (m.x > 145 && m.x < 250&&js1!=2)
					{
						js2 = 2;
					}
					if (m.x > 263 && m.x < 370&&js1!=3)
					{
						js2 = 3;
					}
					if (m.x > 383 && m.x < 490&&js1!=4)
					{
						js2 = 4;
					}
					if (m.x > 503 && m.x < 605&&js1!=5)
					{
						js2 = 5;
					}
					if (m.x > 627 && m.x < 745&&js1!=6)
					{
						js2 = 6;
					}
					if (js2 != 0)
					{
						setbkmode(TRANSPARENT);
						settextcolor(BLACK);
						settextstyle(27, 27, _T("Consolas"));
						outtextxy(65 + 120 * (js2 - 1), 690, "√");
						outtextxy(65 + 120 * (js2 - 1), 310, "x");
						check2 = 1;
					}
				}
			}
		}
	}
}
void choosemode()
{
	initgraph(770, 730);
	IMAGE img;
	loadimage(&img, _T("D:\\五子棋\\模式选择.png"),770,730);
	putimage(0, 0, &img);
	ExMessage m;
	while (1)
	{
		m = getmessage(EM_MOUSE);
		if (m.message == WM_LBUTTONDOWN)
		{
			if (m.x > 275 && m.y > 110 && m.x < 495 && m.y < 195)
			{
				choosechess();
				setgamepage();
				play();
			}
			if (m.x > 275 && m.y > 205 && m.x < 495 && m.y < 280)
			{
				AI = 1;
				AI_difficulty = 1;
				choosechess();
				setgamepage();
				AI_play();
			}
			if (m.x > 275 && m.y > 295 && m.x < 495 && m.y < 370)
			{
				AI = 1;
				AI_difficulty = 2;
				choosechess();
				setgamepage();
				AI_play();
			}
			if (m.x > 275 && m.y > 283 && m.x < 495 && m.y < 460)
			{
				AI = 1;
				AI_difficulty = 3;
				choosechess();
				setgamepage();
				AI_play();
			}
		}
	}
}
void setintropage()
{
	initgraph(770, 730);
	IMAGE img;
	loadimage(&img, _T("D:\\五子棋\\游戏说明.png"),770,730);
	putimage(0, 0, &img);
	ExMessage m;
	while (1)
	{
		m = getmessage(EM_MOUSE);
		if (m.message == WM_LBUTTONDOWN)
		{
			if (m.x > 400 && m.y > 400 && m.x < 580 && m.y < 460)
			{
				sethomepage();
			}
		}
	}
}
void setgamepage()
{
	initgraph(770, 730);
	setbkcolor(RGB(153, 201, 223));
	cleardevice();
	IMAGE img, img3;
	loadimage(&img, _T("D:\\五子棋\\背景2.jpg"), 200, 470);
	putimage(570, 230, &img);
	loadimage(&img3, _T("D:\\五子棋\\背景3.png"), 370, 140);
	putimage(0, 590, &img3);
	setcolor(BLACK);
	for (int i = 0; i <= N-1; i++)
	{
		line(margin + i * block_width, margin, margin + i * block_width, board_width - margin);
		line(margin, margin + i * block_width, board_width - margin, margin + i * block_width);
	}
	setbkmode(TRANSPARENT);
	settextstyle(15, 10, _T("Consolas"));
	for (int i = 0; i < N; i++)
	{
		char str[3];
		sprintf_s(str, "%d", i);//将数字格式化输出为字符串
		outtextxy(margin + i * block_width - 5, margin - 15, str);
	}
	for (int i = 0; i < N; i++)
	{
		char str[2];
		str[0] = 'A' + i;
		str[1] = '\0';
		outtextxy(margin-15, margin + i * block_width-5, str);
	}
	setfillcolor(BLACK);
	solidcircle(margin + 3 * block_width, margin + 3 * block_width, 3);
	circle(margin + 3 * block_width, margin + 3 * block_width, 3);//画两遍圆是因为如果只画一遍的话圆根本就不圆（我也不知为什么）
	solidcircle(margin + 3 * block_width, margin + 11 * block_width, 3);
	circle(margin + 3 * block_width, margin + 11 * block_width, 3);
	solidcircle(margin + 11 * block_width, margin + 3 * block_width, 3);
	circle(margin + 11 * block_width, margin + 3 * block_width, 3);
	solidcircle(margin + 11 * block_width, margin + 11 * block_width, 3);
	circle(margin + 11 * block_width, margin + 11 * block_width, 3);
	solidcircle(margin + 7 * block_width, margin + 7 * block_width, 3);
	circle(margin + 7 * block_width, margin + 7 * block_width, 3);
	settextstyle(17, 13, "幼圆");
	outtextxy(610, margin-15, "玩家1：");
	if (AI == 0)
	{
		outtextxy(610, margin + 170, "玩家2：");
	}
	else outtextxy(610, margin + 170, "AI：");
	sprintf_s(juese1, "D:\\五子棋\\角色%d.jpg", js1);
	sprintf_s(juese2, "D:\\五子棋\\角色%d.jpg", js2);
	IMAGE img1, img2, img4;
	loadimage(&img1, _T(juese1), 130, 115);
	loadimage(&img2, _T(juese2), 130, 115);
	putimage(615, margin+10, &img1);
	putimage(615, margin + 195, &img2);
	setcolor(BLACK);
	rectangle(615, margin + 10, 745, margin+125);
	rectangle(615, margin + 195, 745, margin + 310);
	settextstyle(17, 13, "幼圆");
	outtextxy(625, margin + 130, "你的回合");
	loadimage(&img4, _T("D:\\五子棋\\悔棋.png"));
	putimage(290, 630, &img4);
}
void drawchess(int row, int col, int color)
{
	int x = margin + col * block_width;
	int y = margin + row * block_width;
	sprintf_s(chess1, "D:\\五子棋\\棋子%d.png", js1);
	sprintf_s(chess2, "D:\\五子棋\\棋子%d.png", js2);
	if (color == BLACK)
	{
		IMAGE img;
		loadimage(&img, _T(chess1), 28, 28);
		putimage(x-14, y-14, &img);
		chessbox[row][col] = 1;
		//PlaySound(TEXT("luozi.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
		upgradepage(2);
	}
	else
	{
		IMAGE img;
		loadimage(&img, _T(chess2), 28, 28);
		putimage(x - 14, y - 14, &img);
		chessbox[row][col] = 2;
		upgradepage(1);
	}
	if (checkwin(row, col, chessbox[row][col]))
	{
		if (chessbox[row][col] == 1)
			player1win();
		else player2win();
	}
}
int checkclick(ExMessage msg)
{
	double checkbound = block_width * 0.4;
	int col = (msg.x - margin) / block_width;
	int row = (msg.y - margin) / block_width;
	if (row < 14 && col < 14)
	{
		int left_topx = margin + col * block_width;
		int left_topy = margin + row * block_width;
		int right_topx = left_topx + block_width;
		int right_topy = left_topy;
		int left_bottomx = left_topx;
		int left_bottomy = left_topy + block_width;
		int right_bottomx = right_topx;
		int right_bottomy = left_bottomy;
		double s1 = sqrt(pow(double(msg.x - left_topx), 2) + pow(double(msg.y - left_topy), 2));
		double s2 = sqrt(pow(double(msg.x - right_topx), 2) + pow(double(msg.y - right_topy), 2));
		double s3 = sqrt(pow(double(msg.x - left_bottomx), 2) + pow(double(msg.y - left_bottomy), 2));
		double s4 = sqrt(pow(double(msg.x - right_bottomx), 2) + pow(double(msg.y - right_bottomy), 2));
		if (s1 < checkbound)
		{
			if (chessbox[row][col] == 0)
				return 1;
		}
		else
		{
			if (s2 < checkbound)
			{
				if (chessbox[row][col + 1] == 0)
					return 2;
			}
			else
			{
				if (s3 < checkbound)
				{
					if (chessbox[row + 1][col] == 0)
						return 3;
				}
				else
				{
					if (s4 < checkbound)
						if (chessbox[row + 1][col + 1] == 0)
							return 4;
				}
			}
		}
	}
	if (row == 14)
	{
		int left_topx = margin + col * block_width;
		int left_topy = margin + row * block_width;
		int right_topx = left_topx + block_width;
		int right_topy = left_topy;
		double s1 = sqrt(pow(double(msg.x - left_topx), 2) + pow(double(msg.y - left_topy), 2));
		double s2 = sqrt(pow(double(msg.x - right_topx), 2) + pow(double(msg.y - right_topy), 2));
		if (s1 < checkbound)
		{
			if (chessbox[row][col] == 0)
				return 1;
		}
		else
		{
			if (s2 < checkbound)
			{
				if (chessbox[row][col + 1] == 0)
					return 2;
			}
		}
	}
	if (col == 14)
	{
		int left_topx = margin + col * block_width;
		int left_topy = margin + row * block_width;
		int left_bottomx = left_topx;
		int left_bottomy = left_topy + block_width;
		double s1 = sqrt(pow(double(msg.x - left_topx), 2) + pow(double(msg.y - left_topy), 2));
		double s3 = sqrt(pow(double(msg.x - left_bottomx), 2) + pow(double(msg.y - left_bottomy), 2));
		if (s1 < checkbound)
		{
			if (chessbox[row][col] == 0)
				return 1;
		}
		else
		{
			if (s3 < checkbound)
			{
				if (chessbox[row+1][col] == 0)
					return 3;
			}
		}
	}
	return 0;
}
void player1()
{
	int r;
	if (AI == 1)
		r = 3;
	else r = 2;
	if (regret_nouse == 1)
	{
		regret_nouse = 0;
		return;
	}
	S:
	int color = BLACK;
	ExMessage m;
	m = getmessage(EM_MOUSE);
	if (m.message == WM_LBUTTONDOWN)
	{
		int col = (m.x - margin) / block_width;
		int row = (m.y - margin) / block_width;
		if (checkclick(m) == 1)
		{
			drawchess(row, col, color);
			now_row1 = row;
			now_col1 = col;
		}
		else if (checkclick(m) == 2)
		{
			drawchess(row, col + 1, color);
			now_row1 = row;
			now_col1 = col + 1;
		}
		else if (checkclick(m) == 3)
		{
			drawchess(row + 1, col, color);
			now_row1 = row + 1;
			now_col1 = col;
		}
		else if (checkclick(m) == 4)
		{
			drawchess(row + 1, col + 1, color);
			now_row1 = row + 1;
			now_col1 = col + 1;
		}
		else if (m.x > 295 && m.y > 640 && m.x < 393 && m.y < 690)
		{
			regretplay(r);
			if (AI == 1)
				goto S;
		}
		else if (m.x > 425 && m.y > 640 && m.x < 565 && m.y < 690)
		{
			backmusic();
			goto S;
		}
		else goto S;
	}
	else goto S;
	clearaiScore(now_row1,now_col1);
}
void player2()
{
	if (regret_nouse == 1)
	{
		regret_nouse = 0;
		return;
	}
	S:
	int color = WHITE;
	ExMessage m;
	m = getmessage(EM_MOUSE);
	if (m.message == WM_LBUTTONDOWN)
	{
		int col = (m.x - margin) / block_width;
		int row = (m.y - margin) / block_width;
		if (checkclick(m) == 1)
		{
			drawchess(row, col, color);
			now_row2 = row;
			now_col2 = col;
		}
		else if (checkclick(m) == 2)
		{
			drawchess(row, col + 1, color);
			now_row2 = row;
			now_col2 = col + 1;
		}
		else if (checkclick(m) == 3)
		{
			drawchess(row + 1, col, color);
			now_row2 = row + 1;
			now_col2 = col;
		}
		else if (checkclick(m) == 4)
		{
			drawchess(row + 1, col + 1, color);
			now_row2 = row + 1;
			now_col2 = col + 1;
		}
		else if (m.x > 295 && m.y > 640 && m.x < 393 && m.y < 690)
		{
			regretplay(1);
		}
		else if (m.x > 425 && m.y > 640 && m.x < 565 && m.y < 690)
		{
			backmusic();
			goto S;
		}
		else goto S;
	}
	else goto S;
}
void AI_player2()
{
	if (regret_nouse == 1)
	{
		regret_nouse = 0;
		return;
	}
	Point P = getMinMaxEvaluate(AI_difficulty);
	if (AI_difficulty != 3)
	{
		Sleep(200);
	}
	drawchess(P.row, P.col, WHITE);
	chessbox[P.row][P.col] = 2;
	now_row2 = P.row;
	now_col2 = P.col;
	clearhumanScore(now_row2, now_col2);
	int max1 = humanScore[0][0];
	int max2 = aiScore[0][0];
	int goalrow1 = 0, goalcol1 = 0, goalrow2 = 0, goalcol2 = 0;
	/*for (int i = 0; i < 15; i++)
	{
		for (int j = 0; j < 15; j++)
		{
			if (humanScore[i][j] > max1)
			{
				max1 = humanScore[i][j];
				goalrow1 = i;
				goalcol1 = j;
			}
			else if (humanScore[i][j] == max1)
			{
				if (aiScore[i][j] > aiScore[goalrow1][goalcol1])
				{
					goalrow1 = i;
					goalcol1 = j;
				}
			}
			if (aiScore[i][j] > max2)
			{
				max2 = aiScore[i][j];
				goalrow2 = i;
				goalcol2 = j;
			}
			else if (aiScore[i][j] == max2)
			{
				if (humanScore[i][j] > humanScore[goalrow2][goalcol2])
				{
					goalrow2 = i;
					goalcol2 = j;
				}
			}
		}
	}
	if (max1 > max2)
	{
		drawchess(goalrow1, goalcol1, WHITE);
		chessbox[goalrow1][goalcol1] = 2;
		now_row = goalrow1;
		now_col = goalcol1;
		clearhumanScore(now_row, now_col);
	}
	else
	{
		drawchess(goalrow2, goalcol2, WHITE);
		chessbox[goalrow2][goalcol2] = 2;
		now_row = goalrow2;
		now_col = goalcol2;
		clearhumanScore(now_row, now_col);
	}*/
}
void play()
{
	while (1)
	{
		player1();
		player2();
	}
}
void AI_play()
{
	while (1)
	{
		player1();
		/*calculateScore();
		for (int i = 0; i < 15; i++)
		{
			for (int j = 0; j < 15; j++)
				cout << humanScore[i][j] << " "; 
			cout << endl;
		}
		cout << endl;
		for (int i = 0; i < 15; i++)
		{
			for (int j = 0; j < 15; j++)
				cout << aiScore[i][j] << " ";
			cout << endl;
		}
		cout << endl;*/
		AI_player2();
		/*calculateScore();
		for (int i = 0; i < 15; i++)
		{
			for (int j = 0; j < 15; j++)
				cout << humanScore[i][j] << " ";
			cout << endl;
		}
		cout << endl;
		for (int i = 0; i < 15; i++)
		{
			for (int j = 0; j < 15; j++)
				cout << aiScore[i][j] << " ";
			cout << endl;
		}
		cout << endl;*/
	}
}
int checkwin(int row,int col,int n)
{
	int j = 0;
	{
		int tmp = row;
		while (tmp >= 0)
		{
			if (chessbox[--tmp][col] == n)
				j++;
			else break;
		}
		tmp = row;
		while (tmp <= 14)
		{
			if (chessbox[++tmp][col] == n)
				j++;
			else break;
		}
		if (j >= 4)
			return 1;
		else j = 0;
	}//检查y方向
	{
		int tmp = col;
		while (tmp >= 0)
		{
			if (chessbox[row][--tmp] == n)
				j++;
			else break;
		}
		tmp = col;
		while (tmp <= 14)
		{
			if (chessbox[row][++tmp] == n)
				j++;
			else break;
		}
		if (j >= 4)
			return 1;
		else j = 0;
	}//检查x方向
	{
		int tmp1 = row, tmp2 = col;
		while (tmp1 >= 0 && tmp2 <= 14)
		{
			if (chessbox[--tmp1][++tmp2] == n)
				j++;
			else break;
		}
		tmp1 = row, tmp2 = col;
		while (tmp1 <= 14 && tmp2 >= 0)
		{
			if (chessbox[++tmp1][--tmp2] == n)
				j++;
			else break;
		}
		if (j >= 4)
			return 1;
		else j = 0;
	}//检查y=x方向
	{
		int tmp1 = row, tmp2 = col;
		while (tmp1 >= 0 && tmp2 >= 0)
		{
			if (chessbox[--tmp1][--tmp2] == n)
				j++;
			else break;
		}
		tmp1 = row, tmp2 = col;
		while (tmp1 <= 14 && tmp2 <= 14)
		{
			if (chessbox[++tmp1][++tmp2] == n)
				j++;
			else break;
		}
		if (j >= 4)
			return 1;
		else return 0;
	}//检查y=-x方向
}
void player1win()
{
	Sleep(330);
	reinitialize();
	initgraph(770, 730);
	IMAGE img;
	loadimage(&img, _T("D:\\五子棋\\获胜界面1.png"),770,730);
	putimage(0, 0, &img);
	ExMessage m;
	while (1)
	{
		m = getmessage(EM_MOUSE);
		if (m.message == WM_LBUTTONDOWN)
		{
			if (m.x > 115 && m.x < 302 && m.y>610 && m.y < 650)
			{
				setgamepage();
				if (AI == 0)
					play();
				else AI_play();
			}
			if (m.x > 480 && m.x < 665 && m.y>610 && m.y < 650)
			{
				AI = 0;
				js1 = 0;
				js2 = 0;
				sethomepage();
			}
		}
	}
}
void player2win()
{
	Sleep(330);
	reinitialize();
	initgraph(770, 730);
	IMAGE img;
	loadimage(&img, _T("D:\\五子棋\\获胜界面2.png"), 770, 730);
	putimage(0, 0, &img);
	ExMessage m;
	while (1)
	{
		m = getmessage(EM_MOUSE);
		if (m.message == WM_LBUTTONDOWN)
		{
			if (m.x > 115 && m.x < 302 && m.y>610 && m.y < 650)
			{
				setgamepage();
				if (AI == 0)
					play();
				else AI_play();
			}
			if (m.x > 480 && m.x < 665 && m.y>610 && m.y < 650)
			{
				AI = 0;
				js1 = 0;
				js2 = 0;
				sethomepage();
			}
		}
	}
}
void regretplay(int n)
{
	int now_row, now_col;
	if (n == 1)
	{
		now_col = now_col1;
		now_row = now_row1;
	}
	if (n == 2)
	{
		now_row = now_row2;
		now_col = now_col2;
	}
	if (n == 3)
	{
		if (chessbox[now_row1][now_col1] == 0)
		{
			regret_nouse = 1;
			return;
		}
		cleardevice();
		chessbox[now_row1][now_col1] = 0;
		chessbox[now_row2][now_col2] = 0;
		{
			cleardevice();
			IMAGE img, img3;
			loadimage(&img, _T("D:\\五子棋\\背景2.jpg"), 200, 470);
			putimage(570, 230, &img);
			loadimage(&img3, _T("D:\\五子棋\\背景3.png"), 370, 140);
			putimage(0, 590, &img3);
			setcolor(BLACK);
			for (int i = 0; i <= N - 1; i++)
			{
				line(margin + i * block_width, margin, margin + i * block_width, board_width - margin);
				line(margin, margin + i * block_width, board_width - margin, margin + i * block_width);
			}
			setbkmode(TRANSPARENT);
			settextstyle(15, 10, _T("Consolas"));
			for (int i = 0; i < N; i++)
			{
				char str[3];
				sprintf_s(str, "%d", i);//将数字格式化输出为字符串
				outtextxy(margin + i * block_width - 5, margin - 15, str);
			}
			for (int i = 0; i < N; i++)
			{
				char str[2];
				str[0] = 'A' + i;
				str[1] = '\0';
				outtextxy(margin - 15, margin + i * block_width - 5, str);
			}
			setfillcolor(BLACK);
			solidcircle(margin + 3 * block_width, margin + 3 * block_width, 3);
			circle(margin + 3 * block_width, margin + 3 * block_width, 3);//画两遍圆是因为如果只画一遍的话圆根本就不圆（我也不知为什么）
			solidcircle(margin + 3 * block_width, margin + 11 * block_width, 3);
			circle(margin + 3 * block_width, margin + 11 * block_width, 3);
			solidcircle(margin + 11 * block_width, margin + 3 * block_width, 3);
			circle(margin + 11 * block_width, margin + 3 * block_width, 3);
			solidcircle(margin + 11 * block_width, margin + 11 * block_width, 3);
			circle(margin + 11 * block_width, margin + 11 * block_width, 3);
			solidcircle(margin + 7 * block_width, margin + 7 * block_width, 3);
			circle(margin + 7 * block_width, margin + 7 * block_width, 3);
			settextstyle(17, 13, "幼圆");
			outtextxy(610, margin - 15, "玩家1：");
			outtextxy(610, margin + 170, "AI：");
			sprintf_s(juese1, "D:\\五子棋\\角色%d.jpg", js1);
			sprintf_s(juese2, "D:\\五子棋\\角色%d.jpg", js2);
			IMAGE img1, img2, img4;
			loadimage(&img1, _T(juese1), 130, 115);
			loadimage(&img2, _T(juese2), 130, 115);
			putimage(615, margin + 10, &img1);
			putimage(615, margin + 195, &img2);
			setcolor(BLACK);
			rectangle(615, margin + 10, 745, margin + 125);
			rectangle(615, margin + 195, 745, margin + 310);
			settextstyle(17, 13, "幼圆");
			outtextxy(625, margin + 130, "你的回合");
			loadimage(&img4, _T("D:\\五子棋\\悔棋.png"));
			putimage(290, 630, &img4);
		}//重画游戏界面
		for (int i = 0; i <= 14; i++)
			for (int j = 0; j <= 14; j++)
			{
				if (chessbox[i][j] == 1)
					drawchess(i, j, BLACK);
				if (chessbox[i][j] == 2)
					drawchess(i, j, WHITE);
			}
		upgradepage(1);
		return;
	}
	if (chessbox[now_row][now_col] == 0)
	{
		regret_nouse = 1;
		return;
	}
	cleardevice();
	int tmp = chessbox[now_row][now_col];
	chessbox[now_row][now_col] = 0;
	{
		cleardevice();
		IMAGE img, img3;
		loadimage(&img, _T("D:\\五子棋\\背景2.jpg"), 200, 470);
		putimage(570, 230, &img);
		loadimage(&img3, _T("D:\\五子棋\\背景3.png"), 370, 140);
		putimage(0, 590, &img3);
		setcolor(BLACK);
		for (int i = 0; i <= N - 1; i++)
		{
			line(margin + i * block_width, margin, margin + i * block_width, board_width - margin);
			line(margin, margin + i * block_width, board_width - margin, margin + i * block_width);
		}
		setbkmode(TRANSPARENT);
		settextstyle(15, 10, _T("Consolas"));
		for (int i = 0; i < N; i++)
		{
			char str[3];
			sprintf_s(str, "%d", i);//将数字格式化输出为字符串
			outtextxy(margin + i * block_width - 5, margin - 15, str);
		}
		for (int i = 0; i < N; i++)
		{
			char str[2];
			str[0] = 'A' + i;
			str[1] = '\0';
			outtextxy(margin - 15, margin + i * block_width - 5, str);
		}
		setfillcolor(BLACK);
		solidcircle(margin + 3 * block_width, margin + 3 * block_width, 3);
		circle(margin + 3 * block_width, margin + 3 * block_width, 3);//画两遍圆是因为如果只画一遍的话圆根本就不圆（我也不知为什么）
		solidcircle(margin + 3 * block_width, margin + 11 * block_width, 3);
		circle(margin + 3 * block_width, margin + 11 * block_width, 3);
		solidcircle(margin + 11 * block_width, margin + 3 * block_width, 3);
		circle(margin + 11 * block_width, margin + 3 * block_width, 3);
		solidcircle(margin + 11 * block_width, margin + 11 * block_width, 3);
		circle(margin + 11 * block_width, margin + 11 * block_width, 3);
		solidcircle(margin + 7 * block_width, margin + 7 * block_width, 3);
		circle(margin + 7 * block_width, margin + 7 * block_width, 3);
		settextstyle(17, 13, "幼圆");
		outtextxy(610, margin - 15, "玩家1：");
		outtextxy(610, margin + 170, "玩家2：");
		sprintf_s(juese1, "D:\\五子棋\\角色%d.jpg", js1);
		sprintf_s(juese2, "D:\\五子棋\\角色%d.jpg", js2);
		IMAGE img1, img2, img4;
		loadimage(&img1, _T(juese1), 130, 115);
		loadimage(&img2, _T(juese2), 130, 115);
		putimage(615, margin + 10, &img1);
		putimage(615, margin + 195, &img2);
		setcolor(BLACK);
		rectangle(615, margin + 10, 745, margin + 125);
		rectangle(615, margin + 195, 745, margin + 310);
		settextstyle(17, 13, "幼圆");
		outtextxy(625, margin + 130, "你的回合");
		loadimage(&img4, _T("D:\\五子棋\\悔棋.png"));
		putimage(290, 630, &img4);
	}//重画游戏界面
	for(int i=0;i<=14;i++)
		for (int j = 0; j <= 14; j++)
		{
			if (chessbox[i][j] == 1)
				drawchess(i, j, BLACK);
			if (chessbox[i][j] == 2)
				drawchess(i, j, WHITE);
		}
	upgradepage(tmp);
}
void backmusic()
{
	if (musicplay == true)
	{
		PlaySound(NULL, NULL, NULL);
		musicplay = false;
	}
	else
	{
		PlaySound(TEXT("背景音乐.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
		musicplay = true;
	}
}
void upgradepage(int n)
{
	clearrectangle(625, margin + 130, 625 + 150, margin + 150);
	clearrectangle(625, margin + 315, 625 + 150, margin + 335);
	if (n == 1)
	{
		settextstyle(17, 13, "幼圆");
		outtextxy(625, margin + 130, "你的回合");
	}
	else
	{
		settextstyle(17, 13, "幼圆");
		outtextxy(625, margin + 315, "你的回合");
	}
}
void reinitialize()
{ 
    for (int i = 0; i < 15; i++)
		for (int j = 0; j < 15; j++)
		{
			chessbox[i][j] = 0;
			humanScore[i][j] = 0;
			aiScore[i][j] = 0;
		}
	check1 = 0;
	check2 = 0;
}
void calculatehumanScore(int goalrow, int goalcol)
{
	{
		humanNum = 1;
		int tmp = goalrow;
		while (tmp >= 0)
		{
			if (chessbox[--tmp][goalcol] == 1)
				humanNum++;
			else
			{
				if (tmp > 0 && tmp < 15)
				{
					if (chessbox[tmp][goalcol] == 0)
					{
						emptyNum++;
					}
					if (chessbox[tmp][goalcol] == 2)
						aiNum++;
				}
				else aiNum++;
				break;
			}
		}
		tmp = goalrow;
		while (tmp <= 14)
		{
			if (chessbox[++tmp][goalcol] == 1)
				humanNum++;
			else
			{
				if (tmp > 0 && tmp < 15)
				{
					if (chessbox[tmp][goalcol] == 0)
					{
						emptyNum++;
					}
					if (chessbox[tmp][goalcol] == 2)
						aiNum++;
				}
				else aiNum++;
				break;
			}
		}
		setScore(goalrow, goalcol);
	}//y方向
	{
		humanNum = 1;
		int tmp = goalcol;
		while (tmp >= 0)
		{
			if (chessbox[goalrow][--tmp] == 1)
				humanNum++;
			else
			{
				if (tmp > 0 && tmp < 15)
				{
					if (chessbox[goalrow][tmp] == 0)
					{
						emptyNum++;
					}
					if (chessbox[goalrow][tmp] == 2)
						aiNum++;
				}
				else aiNum++;
				break;
			}
		}
		tmp = goalcol;
		while (tmp <= 14)
		{
			if (chessbox[goalrow][++tmp] == 1)
				humanNum++;
			else
			{
				if (tmp > 0 && tmp < 15)
				{
					if (chessbox[goalrow][tmp] == 0)
					{
						emptyNum++;
					}
					if (chessbox[goalrow][tmp] == 2)
						aiNum++;
				}
				else aiNum++;
				break;
			}
		}
		setScore(goalrow, goalcol);
	}//检查x方向
	{
		humanNum = 1;
		int tmp1 = goalrow, tmp2 = goalcol;
		while (tmp1 >= 0 && tmp2 <= 14)
		{
			if (chessbox[--tmp1][++tmp2] == 1)
				humanNum++;
			else
			{
				if (tmp1 > 0 && tmp1 < 15 && tmp2>0 && tmp2 < 15)
				{
					if (chessbox[tmp1][tmp2] == 0)
					{
						emptyNum++;
					}
					if (chessbox[tmp1][tmp2] == 2)
						aiNum++;
				}
				else aiNum++;
				break;
			}
		}
		tmp1 = goalrow, tmp2 = goalcol;
		while (tmp1 <= 14 && tmp2 >= 0)
		{
			if (chessbox[++tmp1][--tmp2] == 1)
				humanNum++;
			else
			{
				if (tmp1 > 0 && tmp1 < 15 && tmp2>0 && tmp2 < 15)
				{
					if (chessbox[tmp1][tmp2] == 0)
					{
						emptyNum++;
					}
					if (chessbox[tmp1][tmp2] == 2)
						aiNum++;
				}
				else aiNum++;
				break;
			}
		}
		setScore(goalrow, goalcol);
	}//检查y=x方向
	{
		humanNum = 1;
		int tmp1 = goalrow, tmp2 = goalcol;
		while (tmp1 >= 0 && tmp2 >= 0)
		{
			if (chessbox[--tmp1][--tmp2] == 1)
				humanNum++;
			else
			{
				if (tmp1 > 0 && tmp1 < 15 && tmp2>0 && tmp2 < 15)
				{
					if (chessbox[tmp1][tmp2] == 0)
					{
						emptyNum++;
					}
					if (chessbox[tmp1][tmp2] == 2)
						aiNum++;
				}
				else aiNum++;
				break;
			}
		}
		tmp1 = goalrow, tmp2 = goalcol;
		while (tmp1 <= 14 && tmp2 <= 14)
		{
			if (chessbox[++tmp1][++tmp2] == 1)
				humanNum++;
			else
			{
				if (tmp1 > 0 && tmp1 < 15 && tmp2>0 && tmp2 < 15)
				{
					if (chessbox[tmp1][tmp2] == 0)
					{
						emptyNum++;
					}
					if (chessbox[tmp1][tmp2] == 2)
						aiNum++;
				}
				else aiNum++;
				break;
			}
		}
		setScore(goalrow, goalcol);
	}//检查y=-x方向
}
void calculateaiScore(int goalrow,int goalcol)
{
	{
		aiNum = 1;
		int tmp = goalrow;
		while (tmp >= 0)
		{
			if (chessbox[--tmp][goalcol] == 2)
				aiNum++;
			else
			{
				if (tmp > 0 && tmp < 15)
				{
					if (chessbox[tmp][goalcol] == 0)
					{
						emptyNum++;
					}
					if (chessbox[tmp][goalcol] == 1)
						humanNum++;
				}
				else humanNum++;
				break;
			}
		}
		tmp = goalrow;
		while (tmp <= 14)
		{
			if (chessbox[++tmp][goalcol] == 2)
				aiNum++;
			else
			{
				if (tmp > 0 && tmp < 15)
				{
					if (chessbox[tmp][goalcol] == 0)
					{
						emptyNum++;
					}
					if (chessbox[tmp][goalcol] == 1)
						humanNum++;
				}
				else humanNum++;
				break;
			}
		}
		setScore(goalrow, goalcol);
	}//y方向
	{
		aiNum = 1;
		int tmp = goalcol;
		while (tmp >= 0)
		{
			if (chessbox[goalrow][--tmp] == 2)
				aiNum++;
			else
			{
				if (tmp > 0 && tmp < 15)
				{
					if (chessbox[goalrow][tmp] == 0)
					{
						emptyNum++;
					}
					if (chessbox[goalrow][tmp] == 1)
						humanNum++;
				}
				else humanNum++;
				break;
			}
		}
		tmp = goalcol;
		while (tmp <= 14)
		{
			if (chessbox[goalrow][++tmp] == 2)
				aiNum++;
			else
			{
				if (tmp > 0 && tmp < 15)
				{
					if (chessbox[goalrow][tmp] == 0)
					{
						emptyNum++;
					}
					if (chessbox[goalrow][tmp] == 1)
						humanNum++;
				}
				else humanNum++;
				break;
			}
		}
		setScore(goalrow, goalcol);
	}//检查x方向
	{
		aiNum = 1;
		int tmp1 = goalrow, tmp2 = goalcol;
		while (tmp1 >= 0 && tmp2 <= 14)
		{
			if (chessbox[--tmp1][++tmp2] == 2)
				aiNum++;
			else
			{
				if (tmp1 > 0 && tmp1 < 15 && tmp2>0 && tmp2 < 15)
				{
					if (chessbox[tmp1][tmp2] == 0)
					{
						emptyNum++;
					}
					if (chessbox[tmp1][tmp2] == 1)
						humanNum++;
				}
				else humanNum++;
				break;
			}
		}
		tmp1 = goalrow, tmp2 = goalcol;
		while (tmp1 <= 14 && tmp2 >= 0)
		{
			if (chessbox[++tmp1][--tmp2] == 2)
				aiNum++;
			else
			{
				if (tmp1 > 0 && tmp1 < 15 && tmp2>0 && tmp2 < 15)
				{
					if (chessbox[tmp1][tmp2] == 0)
					{
						emptyNum++;
					}
					if (chessbox[tmp1][tmp2] == 1)
						humanNum++;
				}
				else humanNum++;
				break;
			}
		}
		setScore(goalrow, goalcol);
	}//检查y=x方向
	{
		aiNum = 1;
		int tmp1 = goalrow, tmp2 = goalcol;
		while (tmp1 >= 0 && tmp2 >= 0)
		{
			if (chessbox[--tmp1][--tmp2] == 2)
				aiNum++;
			else
			{
				if (tmp1 > 0 && tmp1 < 15 && tmp2>0 && tmp2 < 15)
				{
					if (chessbox[tmp1][tmp2] == 0)
					{
						emptyNum++;
					}
					if (chessbox[tmp1][tmp2] == 1)
						humanNum++;
				}
				else humanNum++;
				break;
			}
		}
		tmp1 = goalrow, tmp2 = goalcol;
		while (tmp1 <= 14 && tmp2 <= 14)
		{
			if (chessbox[++tmp1][++tmp2] == 2)
				aiNum++;
			else
			{
				if (tmp1 > 0 && tmp1 < 15 && tmp2>0 && tmp2 < 15)
				{
					if (chessbox[tmp1][tmp2] == 0)
					{
						emptyNum++;
					}
					if (chessbox[tmp1][tmp2] == 1)
						humanNum++;
				}
				else humanNum++;
				break;
			}
		}
		setScore(goalrow, goalcol);
	}//检查y=-x方向
}
void setScore(int goalrow, int goalcol)
{
	if (humanNum >= 2)
	{
		if (humanNum == 2 && emptyNum == 1 && aiNum == 1)
		{
			if (humanScore[goalrow][goalcol] < 3)
				humanScore[goalrow][goalcol] = 3;
			else if(humanScore[goalrow][goalcol] == 3)
				humanScore[goalrow][goalcol] = 4;
		}
		if (humanNum == 2 && emptyNum == 2 )
		{
			if (humanScore[goalrow][goalcol] < 5)
				humanScore[goalrow][goalcol] = 5;
			else if (humanScore[goalrow][goalcol] == 5)
				humanScore[goalrow][goalcol] = 10;
		}
		if (humanNum == 3 && emptyNum == 1 && aiNum==1)
		{
			if(humanScore[goalrow][goalcol] < 50)
				humanScore[goalrow][goalcol] = 50;
			else if (humanScore[goalrow][goalcol] == 50)
				humanScore[goalrow][goalcol] = 100;
		}
		if (humanNum == 3 && emptyNum == 2)
		{
			if(humanScore[goalrow][goalcol] < 200)
				humanScore[goalrow][goalcol] = 200;
			else if (humanScore[goalrow][goalcol] == 200)
				humanScore[goalrow][goalcol] = 600;
		}
		if (humanNum == 4 && emptyNum == 1 && aiNum ==1)
		{
			if (humanScore[goalrow][goalcol] < 500)
				humanScore[goalrow][goalcol] = 500;
			else if (humanScore[goalrow][goalcol] == 500)
				humanScore[goalrow][goalcol] = 1000;
		}
		if (humanNum == 4 && emptyNum == 2)
		{
			if (humanScore[goalrow][goalcol] < 10000)
				humanScore[goalrow][goalcol] = 10000;
			else if (humanScore[goalrow][goalcol] == 10000)
				humanScore[goalrow][goalcol] = 50000;
		}
		if (humanNum >= 5)
		{
			if(humanScore[goalrow][goalcol] < 100000)
				humanScore[goalrow][goalcol] = 100000;
		}
	}
	if (aiNum >= 2)
	{
		if (aiNum == 2 && emptyNum == 1&&humanNum==1)
		{
			if (aiScore[goalrow][goalcol] < 3)
				aiScore[goalrow][goalcol] = 3;
			else if (aiScore[goalrow][goalcol] == 3)
				aiScore[goalrow][goalcol] = 4;
		}
		if (aiNum == 2 && emptyNum == 2)
		{
			if (aiScore[goalrow][goalcol] < 5)
				aiScore[goalrow][goalcol] = 5;
			else if (aiScore[goalrow][goalcol] == 5)
				aiScore[goalrow][goalcol] = 10;//双活二
		}
		if (aiNum == 3 && emptyNum == 1&&humanNum==1)
		{
			if (aiScore[goalrow][goalcol] < 50)
				aiScore[goalrow][goalcol] = 50;
			else if (aiScore[goalrow][goalcol] == 50)
				aiScore[goalrow][goalcol] = 100;//双眠三
		}
		if (aiNum == 3 && emptyNum == 2)
		{
			if (aiScore[goalrow][goalcol] < 200)
				aiScore[goalrow][goalcol] = 200;
			else if (aiScore[goalrow][goalcol] == 200)
				aiScore[goalrow][goalcol] = 600;//双活三（分值比眠四大）
		}
		if (aiNum == 4 && emptyNum == 1&&humanNum==1)
		{
			if (aiScore[goalrow][goalcol] < 500)
				aiScore[goalrow][goalcol] = 500;
			else if (aiScore[goalrow][goalcol] == 500)
				aiScore[goalrow][goalcol] = 1000;
		}
		if (aiNum == 4 && emptyNum == 2)
		{
			if (aiScore[goalrow][goalcol] < 10000)
				aiScore[goalrow][goalcol] = 10000;
			else if (aiScore[goalrow][goalcol] == 10000)
				aiScore[goalrow][goalcol] = 50000;
		}
		if (aiNum >= 5)
		{
			if(aiScore[goalrow][goalcol] < 100000)
				aiScore[goalrow][goalcol] = 100000;
		}
	}
	emptyNum = 0;
	aiNum = 0;
	humanNum = 0;
}
void calculateScore(int row,int col)
{
	int goalrow, goalcol;
	for (int i = 0; i < 4; i++)
	{
		int x = directs[i][0];
		int y = directs[i][1];
		for (int j = 1; j < 5; j++)
		{
			goalrow = row + j * x;
			goalcol = col + j * y;
			if (goalrow > 0 && goalrow < 15 && goalcol>0 && goalcol < 15 && chessbox[goalrow][goalcol] == 0)
			{
				calculatehumanScore(goalrow, goalcol);
				calculateaiScore(goalrow, goalcol);
			}
			goalrow = row - j * x;
			goalcol = col - j * y;
			if (goalrow > 0 && goalrow < 15 && goalcol>0 && goalcol < 15 && chessbox[goalrow][goalcol] == 0)
			{
				calculatehumanScore(goalrow, goalcol);
				calculateaiScore(goalrow, goalcol);
			}
		}
	}
}
void clearhumanScore(int goalrow, int goalcol)
{
	aiScore[goalrow][goalcol] = 0;
	humanScore[goalrow][goalcol] = 0;
	int row, col;
	for (int i = 0; i < 4; i++)
	{
		int x = directs[i][0];
		int y = directs[i][1];
		for (int j = 1; j < 5; j++)
		{
			row = goalrow + j * x;
			col = goalcol + j * y;
			if (row > 0 && row < 15 && col>0 && col < 15)
				humanScore[row][col] = 0;
			row = goalrow - j * x;
			col = goalcol - j * y;
			if (row > 0 && row < 15 && col>0 && col < 15)
				humanScore[row][col] = 0;
		}
	}
}
void clearaiScore(int goalrow, int goalcol)
{
	aiScore[goalrow][goalcol] = 0;
	humanScore[goalrow][goalcol] = 0;
	int row, col;
	for (int i = 0; i < 4; i++)
	{
		int x = directs[i][0];
		int y = directs[i][1];
		for (int j = 1; j < 5; j++)
		{
			row = goalrow + j * x;
			col = goalcol + j * y;
			if(row>0&&row<15&&col>0&&col<15)
			aiScore[row][col] = 0;
			row = goalrow - j * x;
			col = goalcol - j * y;
			if(row>0&&col<15&&col>0&&col<15)
			aiScore[row][col] = 0;
		}
	}
}
int evaluateValue(int row, int col)
{
	return aiScore[row][col] + humanScore[row][col];
}
Point getMinMaxEvaluate(int depth)//深度搜索AI
{
	Point maxPoint[N * N] = { 0 };
	int maxValue = INT_MIN;
	int k = 0;
	int alpha = INT_MIN;//alpha-beta剪枝
	int beta = INT_MAX;//alpha-beta剪枝
	for (int rowi = 0; rowi < 15; rowi++)
		for (int coli = 0; coli < 15; coli++)
		{
			if (chessbox[rowi][coli] == 0)
			{
				aiScore[rowi][coli] = 0;
				humanScore[rowi][coli] = 0;
				calculateaiScore(rowi, coli);
				calculatehumanScore(rowi, coli);
				if (aiScore[rowi][coli] == 100000)
				{
					maxPoint[k].row = rowi;
					maxPoint[k].col = coli;
					return maxPoint[k];
				}
				if (humanScore[rowi][coli] == 100000)
				{
					maxPoint[k].row = rowi;
					maxPoint[k].col = coli;
					return maxPoint[k];
				}
				chessbox[rowi][coli] = 2;
				//calculateScore(rowi, coli);
				int temp = Min_Value(rowi, coli, depth - 1, alpha, beta);
				if (temp > maxValue)
				{
					maxValue = temp;
					maxPoint[k].row = rowi;
					maxPoint[k].col = coli;
				}
				else if (temp == maxValue)
				{
					k++;
					maxPoint[k].row = rowi;
					maxPoint[k].col = coli;
				}
				chessbox[rowi][coli] = 0;
				//clearhumanScore(rowi, coli);
				//clearaiScore(rowi, coli);
				//calculateScore(rowi, coli);
			}
		}
	return maxPoint[k];
}
int Min_Value(int row, int col, int depth, int alpha, int beta)
{
	int value = evaluateValue(row, col);
	if (depth == 0)
		return value;
	int minValue = INT_MAX;
	for (int rowi = 0; rowi < 15; rowi++)
		for (int coli = 0; coli < 15; coli++)
		{
			if (chessbox[rowi][coli] == 0)
			{
				//alphai = alpha;
				aiScore[row][col] = 0;
				humanScore[row][col] = 0;
				chessbox[rowi][coli] = 1;
				calculateaiScore(row, col);
				calculatehumanScore(row, col);
				//calculateScore(rowi, coli);
				int temp = Max_Value(row, col, depth - 1, alpha, beta);
				if (temp < beta)
					beta = temp;
				if (alpha > beta)
				{
					chessbox[rowi][coli] = 0;
					aiScore[row][col] = 0;
					humanScore[row][col] = 0;
					calculateaiScore(row, col);
					calculatehumanScore(row, col);
					return beta;
				}
				if (temp < minValue)
				{
					minValue = temp;
				}
				chessbox[rowi][coli] = 0;
				aiScore[row][col] = 0;
				humanScore[row][col] = 0;
				calculateaiScore(row, col);
				calculatehumanScore(row, col);
				//clearhumanScore(row,col);
				//clearaiScore(row, col);
				//calculateScore(row, col);
			}
		}
	alpha = INT_MIN;
	return minValue;
}
int Max_Value(int row, int col, int depth, int alpha, int beta)
{
	int value = evaluateValue(row, col);
	if (depth == 0)
		return value;
	int maxValue = INT_MIN;
	for (int rowi = 0; rowi < 15; rowi++)
		for (int coli = 0; coli < 15; coli++)
		{
			if (chessbox[rowi][coli] == 0)
			{
				aiScore[row][col] = 0;
				humanScore[row][col] = 0;
				chessbox[rowi][coli] = 2;
				calculatehumanScore(row, col);
				calculateaiScore(row, col);
				//calculateScore(rowi, coli);
				int temp = Min_Value(row, col, depth - 1, alpha, beta);
				if (temp > maxValue)
				{
					maxValue = temp;
				}
				if (temp > alpha)
					alpha = temp;
				if (alpha > beta)
				{
					chessbox[rowi][coli] = 0;
					aiScore[row][col] = 0;
					humanScore[row][col] = 0;
					calculateaiScore(row, col);
					calculatehumanScore(row, col);
					return alpha;
				}
				chessbox[rowi][coli] = 0;
				aiScore[row][col] = 0;
				humanScore[row][col] = 0;
				calculateaiScore(row, col);
				calculatehumanScore(row, col);
				//clearhumanScore(row, col);
				//clearaiScore(row, col);
				//calculateScore(rowi, coli);
			}
		}
	beta = INT_MAX;
	return maxValue;
}