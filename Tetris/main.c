#include "console.h"

#include <stdio.h>
#include <time.h>

typedef struct t_pos {
	int x, y;
}pair_pos;

typedef struct t_block {
	int type;
	int rotate;
	int near_block[4];
	pair_pos pos;

}block;

void print_board(void);
void print_blank(int x, int y);
void rotate_tetrimino(block* p_tetrimino);
void set_tetrimino_value(block* p_tetrimino);
void make_new_tetrimino(block* p_tetrimino);
void print_tetrimino(block* p_tetrimino);
void check(block* p_tetrimino, int x, int y);
void delete_tetrimino_board(block* p_tetrimino);
void add_tetrimino_board(block* p_tetrimino);

/*
0x20 �����̽�
0x25 ����
0x26 ����
0x27 ������
0x28 �Ʒ���
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

const char blank = { ' ' };
int board[22][21] = { 0, };
int visit[20][20] = { 0, };

int main()
{
	block tetrimino;
	block* p_tetrimino = &tetrimino;
	//int board[22][32] = { 0, };
	//int visit[20][30] = { 0, };
	double time_interval_update = 0.6;
	double time_start;
	double time_now;

	int i;
	for (i = 0; i < 22; i++)
		board[i][20] = 1;

	for (i = 0; i < 21; i++) {
		board[0][i] = 1;
		board[21][i] = 1;
	}


	Start_Console();
	Stopwatch_Start();
	time_start = Stopwatch_Get();
	time_now = Stopwatch_Get();

	print_board();
	set_tetrimino_value(p_tetrimino);
	print_tetrimino(p_tetrimino);
	add_tetrimino_board(p_tetrimino, board);


	int  j;
	/*
	  for (i = 0; i < 22; i++) {
		  for (j = 0; j < 21; j++) {
			  Move_Cursor(i, j);
			  printf("%d ", board[i][j]);
		  }
		  printf("\n");
	  }

  //	*/
	while (1)
	{

		Accept_Inputs();

		memset(visit, 0, sizeof(visit));
		check(p_tetrimino, tetrimino.pos.x + 2, tetrimino.pos.y + 2);

		delete_tetrimino_board(p_tetrimino);

		while (Can_Read())
		{
			struct Info_Input info;

			Read_Input(&info);



			if (info.type & TypeCode_Key)
			{

				//����Ű
				if (info.type == TypeCode_Key_Press && info.code == 0x26) {
					rotate_tetrimino(p_tetrimino);
				}

				//����Ű
				if (info.type == TypeCode_Key_Press && info.code == 0x25) {
					if (p_tetrimino->near_block[2] == 0)
						p_tetrimino->pos.x -= 1;
					p_tetrimino->near_block[0] = 0;
				}

				//������Ű
				if (info.type == TypeCode_Key_Press && info.code == 0x27) {
					if (p_tetrimino->near_block[0] == 0)
						p_tetrimino->pos.x += 1;
					else p_tetrimino->near_block[2] = 0;
				}
			}
			else
			{
				struct Point pos;

				pos = Get_Mouse_Pos();

			}
			memset(visit, 0, sizeof(visit));
			check(p_tetrimino, tetrimino.pos.x + 2, tetrimino.pos.y + 2);
		}

		add_tetrimino_board(p_tetrimino, board);




		//�ð������� ����� ������
		if (Stopwatch_Get() - time_now > time_interval_update) {
			print_blank(tetrimino.pos.x, tetrimino.pos.y);
			if (p_tetrimino->near_block[1] == 0)
				p_tetrimino->pos.y += 1;
			time_now = Stopwatch_Get();
			print_tetrimino(p_tetrimino);






			for (i = 0; i < 22; i++) {
				for (j = 0; j < 21; j++) {
					Move_Cursor(2 * i + 21, j);
					printf("%d ", board[i][j]);
				}
			}
			// */
			Move_Cursor(30, 25);
			printf("���� %d, ������ %d, ", p_tetrimino->near_block[2], p_tetrimino->near_block[0]);
			printf("�Ʒ��� %d", p_tetrimino->near_block[1]);
		}

		//����� �ٴڿ� ���̸� ���ο� �� ����
//		if (p_tetrimino->near_block[1] == 1)
//			make_new_tetrimino(p_tetrimino);

		//��� �� ������ �����
		print_blank(tetrimino.pos.x, tetrimino.pos.y);
		print_tetrimino(p_tetrimino);
		Move_Cursor(0, 0);
		print_board();
		Stopwatch_SpinLock(1.0 / 240);
	}

	return 0;
}

