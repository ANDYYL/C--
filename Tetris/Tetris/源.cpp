#include<Windows.h>
#include<conio.h>//控制台的键盘输入输出
#include<time.h>
#include "colorConsole.h"

HANDLE handle;//无符号的整型数唯一标识窗口ID，显示黑屏控制台
WORD wColors[7] = {
	FOREGROUND_RED | FOREGROUND_INTENSITY,
	FOREGROUND_GREEN | FOREGROUND_INTENSITY,
	FOREGROUND_BLUE | FOREGROUND_INTENSITY,
	FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY,
	FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
	FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
	FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY
};

int const STARTX = 20;//开始下落的横坐标
int const STARTY = 5;//开始下落的纵坐标
int const LEFT = 75;
int const RIGHT = 77;
int const UP = 72;
int const DOWN = 80;
int const MAPX = 12;
int const MAPY = 20;
int map[MAPY][MAPX] = { 0 };

int data[28][4][4] =
{
	{ { 1 }, { 1, 1, 1 } }, { { 1, 1 }, { 1, 0 }, { 1, 0 } }, { { 1, 1, 1 }, { 0, 0, 1 } }, { { 0, 1 }, { 0, 1 }, { 1, 1 } },
	{ { 0, 2 }, { 2, 2, 2 } }, { { 2, 0 }, { 2, 2 }, { 2, 0 } }, { { 2, 2, 2 }, { 0, 2 } }, { { 0, 2 }, { 2, 2 }, { 0, 2 } },
	{ { 3, 3 }, { 0, 3, 3 } }, { { 0, 3 }, { 3, 3 }, { 3, 0 } }, { { 3, 3 }, { 0, 3, 3 } }, { { 0, 3 }, { 3, 3 }, { 3, 0 } },
	{ { 0, 0, 4 }, { 4, 4, 4 } }, { { 4, 0 }, { 4, 0 }, { 4, 4 } }, { { 4, 4, 4 }, { 4, 0 } }, { { 4, 4 }, { 0, 4 }, { 0, 4 } },
	{ { 0, 5, 5 }, { 5, 5 } }, { { 5, 0 }, { 5, 5 }, { 0, 5 } }, { { 0, 5, 5 }, { 5, 5 } }, { { 5, 0 }, { 5, 5 }, { 0, 5 } },
	{ { 6, 6, 6, 6 } }, { { 6, 0 }, { 6, 0 }, { 6, 0 }, { 6, 0 } }, { { 6, 6, 6, 6 } }, { { 6, 0 }, { 6, 0 }, { 6, 0 }, { 6, 0 } },
	{ { 7, 7 }, { 7, 7 } }, { { 7, 7 }, { 7, 7 } }, { { 7, 7 }, { 7, 7 } }, { { 7, 7 }, { 7, 7 } }
};
void Display(int shape, int dx, int dy)
{
	for (int i = 0; i <= 3; i++)
	{
		for (int j = 0; j <= 3; j++)
		if (data[shape][i][j] != 0)
			textout(handle, STARTX + (j + dx + MAPX / 2 - 2) * 2, STARTY + i + dy, wColors + shape / 4, 1, "■");
	}
}

void Erase(int shape, int dx, int dy)
{
	for (int i = 0; i <= 3; i++)
	{
		for (int j = 0; j <= 3; j++)
		if (data[shape][i][j] != 0)
			textout(handle, STARTX + (dx + j) * 2 + MAPX - 4, STARTY + i + dy, wColors + shape / 4, 1, "  ");
	}
}

void InitMap()
{
	map[0][0] = -3;//左上角
	map[MAPY - 1][0] = -3;//左下角
	map[0][MAPX - 1] = -3;//右上角
	map[MAPY - 1][MAPX - 1] = -3;//右下角

	for (int i = 1; i<MAPX - 1; i++)
		map[0][i] = -1;//上边界

	for (int i = 1; i<MAPX - 1; i++)
		map[MAPY - 1][i] = -1;//下边界

	for (int i = 1; i<MAPY - 1; i++)
		map[i][0] = -2;//左边界

	for (int i = 1; i<MAPY - 1; i++)
		map[i][MAPX - 1] = -2;//右边界

}
void InitText()
{
	textout(handle, STARTX*3, STARTY*2, wColors, 1, "SCORE");
	textout(handle, STARTX * 3, STARTY * 2+4, wColors, 1, "LEVEL");
	//textout(handle, STARTX * 3, STARTY * 2+8, wColors, 1, "NEXT");
}


void DisplayEgde()
{
	for (int i = 0; i<MAPY; i++)
	for (int j = 0; j<MAPX; j++)
	{
		if (map[i][j] == -3)
			textout(handle, STARTX + j * 2, STARTY + i, wColors + 4, 1, "◆");
		else if (map[i][j] == -2)
			textout(handle, STARTX + j * 2, STARTY + i, wColors + 4, 1, "║");
		else if (map[i][j] == -1)
			textout(handle, STARTX + j * 2, STARTY + i, wColors + 4, 1, "═");
		Sleep(2);

	}
}

