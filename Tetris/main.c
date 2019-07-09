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
block* make_new_tetrimino(int**** tetri_set, block* p_tetrimino);
void print_tetrimino(block* p_tetrimino);
// 프로그램 전체에서 사용할 각종 이름들을 선언해 둘 만한 부분

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

int main()
{
	block tetrimino;
	block* p_tetrimino;
	int board[20][30];
	int time_interval_update = 0.5;
	int time_start;
	
	
	Start_Console();
	Stopwatch_Start();
	time_start = Stopwatch_Get();

	print_board();
	make_new_tetrimino(&tetriminos, &p_tetrimino);
	print_tetrimino(&p_tetrimino);

	while ( 1 )
	{
		// 사용자 입력을 처리하는 부분
		Accept_Inputs();
		
		while ( Can_Read() )
		{
			struct Info_Input info;

			Read_Input(&info);

			if ( info.type & TypeCode_Key )
			{

				if (info.type == TypeCode_Key_Press && info.code == 0x25);
						
						
			}
			else
			{
				struct Point pos;

				pos = Get_Mouse_Pos();

			}
		}



		




		
		Stopwatch_SpinLock(1.0 / 240);
	}

	return 0;
}

block* make_new_tetrimino(int**** tetri_set, block* p_tetrimino) {
	int t_type = rand() % 7;
	p_tetrimino->type = t_type;
	p_tetrimino->rotate = 0;
	p_tetrimino->pos.x = 15;
	p_tetrimino->pos.y = 1;
	return p_tetrimino;
}

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