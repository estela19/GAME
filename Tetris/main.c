#include "console.h"

#include <stdio.h>
#include <time.h>

typedef struct t_pos {
	int x, y;
}pair_pos;

typedef struct t_block {
	int type;
	int rotate;
	pair_pos pos;

}block;

void print_board(void);
void print_blank(int x, int y);
void rotate_tetrimino(block* p_tetrimino);
void make_new_tetrimino(block* p_tetrimino);
void print_tetrimino(block* p_tetrimino);


/*
0x20 스페이스
0x25 왼쪽
0x26 위쪽
0x27 오른쪽
0x28 아래쪽
*/

const int tetriminos[7][4][5][5] =
{ {	{	{	0,	0,	0,	0,	0,	},
			{	0,	0,	0,	0,	0,	},
			{	0,	0,	1,	1,	0,	},
			{	0,	0,	1,	1,	0,	},
			{	0,	0,	0,	0,	0,	}	},

		{	{	0,	0,	0,	0,	0,	},
			{	0,	0,	0,	0,	0,	},
			{	0,	0,	1,	1,	0,	},
			{	0,	0,	1,	1,	0,	},
			{	0,	0,	0,	0,	0,	}	},

		{	{	0,	0,	0,	0,	0,	},
			{	0,	0,	0,	0,	0,	},
			{	0,	0,	1,	1,	0,	},
			{	0,	0,	1,	1,	0,	},
			{	0,	0,	0,	0,	0,	}	},

		{	{	0,	0,	0,	0,	0,	},
			{	0,	0,	0,	0,	0,	},
			{	0,	0,	1,	1,	0,	},
			{	0,	0,	1,	1,	0,	},
			{	0,	0,	0,	0,	0,	}	}	},

	{	{	{	0,	0,	0,	0,	0,	},
			{	0,	0,	0,	0,	0,	},
			{	1,	1,	1,	1,	0,	},
			{	0,	0,	0,	0,	0,	},
			{	0,	0,	0,	0,	0,	}	},

		{	{	0,	0,	1,	0,	0,	},
			{	0,	0,	1,	0,	0,	},
			{	0,	0,	1,	0,	0,	},
			{	0,	0,	1,	0,	0,	},
			{	0,	0,	0,	0,	0,	}	},

		{	{	0,	0,	0,	0,	0,	},
			{	0,	0,	0,	0,	0,	},
			{	0,	1,	1,	1,	1,	},
			{	0,	0,	0,	0,	0,	},
			{	0,	0,	0,	0,	0,	}	},

		{	{	0,	0,	0,	0,	0,	},
			{	0,	0,	1,	0,	0,	},
			{	0,	0,	1,	0,	0,	},
			{	0,	0,	1,	0,	0,	},
			{	0,	0,	1,	0,	0,	}	}	},

	{	{	{	0,	0,	0,	0,	0,	},
			{	0,	1,	0,	0,	0,	},
			{	0,	1,	1,	1,	0,	},
			{	0,	0,	0,	0,	0,	},
			{	0,	0,	0,	0,	0,	}	},

		{	{	0,	0,	0,	0,	0,	},
			{	0,	0,	1,	1,	0,	},
			{	0,	0,	1,	0,	0,	},
			{	0,	0,	1,	0,	0,	},
			{	0,	0,	0,	0,	0,	}	},

		{	{	0,	0,	0,	0,	0,	},
			{	0,	0,	0,	0,	0,	},
			{	0,	1,	1,	1,	0,	},
			{	0,	0,	0,	1,	0,	},
			{	0,	0,	0,	0,	0,	}	},

		{	{	0,	0,	0,	0,	0,	},
			{	0,	0,	1,	0,	0,	},
			{	0,	0,	1,	0,	0,	},
			{	0,	1,	1,	0,	0,	},
			{	0,	0,	0,	0,	0,	}	}	},

	{	{	{	0,	0,	0,	0,	0,	},
			{	0,	0,	0,	0,	0,	},
			{	0,	1,	1,	1,	0,	},
			{	0,	1,	0,	0,	0,	},
			{	0,	0,	0,	0,	0,	}	},

		{	{	0,	0,	0,	0,	0,	},
			{	0,	1,	1,	0,	0,	},
			{	0,	0,	1,	0,	0,	},
			{	0,	0,	1,	0,	0,	},
			{	0,	0,	0,	0,	0,	}	},

		{	{	0,	0,	0,	0,	0,	},
			{	0,	0,	0,	1,	0,	},
			{	0,	1,	1,	1,	0,	},
			{	0,	0,	0,	0,	0,	},
			{	0,	0,	0,	0,	0,	}	},

		{	{	0,	0,	0,	0,	0,	},
			{	0,	0,	1,	0,	0,	},
			{	0,	0,	1,	0,	0,	},
			{	0,	0,	1,	1,	0,	},
			{	0,	0,	0,	0,	0,	}	}	},

	{	{	{	0,	0,	0,	0,	0,	},
			{	0,	0,	0,	0,	0,	},
			{	0,	0,	1,	1,	0,	},
			{	0,	1,	1,	0,	0,	},
			{	0,	0,	0,	0,	0,	}	},

		{	{	0,	0,	0,	0,	0,	},
			{	0,	0,	1,	0,	0,	},
			{	0,	0,	1,	1,	0,	},
			{	0,	0,	0,	1,	0,	},
			{	0,	0,	0,	0,	0,	}	},

		{	{	0,	0,	0,	0,	0,	},
			{	0,	0,	0,	0,	0,	},
			{	0,	0,	1,	1,	0,	},
			{	0,	1,	1,	0,	0,	},
			{	0,	0,	0,	0,	0,	}	},

		{	{	0,	0,	0,	0,	0,	},
			{	0,	0,	1,	0,	0,	},
			{	0,	0,	1,	1,	0,	},
			{	0,	0,	0,	1,	0,	},
			{	0,	0,	0,	0,	0,	}	}	},

	{	{	{	0,	0,	0,	0,	0,	},
			{	0,	0,	1,	0,	0,	},
			{	0,	1,	1,	0,	0,	},
			{	0,	1,	0,	0,	0,	},
			{	0,	0,	0,	0,	0,	}	},

		{	{	0,	0,	0,	0,	0,	},
			{	0,	1,	1,	0,	0,	},
			{	0,	0,	1,	1,	0,	},
			{	0,	0,	0,	0,	0,	},
			{	0,	0,	0,	0,	0,	}	},

		{	{	0,	0,	0,	0,	0,	},
			{	0,	0,	1,	0,	0,	},
			{	0,	1,	1,	0,	0,	},
			{	0,	1,	0,	0,	0,	},
			{	0,	0,	0,	0,	0,	}	},

		{	{	0,	0,	0,	0,	0,	},
			{	0,	1,	1,	0,	0,	},
			{	0,	0,	1,	1,	0,	},
			{	0,	0,	0,	0,	0,	},
			{	0,	0,	0,	0,	0,	}	}	},

	{	{	{	0,	0,	0,	0,	0,	},
			{	0,	0,	0,	0,	0,	},
			{	0,	0,	1,	0,	0,	},
			{	0,	1,	1,	1,	0,	},
			{	0,	0,	0,	0,	0,	}	},

		{	{	0,	0,	0,	0,	0,	},
			{	0,	1,	0,	0,	0,	},
			{	0,	1,	1,	0,	0,	},
			{	0,	1,	0,	0,	0,	},
			{	0,	0,	0,	0,	0,	}	},

		{	{	0,	0,	0,	0,	0,	},
			{	0,	1,	1,	1,	0,	},
			{	0,	0,	1,	0,	0,	},
			{	0,	0,	0,	0,	0,	},
			{	0,	0,	0,	0,	0,	}	},

		{	{	0,	0,	0,	0,	0,	},
			{	0,	0,	0,	1,	0,	},
			{	0,	0,	1,	1,	0,	},
			{	0,	0,	0,	1,	0,	},
			{	0,	0,	0,	0,	0,	}	}	} };

