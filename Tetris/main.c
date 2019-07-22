#include "console.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

typedef struct t_pos {
	int x, y;
}pair_pos;

typedef struct t_block {
	int type;
	int rotate;
	int near_block[4];
	int pre_rotate;
	pair_pos pos;
	pair_pos pre_pos;

}block;

void print_board(void);
void down_board(block* p_tetrimino, int line);
void print_blank(block* p_tetrimino);
void rotate_tetrimino(block* p_tetrimino);
void set_tetrimino_value(block* p_tetrimino);
void make_new_tetrimino(block* p_tetrimino);
void print_tetrimino(block* p_tetrimino);
void previous_tetrimino_print(void);
int check(block* p_tetrimino, int rotate, int x, int y);
void delete_tetrimino_board(block* p_tetrimino);
void add_tetrimino_board(block* p_tetrimino);
int next_stage_check(block* p_tetrimino, int* score);

/*
0x20 �����̽�
0x25 ����
0x26 ����
0x27 ������
0x28 �Ʒ���
*/

const int tetriminos[7][4][5][5] =
{ {   {   {   0,   0,   0,   0,   0,   },
	  {   0,   0,   0,   0,   0,   },
	  {   0,   0,   1,   1,   0,   },
	  {   0,   0,   1,   1,   0,   },
	  {   0,   0,   0,   0,   0,   }   },

	{   {   0,   0,   0,   0,   0,   },
	  {   0,   0,   0,   0,   0,   },
	  {   0,   0,   1,   1,   0,   },
	  {   0,   0,   1,   1,   0,   },
	  {   0,   0,   0,   0,   0,   }   },

	{   {   0,   0,   0,   0,   0,   },
	  {   0,   0,   0,   0,   0,   },
	  {   0,   0,   1,   1,   0,   },
	  {   0,   0,   1,   1,   0,   },
	  {   0,   0,   0,   0,   0,   }   },

	{   {   0,   0,   0,   0,   0,   },
	  {   0,   0,   0,   0,   0,   },
	  {   0,   0,   1,   1,   0,   },
	  {   0,   0,   1,   1,   0,   },
	  {   0,   0,   0,   0,   0,   }   }   },

   {   {   {   0,   0,   0,   0,   0,   },
	  {   0,   0,   0,   0,   0,   },
	  {   1,   1,   1,   1,   0,   },
	  {   0,   0,   0,   0,   0,   },
	  {   0,   0,   0,   0,   0,   }   },

	{   {   0,   0,   1,   0,   0,   },
	  {   0,   0,   1,   0,   0,   },
	  {   0,   0,   1,   0,   0,   },
	  {   0,   0,   1,   0,   0,   },
	  {   0,   0,   0,   0,   0,   }   },

	{   {   0,   0,   0,   0,   0,   },
	  {   0,   0,   0,   0,   0,   },
	  {   0,   1,   1,   1,   1,   },
	  {   0,   0,   0,   0,   0,   },
	  {   0,   0,   0,   0,   0,   }   },

	{   {   0,   0,   0,   0,   0,   },
	  {   0,   0,   1,   0,   0,   },
	  {   0,   0,   1,   0,   0,   },
	  {   0,   0,   1,   0,   0,   },
	  {   0,   0,   1,   0,   0,   }   }   },

   {   {   {   0,   0,   0,   0,   0,   },
	  {   0,   1,   0,   0,   0,   },
	  {   0,   1,   1,   1,   0,   },
	  {   0,   0,   0,   0,   0,   },
	  {   0,   0,   0,   0,   0,   }   },

	{   {   0,   0,   0,   0,   0,   },
	  {   0,   0,   1,   1,   0,   },
	  {   0,   0,   1,   0,   0,   },
	  {   0,   0,   1,   0,   0,   },
	  {   0,   0,   0,   0,   0,   }   },

	{   {   0,   0,   0,   0,   0,   },
	  {   0,   0,   0,   0,   0,   },
	  {   0,   1,   1,   1,   0,   },
	  {   0,   0,   0,   1,   0,   },
	  {   0,   0,   0,   0,   0,   }   },

	{   {   0,   0,   0,   0,   0,   },
	  {   0,   0,   1,   0,   0,   },
	  {   0,   0,   1,   0,   0,   },
	  {   0,   1,   1,   0,   0,   },
	  {   0,   0,   0,   0,   0,   }   }   },

   {   {   {   0,   0,   0,   0,   0,   },
	  {   0,   0,   0,   0,   0,   },
	  {   0,   1,   1,   1,   0,   },
	  {   0,   1,   0,   0,   0,   },
	  {   0,   0,   0,   0,   0,   }   },

	{   {   0,   0,   0,   0,   0,   },
	  {   0,   1,   1,   0,   0,   },
	  {   0,   0,   1,   0,   0,   },
	  {   0,   0,   1,   0,   0,   },
	  {   0,   0,   0,   0,   0,   }   },

	{   {   0,   0,   0,   0,   0,   },
	  {   0,   0,   0,   1,   0,   },
	  {   0,   1,   1,   1,   0,   },
	  {   0,   0,   0,   0,   0,   },
	  {   0,   0,   0,   0,   0,   }   },

	{   {   0,   0,   0,   0,   0,   },
	  {   0,   0,   1,   0,   0,   },
	  {   0,   0,   1,   0,   0,   },
	  {   0,   0,   1,   1,   0,   },
	  {   0,   0,   0,   0,   0,   }   }   },

   {   {   {   0,   0,   0,   0,   0,   },
	  {   0,   0,   0,   0,   0,   },
	  {   0,   0,   1,   1,   0,   },
	  {   0,   1,   1,   0,   0,   },
	  {   0,   0,   0,   0,   0,   }   },

	{   {   0,   0,   0,   0,   0,   },
	  {   0,   0,   1,   0,   0,   },
	  {   0,   0,   1,   1,   0,   },
	  {   0,   0,   0,   1,   0,   },
	  {   0,   0,   0,   0,   0,   }   },

	{   {   0,   0,   0,   0,   0,   },
	  {   0,   0,   0,   0,   0,   },
	  {   0,   0,   1,   1,   0,   },
	  {   0,   1,   1,   0,   0,   },
	  {   0,   0,   0,   0,   0,   }   },

	{   {   0,   0,   0,   0,   0,   },
	  {   0,   0,   1,   0,   0,   },
	  {   0,   0,   1,   1,   0,   },
	  {   0,   0,   0,   1,   0,   },
	  {   0,   0,   0,   0,   0,   }   }   },

   {   {   {   0,   0,   0,   0,   0,   },
	  {   0,   0,   1,   0,   0,   },
	  {   0,   1,   1,   0,   0,   },
	  {   0,   1,   0,   0,   0,   },
	  {   0,   0,   0,   0,   0,   }   },

	{   {   0,   0,   0,   0,   0,   },
	  {   0,   1,   1,   0,   0,   },
	  {   0,   0,   1,   1,   0,   },
	  {   0,   0,   0,   0,   0,   },
	  {   0,   0,   0,   0,   0,   }   },

	{   {   0,   0,   0,   0,   0,   },
	  {   0,   0,   1,   0,   0,   },
	  {   0,   1,   1,   0,   0,   },
	  {   0,   1,   0,   0,   0,   },
	  {   0,   0,   0,   0,   0,   }   },

	{   {   0,   0,   0,   0,   0,   },
	  {   0,   1,   1,   0,   0,   },
	  {   0,   0,   1,   1,   0,   },
	  {   0,   0,   0,   0,   0,   },
	  {   0,   0,   0,   0,   0,   }   }   },

   {   {   {   0,   0,   0,   0,   0,   },
	  {   0,   0,   0,   0,   0,   },
	  {   0,   0,   1,   0,   0,   },
	  {   0,   1,   1,   1,   0,   },
	  {   0,   0,   0,   0,   0,   }   },

	{   {   0,   0,   0,   0,   0,   },
	  {   0,   1,   0,   0,   0,   },
	  {   0,   1,   1,   0,   0,   },
	  {   0,   1,   0,   0,   0,   },
	  {   0,   0,   0,   0,   0,   }   },

	{   {   0,   0,   0,   0,   0,   },
	  {   0,   1,   1,   1,   0,   },
	  {   0,   0,   1,   0,   0,   },
	  {   0,   0,   0,   0,   0,   },
	  {   0,   0,   0,   0,   0,   }   },

	{   {   0,   0,   0,   0,   0,   },
	  {   0,   0,   0,   1,   0,   },
	  {   0,   0,   1,   1,   0,   },
	  {   0,   0,   0,   1,   0,   },
	  {   0,   0,   0,   0,   0,   }   }   } };

