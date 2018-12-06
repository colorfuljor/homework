---
layout: default
title: 智能蛇实验报告
---
# 智能蛇实验报告

## 一、实验目的
1. 了解 算法 与 “智能” 的关系
2. 通过算法赋予蛇智能
3. 了解 Linux IO 设计的控制

## 二、游戏设计
### 1、玩法

贪吃蛇游戏是一款经典的益智游戏，有PC和手机等多平台版本。既简单又耐玩。该游戏通过控制蛇头方向吃蛋，从而使得蛇变得越来越长。

### 2、游戏表示

给定一个10*10的字符矩阵表示蛇的生存空间,其中有一条长度5的蛇(HXXXX), “H”表示蛇头,“X”表示蛇身体。空间中可能有食物（“$”表示）和障碍物（“*”表示）

你可以使用“ADWS”按键分别控制蛇的前进方向“左右上下”, 当蛇头碰到自己的身体或走出边界,游戏结束,否则蛇按你指定方向前进一步。

### 3、创新玩法
我希望蛇能一秒动一次，键盘键入只是改变蛇的方向，因此我需要非阻塞的输入。

## 三、算法
### 以下为代码框架
```
输出字符矩阵
	WHILE not 游戏结束 DO
		ch＝等待输入
		CASE ch DO
		‘A’:左前进一步，break 
		‘D’:右前进一步，break    
		‘W’:上前进一步，break    
		‘S’:下前进一步，break    
		END CASE
		输出字符矩阵
	END WHILE
	输出 Game Over!!!
```
### 数据结构使用
首先我利用两个一维分别数组存放蛇的坐标x，y（在数组中的下标），用一个二维数组存放地图（用以打印）。
### 算法
需要设计这些函数：
1. 蛇根据键盘进行移动
2. 随机投放实物
3. 蛇吃食物增长
4. 判断蛇是否碰撞
5. 游戏结束

为了实现非阻塞的输入，我利用了knit()函数，而实现蛇一秒走一次，我利用了Windows.h中的Sleep()函数（这是为C++拓展的多线程准备的）。


### 四、代码
```
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <windows.h>

#define SNAKE_MAX_LENGTH 20
#define SNAKE_HEAD 'H'
#define SNAKE_BODY 'X'
#define BLANK_CELL ' '
#define SNAKE_FOOD '$'
#define WALL_CELL '*'
//#define SPEED 500

//snake stepping: dy = -1(up),1(down); dx = -1(left), 1(right), 0(no move)
void snakeMove();
//put a food rendomized on a blank cell
void put_money(void);
//out cells of the grid
void output(void);
//outs when gameover
void gameover(void);
//ÅöÇ½ÅÐ¶Ï
int isCrash();
int Eat();

char map[12][12] = {
	"************",
	"*XXXXH     *",
	"*          *",
	"*          *",
	"*          *",
	"*          *",
	"*          *",
	"*          *",
	"*          *",
	"*          *",
	"*          *",
	"************",
};

//define vars for snake, notice name of vars in C
int snakeX[SNAKE_MAX_LENGTH] = { 5, 4, 3, 2, 1 };
int snakeY[SNAKE_MAX_LENGTH] = { 1, 1, 1, 1, 1 };
int dx = 1;
int dy = 0;
int snakeLength = 5;
int food_x;
int food_y;
int speed_time = 500;

int main() {
	output();
	put_money();
	char ch;
	while (!isCrash() && snakeLength < SNAKE_MAX_LENGTH) {
		if (_kbhit()) {
			ch = _getch();
			switch (ch) {
			case 'A':
			case 'a':
				if (dx != 1) {
					dx = -1;
					dy = 0;
				}
				break;
			case 'D':
			case 'd':
				if (dx != -1) {
					dx = 1;
					dy = 0;
				}
				break;
			case 'W':
			case 'w':
				if (dy != 1) {
					dx = 0;
					dy = -1;
				}				
				break;
			case 'S':
			case 's':
				if (dy != -1) {
					dx = 0;
					dy = 1;
				}
				break;
			}
			//snakeMove();
		}
			snakeMove();
			Eat();
			output();
			Sleep(speed_time);
	}
	gameover();

	system("pause");
	return 0;
}

void output() {
	system("cls");
	for (int i = 0; i <= 11; i++) {
		for (int j = 0; j <= 11; j++) {
			printf("%c", map[i][j]);
		}
		printf("\n");
	}
}

void snakeMove() {
	snakeX[0] += dx;
	snakeY[0] += dy;
	if (isCrash()) return;
	map[snakeY[0]][snakeX[0]] = SNAKE_HEAD;
	map[snakeY[snakeLength - 1]][snakeX[snakeLength - 1]] = ' ';
	for (int i = snakeLength - 1; i > 0; i--) {
		snakeX[i] = snakeX[i - 1];
		snakeY[i] = snakeY[i - 1];
		if (i == 1) {
			snakeX[i] -= dx;
			snakeY[i] -= dy;
		}
		map[snakeY[i]][snakeX[i]] = SNAKE_BODY;
	}
}

void put_money(void) {
	srand((unsigned)(time(NULL)));
	food_x = rand() % 9 + 1;
	food_y = rand() % 9 + 1;
	while (map[food_y][food_x] != ' ') {
		food_x = rand() % 9 + 1;
		food_y = rand() % 9 + 1;
	}
	map[food_y][food_x] = '$';
}

int Eat() {
	if (snakeX[0] == food_x && snakeY[0] == food_y) {
		snakeX[snakeLength] = snakeX[snakeLength - 1] - snakeX[snakeLength - 2] + snakeX[snakeLength - 1];
		snakeY[snakeLength] = snakeY[snakeLength - 1] - snakeY[snakeLength - 2] + snakeY[snakeLength - 1];
		map[snakeY[snakeLength]][snakeX[snakeLength]] = SNAKE_BODY;
		snakeLength++;
		/*speed_time -= 50;*/
		put_money();
		return 1;
	}
	return 0;
}

int isCrash() {
	if (map[snakeY[0]][snakeX[0]] == '*' || map[snakeY[0]][snakeX[0]] == 'X') return 1;
	
	return 0;
}

void gameover() {
	if (snakeLength == SNAKE_MAX_LENGTH) {
		printf("You win!");
	}
	else {
		printf("Crash!");
	}
}
```