const char blank = {' '};

int main()
{
	block tetrimino;
	block* p_tetrimino = &tetrimino;
	int board[20][30];
	float time_interval_update = 0.6;
	float time_start;
	float time_now;
	
	
	Start_Console();
	Stopwatch_Start();
	time_start = Stopwatch_Get();
	time_now = Stopwatch_Get();

	print_board();
	make_new_tetrimino(p_tetrimino);
	print_tetrimino(p_tetrimino);


	while ( 1 )
	{
		
		Accept_Inputs();
		
		while ( Can_Read() )
		{
			struct Info_Input info;

			Read_Input(&info);

			if ( info.type & TypeCode_Key )
			{
				//위쪽키
				if (info.type == TypeCode_Key_Press && info.code == 0x26) {
					rotate_tetrimino(p_tetrimino);
				}
				
				//왼쪽키
				if (info.type == TypeCode_Key_Press && info.code == 0x25) {
					p_tetrimino->pos.x -= 1;
				}
				
				//오른쪽키
				if (info.type == TypeCode_Key_Press && info.code == 0x27) {
					p_tetrimino->pos.x += 1;
				}
			}
			else
			{
				struct Point pos;

				pos = Get_Mouse_Pos();

			}
		}

		
		if (Stopwatch_Get() - time_now > time_interval_update) {
			print_blank(tetrimino.pos.x, tetrimino.pos.y);
			p_tetrimino->pos.y += 1;
			time_now = Stopwatch_Get();
			print_tetrimino(p_tetrimino);
		}
		



		
		Stopwatch_SpinLock(1.0 / 240);
	}

	return 0;
}