const char blank = { ' ' };
int board[22][21] = { 0, };

int main()
{
	srand(time(NULL));
	block tetrimino;
	block* p_tetrimino = &tetrimino;
	int line_cnt = 0;
	int immeadiatly_flag = 0;
	int fast_flag = 0;
	int score = 0;
	int level = 1;
	double time_interval_update = 0.6;
	double time_interval = 0.6;
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
	add_tetrimino_board(p_tetrimino);


	int  j;

	while (1)
	{
		int go_down = 1;
		fast_flag = 0;
		immeadiatly_flag = 0;
		Accept_Inputs();

		if (p_tetrimino->near_block[1] == 1)
			immeadiatly_flag = 0;

		time_interval_update = time_interval;
		print_blank(p_tetrimino);

		p_tetrimino->pre_pos.x = p_tetrimino->pos.x;
		p_tetrimino->pre_pos.y = p_tetrimino->pos.y;
		p_tetrimino->pre_rotate = p_tetrimino->rotate;

		while (Can_Read())
		{
			struct Info_Input info;

			Read_Input(&info);



			if (info.type & TypeCode_Key)
			{
				//�����̽���
				if (info.type == TypeCode_Key_Press && info.code == 0x20) {
					delete_tetrimino_board(p_tetrimino);
					while ((go_down = check(p_tetrimino, p_tetrimino->rotate, p_tetrimino->pos.x, p_tetrimino->pos.y + 1)) != 0) {
						p_tetrimino->pos.y += 1;
					}
				}

				//����Ű
				if (info.type == TypeCode_Key_Press && info.code == 0x26) {
					if (check(p_tetrimino, p_tetrimino->rotate + 1, p_tetrimino->rotate, p_tetrimino->pos.x) != 0)
						rotate_tetrimino(p_tetrimino);
				}

				//�Ʒ�Ű
				if (info.type == TypeCode_Key_Press && info.code == 0x28) {
					fast_flag = 1;
				}

				//����Ű
				if (info.type == TypeCode_Key_Press && info.code == 0x25) {
					if (check(p_tetrimino, p_tetrimino->rotate, p_tetrimino->pos.x - 1, p_tetrimino->pos.y) != 0)
						p_tetrimino->pos.x -= 1;
				}

				//������Ű
				if (info.type == TypeCode_Key_Press && info.code == 0x27) {
					if (check(p_tetrimino, p_tetrimino->rotate, p_tetrimino->pos.x + 1, p_tetrimino->pos.y) != 0)
						p_tetrimino->pos.x += 1;
				}
			}
			else
			{
				struct Point pos;

				pos = Get_Mouse_Pos();

			}
		}


		//�ð������� ����� ������
		if (fast_flag == 1)
			time_interval_update = 0.1;

		if (immeadiatly_flag == 1)
			time_interval_update = 0.01;

		if (Stopwatch_Get() - time_now > time_interval_update) {
			if ((go_down = check(p_tetrimino, p_tetrimino->rotate, p_tetrimino->pos.x, p_tetrimino->pos.y + 1)) != 0)
				p_tetrimino->pos.y += 1;
			time_now = Stopwatch_Get();

		}

		//����� �ٴڿ� ���̸� ���ο� �� ����
		if (go_down == 0) {
			make_new_tetrimino(p_tetrimino);
			time_interval_update = time_interval;
		}

		delete_tetrimino_board(p_tetrimino);
		add_tetrimino_board(p_tetrimino);
		line_cnt += next_stage_check(p_tetrimino, &score);

		//line_cnt�� Ȯ���ؼ� ���� ����
		if (line_cnt == 2) {
			line_cnt = 0;
			level++;
			time_interval = time_interval / 2;
			time_interval_update = time_interval;
		}

		//��� �� ������ �����
		Move_Cursor(30, 6);
		printf("cleard line : %d", line_cnt);
		Move_Cursor(30, 4);
		printf("score : %d", score);
		Move_Cursor(30, 2);
		printf("Level : %d", level);
		print_tetrimino(p_tetrimino);
		Move_Cursor(0, 0);
		print_board();
		previous_tetrimino_print();
		Stopwatch_SpinLock(1.0 / 240);
	}

	return 0;
}