void delete_tetrimino_board(block* p_tetrimino) {
	int x = p_tetrimino->pos.x;
	int y = p_tetrimino->pos.y;
	int type = p_tetrimino->type;
	int rotate = p_tetrimino->rotate;

	int i, j;
	for (i = 0; i < 5; i++) {
		for (j = 0; j < 5; j++) {
			board[x + i][y + j] = 0;
		}
	}

	for (i = 0; i < 22; i++)
		board[i][20] = 1;

	for (i = 0; i < 21; i++) {
		board[0][i] = 1;
		board[21][i] = 1;
	}
}

void add_tetrimino_board(block* p_tetrimino) {
	int x = p_tetrimino->pos.x;
	int y = p_tetrimino->pos.y;
	int type = p_tetrimino->type;
	int rotate = p_tetrimino->rotate;

	int i, j;

	for (i = 0; i < 5; i++) {
		for (j = 0; j < 5; j++) {
			if (tetriminos[type][rotate][i][j] == 1)
				board[x + j][y + i] = 10;
		}
	}
}

void make_new_tetrimino(block* p_tetrimino) {
	int x = p_tetrimino->pos.x;
	int y = p_tetrimino->pos.y;
	int type = p_tetrimino->type;
	int rotate = p_tetrimino->rotate;

	int i, j;
	for (i = 0; i < 5; i++) {
		for (j = 0; j < 5; j++) {
			if (tetriminos[type][rotate][i][j] == 1)
				board[x + j][y + i] = 1;
		}
	}
	set_tetrimino_value(p_tetrimino);
}

//�̵��Ϸ�� tetrimino�� ������ �����ϴ� �Լ�
void set_tetrimino_value(block* p_tetrimino) {
	int t_type = rand() % 7;
	p_tetrimino->type = t_type;
	p_tetrimino->rotate = 0;
	p_tetrimino->pos.x = 13;
	p_tetrimino->pos.y = 1;
	int i;
	for (i = 0; i < 4; i++) {
		p_tetrimino->near_block[i] = 0;
	}
}

//tetrimino�� ����ϴ� �Լ�
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

//tetrimino�� �ð�������� 90�� ȸ���ϴ� �Լ�
void rotate_tetrimino(block* p_tetrimino) {
	int temp = p_tetrimino->rotate;
	temp = (temp + 1) % 4;
	p_tetrimino->rotate = temp;
}

//�⺻���� �������� ����� �Լ�
void print_board(void) {
	int i;
	for (i = 0; i < 20; i++) {
		Move_Cursor(0, i);
		printf("|");
		Move_Cursor(21, i);
		printf("|");
	}

	for (i = 0; i < 22; i++) {
		Move_Cursor(i, 20);
		printf("-");
	}

}

//���� tetrimino�� ����� ����� ���� ������ ����ϴ� �Լ�
void print_blank(int x, int y) {
	int i, j;
	Move_Cursor(x, y);
	for (i = -5; i < 5; i++) {
		for (j = -5; j < 5; j++) {
			Move_Cursor(x + i, y + j);
			printf("%c", blank);
		}
	}
}

//�������� tetrimino ������ �ٸ� ��ֹ��� �ִ��� �Ǻ��ϴ� �Լ�
void check(block* p_tetrimino, int x, int y) {
	int dx[4] = { 1, 0, -1, 0 };
	int dy[4] = { 0, 1, 0, -1 };
	int condition;
	visit[x][y] = 1;

	int i;
	for (i = 0; i < 4; i++) {
		condition = (x < 1 || y > 19 || x > 20 || visit[x + dx[i]][y + dy[i]]);
		if (condition == 0 && board[x + dx[i]][y + dy[i]] == 1)
			p_tetrimino->near_block[i] = 1;
		if (condition == 0 && board[x + dx[i]][y + dy[i]] == 10)
			check(p_tetrimino, x + dx[i], y + dy[i]);

	}
}