//이동완료된 tetrimino의 정보를 갱신하는 함수
void make_new_tetrimino(block* p_tetrimino) {
	int t_type = rand() % 7;
	p_tetrimino->type = t_type;
	p_tetrimino->rotate = 0;
	p_tetrimino->pos.x = 15;
	p_tetrimino->pos.y = 1;
}

//tetrimino를 출력하는 함수
void print_tetrimino(block* p_tetrimino) {
	int i, j;
	int x = p_tetrimino->pos.x;
	int y = p_tetrimino->pos.y;
	int type = p_tetrimino->type;
	int rotate = p_tetrimino->rotate;
	for (i = 0; i < 5; i++) {
		for (j = 0; j < 5; j++) {
			if (tetriminos[type][rotate][i][j] == 1) {
				Move_Cursor(x + j, y + i);
				printf("@", tetriminos[type][rotate][i][j]);
			}
		}
		printf("\n");
	}
}

//tetrimino를 시계방향으로 90도 회전하는 함수
void rotate_tetrimino(block* p_tetrimino) {
	int temp = p_tetrimino->rotate;
	temp = (temp + 1) % 4;
	p_tetrimino->rotate = temp;
}

//기본적인 게임판을 만드는 함수
void print_board(void) {
	int i;
	for (i = 0; i < 30; i++) {
		printf(" |\n");
	}
	printf("  ");
	for (i = 0; i < 20; i++) {
		printf("ㅡ");
	}
	for (i = 0; i < 30; i++) {
		Move_Cursor(42, i);
		printf("|");
	}
}

//저번 tetrimino의 출력을 지우기 위해 공백을 출력하는 함수
void print_blank(int x, int y) {
	int i, j;
	Move_Cursor(x, y);
	for (i = -5; i < 10; i++) {
		for (j = -5; j < 10; j++) {
			Move_Cursor(x + i, y + j);
			printf("%c", blank);
		}
	}
}