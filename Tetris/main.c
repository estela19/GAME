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
void check(block* p_tetrimino, int x, int y);
void delete_tetrimino_board(block* p_tetrimino);
void add_tetrimino_board(block* p_tetrimino);
int next_stage_check(block* p_tetrimino);

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
int visit[22][21] = { 0, };

int main()
{
	block tetrimino;
	block* p_tetrimino = &tetrimino;
	int line_cnt = 0;
	int immeadiatly_flag = 0;
	int fast_flag = 0;
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
	add_tetrimino_board(p_tetrimino);


	int  j;
	/*
	 for (i = 0; i < 22; i++) {
	   for (j = 0; j < 21; j++) {
		  Move_Cursor(i, j);
		  printf("%d ", board[i][j]);
	   }
	   printf("\n");
	 }

   //   */
	while (1)
	{
		time_interval_update = 0.6;
		fast_flag = 0;
		immeadiatly_flag = 0;
		Accept_Inputs();

		if (p_tetrimino->near_block[1] == 1)
			immeadiatly_flag = 0;

		memset(visit, 0, sizeof(visit));
		check(p_tetrimino, tetrimino.pos.x + 2, tetrimino.pos.y + 2);

		//		delete_tetrimino_board(p_tetrimino);
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
					immeadiatly_flag = 1;
				}

				//����Ű
				if (info.type == TypeCode_Key_Press && info.code == 0x26) {
					// TODO: ȸ�� ���ϴ� ��쵵 ���.
					rotate_tetrimino(p_tetrimino);
				}

				//�Ʒ�Ű
				if (info.type == TypeCode_Key_Press && info.code == 0x28) {
					fast_flag = 1;
				}

				//����Ű
				if (info.type == TypeCode_Key_Press && info.code == 0x25) {
					if (p_tetrimino->near_block[2] == 0)
						p_tetrimino->pos.x -= 1;
				}

				//������Ű
				if (info.type == TypeCode_Key_Press && info.code == 0x27) {
					if (p_tetrimino->near_block[0] == 0)
						p_tetrimino->pos.x += 1;
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

		//		add_tetrimino_board(p_tetrimino);




				//�ð������� ����� ������
		if (fast_flag == 1)
			time_interval_update = 0.1;

		if (immeadiatly_flag == 1)
			time_interval_update = 0.01;

		if (Stopwatch_Get() - time_now > time_interval_update) {
			//			if (p_tetrimino->near_block[1] == 0)
			p_tetrimino->pos.y += 1;
			time_now = Stopwatch_Get();



			for (i = 0; i < 22; i++) {
				for (j = 0; j < 21; j++) {
					Move_Cursor(2 * i + 21, j);
					printf("%d ", board[i][j]);
				}
			}
			// */
			Move_Cursor(30, 25);
			//printf("���� %d, ������ %d, ", p_tetrimino->near_block[2], p_tetrimino->near_block[0]);
			//printf("�Ʒ��� %d, ���� %d", p_tetrimino->near_block[1], p_tetrimino->near_block[3]);
		}

		//����� �ٴڿ� ���̸� ���ο� �� ����
		if (p_tetrimino->near_block[1] == 1)
			make_new_tetrimino(p_tetrimino);

		delete_tetrimino_board(p_tetrimino);
		add_tetrimino_board(p_tetrimino);
		line_cnt += next_stage_check(p_tetrimino);

		//��� �� ������ �����
		Move_Cursor(10, 30);
		printf("cleard line : %d", line_cnt);
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
	//int t_type = rand() % 7;
	int t_type = 1;
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
void check(block * p_tetrimino, int x, int y) {
	int dx[4] = { 1, 0, -1, 0 };
	int dy[4] = { 0, 1, 0, -1 };
	int condition;
	int can_go;
	visit[x][y] = 1;

	int i;
	for (i = 0; i < 4; i++) {
		condition = (x < 1 || y > 19 || x > 20 || visit[x + dx[i]][y + dy[i]]);
		can_go = !(board[x + dx[i]][y + dy[i]] == 1 || board[x + dx[i]][y + dy[i]] == 2);
		if (condition == 0 && can_go == 0)
			p_tetrimino->near_block[i] = 1;
		if (condition == 0 && board[x + dx[i]][y + dy[i]] == 10)
			check(p_tetrimino, x + dx[i], y + dy[i]);

	}
}

//������ �� ä������ �� ���� ���� ȭ���� �����ϴ� �Լ�
int next_stage_check(block* p_tetrimino) {
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
			down_board(p_tetrimino, i);
			system("cls");
		}
	}

	return next_stage_count;
}

//�� ���� �� ä���� �� �� �� ���� �ϳ� ������ �Լ�
void down_board(block* p_tetrimino, int line) {
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