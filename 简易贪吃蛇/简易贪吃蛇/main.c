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