bool IsAvailable(int shape, int dx, int dy)
{
	for (int i = 0; i<4; i++)
	for (int j = 0; j<4; j++)//双重for循环理解成遍历。
	{
		if (data[shape][i][j] != 0 && map[dy + i][dx + j + MAPX / 2 - 2] != 0)//如果两者都为1（有方块），则不允许下移。
			return false;
	}
	return true;
}

void DeleteLine(int map[][MAPX], int row)
{
	for (int i = row; i>1; i--)
	for (int j = 1; j<MAPX - 1; j++)
	{
		map[i][j] = map[i - 1][j]; //行-1
	
	}
}

void Update(int row)
{
	for (int i = row; i>0; i--)
	for (int j = 1; j<MAPX - 1; j++)
	{
		if (map[i][j] == 0)
			textout(handle, STARTX + j * 2, STARTY + i, wColors, 1, "  ");
		else
			textout(handle, STARTX + j * 2, STARTY + i, wColors + map[i][j] - 1, 1, "■");
	}
}
void GameOver()
{
	textout(handle, STARTX , STARTY -2, wColors+1, 1, "GameOver");
}
void main()
{
	handle = initiate();
	srand(time(NULL));
	InitMap();
	InitText();
	DisplayEgde();

	int dx = 0, dy = 0;
	int olddx = dx;
	int olddy = dy;
	int score = 0;
	int level = 0;

	int shape = rand() % 7;
	int oldshape = shape;
	int count = 0;
	int delay = 40;//键盘检测间隔
	int max_delay = 400;
	int turn = 0;//翻转变量
	int oldturn = turn;

	

	while (1)
	{
		
		if (_kbhit())//键盘控制
		{
			int ch = _getch();//把获取到的键值赋给整形变量ch。
			if (ch == 224)
				ch = _getch();
			switch (ch)
			{
			case LEFT:
				if (IsAvailable(shape * 4 + turn % 4, dx - 1, dy))
				{
					dx -= 1;
				}
				break;
			case RIGHT:
				if (IsAvailable(shape * 4 + turn % 4, dx + 1, dy))
				{
					dx += 1;
				}
				break;
			case UP:
				if (IsAvailable(shape * 4 + (turn + 1) % 4, dx, dy))
				{
					Erase(shape * 4 + oldturn % 4, olddx, olddy);
					turn++;
					Display(shape * 4 + turn % 4, olddx, olddy);
					oldturn = turn;
				}
				break;
			case DOWN:
				if (IsAvailable(shape * 4 + (turn + 1) % 4, dx+1, dy+1))
				{
					dy += 1;		//由于IsAvailable只检测下一个，所以会把下一行吃掉！！！！！！！！！！！！！,那么如果我让它检测下一个（形参+1）= 完美~
					//delay = max_delay;		按键时增加延迟就不动了！！！！！！！！！
				}
				break;
			}
		}
		else if (delay*count > max_delay)//没有键盘输入，块自动下落
		{
			delay = delay - level;
			max_delay = delay * 10;
			if (IsAvailable(shape * 4 + turn % 4, dx, dy + 1))
			{
				dy++;//能下落的话就下落，dy加1
				count = 0;
			}
			else
			{
				//Erase(oldshape*4+turn%4,olddx,olddy);
				for (int i = 0; i<4; i++)
				for (int j = 0; j<4; j++)
				{
					if (data[shape * 4 + turn % 4][i][j] != 0)
					{
						map[dy + i][dx + j + MAPX / 2 - 2] = data[shape * 4 + turn % 4][i][j];//不能下落的话，把下落的方块（多维数组）赋值给工作区。
						if (dy <= 1)
							GameOver();
					}
				}

				shape = rand() % 7;//更新shape (下落一个新的随机方块)。
				turn = 0;
				dy = 0;
				dx = 0;
				olddx = dx;
				olddy = dy;

			}
		}
		if (!IsAvailable(shape * 4 + turn % 4, 0, 0))//刷新上边界
		{
			if (map[0][4] = -1 && (data[0][0] != 0))
			for (int j = MAPX / 2 - 2; j<MAPX / 2 + 4; j++)
				textout(handle, STARTX + j * 2, STARTY, wColors + 4, 1, "═");
		}

		if ((olddx != dx) || (olddy != dy))
		{
			Erase(oldshape * 4 + oldturn % 4, olddx, olddy);
			Display(shape * 4 + turn % 4, dx, dy);

			oldshape = shape;
			olddx = dx;
			olddy = dy;
			oldturn = turn;
		}

		//消行
		for (int i = 1; i<MAPY - 1; i++)
		{
			int full = 1;
			for (int j = 1; j<MAPX - 1; j++)
			{

				if (map[i][j] == 0)
					full = 0;

			}
			if (full == 1)
			{
				DeleteLine(map, i);
				score = score + 10;
				char string[10];//用好debug,人生赢家~
				_itoa_s(score, string, 10);
				textout(handle, STARTX * 3, STARTY * 2 + 2, wColors, 1, string);
				level = min (score/30,9);
				_itoa_s(level, string, 5);
				textout(handle, STARTX * 3, STARTY * 2 + 6, wColors, 1, string);
				max_delay = 100 - 10 * level;
				Update(i);//更新第i行工作区
			}
		}
		Sleep(delay);
		count++;
		
	}
}