//tetrimino�� ���� ������ ����� �Լ�
void delete_tetrimino_board(block * p_tetrimino) {
	int x = p_tetrimino->pre_pos.x;
	int y = p_tetrimino->pre_pos.y;
	int type = p_tetrimino->type;
	int rotate = p_tetrimino->pre_rotate;

	int i, j;
	for (i = 0; i < 5; i++) {
		for (j = 0; j < 5; j++) {
			if (tetriminos[type][rotate][i][j] == 1)
				board[x + j][y + i] = 0;
		}
	}

	for (i = 0; i < 22; i++)
		board[i][20] = 1;

	for (i = 0; i < 21; i++) {
		board[0][i] = 1;
		board[21][i] = 1;
	}
}

//tetrimino�� ������ board�� �Է��ϴ� �Լ�
void add_tetrimino_board(block * p_tetrimino) {
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

//���ο� tetrimino�� ����� �Լ�
void make_new_tetrimino(block * p_tetrimino) {
	int x = p_tetrimino->pos.x;
	int y = p_tetrimino->pos.y;
	int type = p_tetrimino->type;
	int rotate = p_tetrimino->rotate;

	int i, j;
	for (i = 0; i < 5; i++) {
		for (j = 0; j < 5; j++) {
			if (tetriminos[type][rotate][i][j] == 1)
				board[x + j][y + i] = 2;
		}
	}
	set_tetrimino_value(p_tetrimino);
}

//tetrimino�� ������ �����ϴ� �Լ�
void set_tetrimino_value(block * p_tetrimino) {
	int t_type = rand() % 7;
	p_tetrimino->type = t_type;
	p_tetrimino->rotate = 0;
	p_tetrimino->pos.x = 13;
	p_tetrimino->pos.y = 1;
	p_tetrimino->pre_pos.x = 1;
	p_tetrimino->pre_pos.y = 1;
	p_tetrimino->pre_rotate = 0;
	int i;
	for (i = 0; i < 4; i++) {
		p_tetrimino->near_block[i] = 0;
	}
}

//tetrimino�� ����ϴ� �Լ�
void print_tetrimino(block * p_tetrimino) {
	int i, j;
	int x = p_tetrimino->pos.x;
	int y = p_tetrimino->pos.y;
	int type = p_tetrimino->type;
	int rotate = p_tetrimino->rotate;
	for (i = 0; i < 5; i++) {
		for (j = 0; j < 5; j++) {
			if (tetriminos[type][rotate][i][j] == 1) {
				Move_Cursor(x + j, y + i);
				printf("@");
			}
		}
	}
}

//������ �̵��Ϸ��� tetrimino�� ����ϴ� �Լ�
void previous_tetrimino_print(void) {
	int i, j;
	for (i = 0; i < 22; i++) {
		for (j = 0; j < 21; j++) {
			if (board[i][j] == 2) {
				Move_Cursor(i, j);
				printf("@");
			}
		}
	}
}

//tetrimino�� �ð�������� 90�� ȸ���ϴ� �Լ�
void rotate_tetrimino(block * p_tetrimino) {
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
void print_blank(block * p_tetrimino) {
	int i, j;
	int x = p_tetrimino->pos.x;
	int y = p_tetrimino->pos.y;
	int type = p_tetrimino->type;
	int rotate = p_tetrimino->rotate;

	for (i = 0; i < 5; i++) {
		for (j = 0; j < 5; j++) {
			if (tetriminos[type][rotate][i][j] == 1) {
				Move_Cursor(x + j, y + i);
				printf("%c", blank);
			}
		}
	}
}

//�������� tetrimino ������ �ٸ� ��ֹ��� �ִ��� �Ǻ��ϴ� �Լ�
int check(block * p_tetrimino, int rotate, int x, int y) {
	x += 2;
	y += 2;

	for (int dX = -2; dX <= 2; ++dX)
	{
		for (int dY = -2; dY <= 2; ++dY)
		{
			if ((x + dX >= 0) && (x + dX < 22) && (y + dY < 20))
			{
				if ((board[x + dX][y + dY] == 1 || board[x + dX][y + dY] == 2) &&
					tetriminos[p_tetrimino->type][rotate][2 + dY][2 + dX] == 1)
					return 0;
			}

			else if (tetriminos[p_tetrimino->type][rotate][2 + dY][2 + dX] != 0)
			{
				return 0;
			}
		}
	}

	return 1;
}

//������ �� ä������ �� ���� ���� ȭ���� �����ϴ� �Լ�
int next_stage_check(block * p_tetrimino, int* score) {
	int i, j;
	int next_stage_count = 0;
	int cnt = 0;
	for (i = 19; i > 0; i--) {
		cnt = 0;
		for (j = 1; j < 21; j++) {
			if (board[j][i] == 2)
				cnt++;
		}
		if (cnt == 20) {
			next_stage_count++;
			*score += 100 * next_stage_count;
			down_board(p_tetrimino, i);
			system("cls");
		}
	}

	return next_stage_count;
}

//�� ���� �� ä���� �� �� �� ���� �ϳ� ������ �Լ�
void down_board(block * p_tetrimino, int line) {
	int i, j;
	for (i = line; i > 1; i--) {
		for (j = 1; j < 21; j++) {
			board[j][i] = board[j][i - 1];
		}
	}

	for (i = 1; i < 21; i++) {
		board[i][0] = 0;
	}
}