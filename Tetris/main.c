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
0x20 스페이스
0x25 왼쪽
0x26 위쪽
0x27 오른쪽
0x28 아래쪽
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
				//스페이스바
				if (info.type == TypeCode_Key_Press && info.code == 0x20) {
					immeadiatly_flag = 1;
				}

				//위쪽키
				if (info.type == TypeCode_Key_Press && info.code == 0x26) {
					// TODO: 회전 못하는 경우도 고려.
					rotate_tetrimino(p_tetrimino);
				}

				//아래키
				if (info.type == TypeCode_Key_Press && info.code == 0x28) {
					fast_flag = 1;
				}

				//왼쪽키
				if (info.type == TypeCode_Key_Press && info.code == 0x25) {
					if (p_tetrimino->near_block[2] == 0)
						p_tetrimino->pos.x -= 1;
				}

				//오른쪽키
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




				//시간에따라 블록이 내려옴
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
			//printf("왼쪽 %d, 오른쪽 %d, ", p_tetrimino->near_block[2], p_tetrimino->near_block[0]);
			//printf("아래쪽 %d, 위쪽 %d", p_tetrimino->near_block[1], p_tetrimino->near_block[3]);
		}

		//블록이 바닥에 놓이면 새로운 블럭 생성
		if (p_tetrimino->near_block[1] == 1)
			make_new_tetrimino(p_tetrimino);

		delete_tetrimino_board(p_tetrimino);
		add_tetrimino_board(p_tetrimino);
		line_cnt += next_stage_check(p_tetrimino);

		//블록 및 게임판 재출력
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

//tetrimino의 이전 정보를 지우는 함수
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

//tetrimino의 정보를 board에 입력하는 함수
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

//새로운 tetrimino를 만드는 함수
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

//tetrimino의 정보를 설정하는 함수
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

//tetrimino를 출력하는 함수
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

//이전에 이동완료한 tetrimino를 출력하는 함수
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

//tetrimino를 시계방향으로 90도 회전하는 함수
void rotate_tetrimino(block * p_tetrimino) {
	int temp = p_tetrimino->rotate;
	temp = (temp + 1) % 4;
	p_tetrimino->rotate = temp;
}

//기본적인 게임판을 만드는 함수
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

//저번 tetrimino의 출력을 지우기 위해 공백을 출력하는 함수
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

//내려오는 tetrimino 주위에 다른 장애물이 있는지 판별하는 함수
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

//한줄을 다 채웠을때 줄 수를 세고 화면을 조정하는 함수
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

//한 줄을 다 채웠을 때 그 윗 줄을 하나 내리는 함수
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