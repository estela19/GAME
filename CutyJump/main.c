#include "console.h"

#include <stdio.h>
#include<time.h>

// 프로그램 전체에서 사용할 각종 이름들을 선언해 둘 만한 부분
void score(void);
void print_me(int a, int b, const char* c);
void print_block(int x, int n);
void print_me_run(void);
void move_block(int a);
void print_me_slide(void);
void erase_block(int x, int n);
int fail(void);
int fail_(int a);
char me_run[5] = { "run!" };
char me_slide[6] = { "slide" };
char me_jump[5] = { "jump" };
char space[5] = { "     " };
char block1 = 'M';
char block2[5] = { "MH" };
char block_fly[5] = { "Wo" };
int pos_me_x = 5;
int pos_me_y = 11;
double time_push_me_jump = -1;
double time_push_me_slide = -1;
double interval = 0.3;
double interval_update_obstacle_var;
double interval_update_obstacle = 0.05;
double time_now;
double time_start;
char what_block[5] = { 1,2,3 };
double time_push_block[5];
int pos_block[5] = { 62, };
int n = 1;
int breakpoint_ = 0;
#define baseposition 11
#define jumphigh 4


int main()
{

	// 라이브러리를 사용하기 위해 미리 적어 둔 부분(창 세팅하기 + 스톱워치 켜기)
	Start_Console();
	Stopwatch_Start();

	int j = 0;

	for (int i = 0; i < 3; i++) time_push_block[i] = Stopwatch_Get();
	time_start = Stopwatch_Get();

	while (j < 64) {
		Move_Cursor(j, 15);
		printf("=");
		j++;
	}

	// '큰 while문'
	while (1)
	{
		// 사용자 입력을 처리하는 부분
		Accept_Inputs();

		time_now = Stopwatch_Get();


		while (Can_Read())
		{
			struct Info_Input info;

			Read_Input(&info);

			if (info.type & TypeCode_Key)
			{
				//sace 키를 눌렀을때
				if (info.type == TypeCode_Key_Press && info.code == 0x20 && time_push_me_jump == -1) {
					time_push_me_jump = time_now;
				}

				//아래키를 눌렀을때
				if (info.type == TypeCode_Key_Press && info.code == 0x28 && time_push_me_slide == -1) {
					time_push_me_slide = time_now;
				}
			}
			else
			{
				struct Point pos;

				pos = Get_Mouse_Pos();

				// 마우스 입력을 처리하는 부분
			}
		}

		score();

		//상태에 따른 플레이어 출력
		print_me_run();
		print_me_slide();
		if (time_push_me_slide == -1 && time_push_me_jump == -1)
			print_me(pos_me_x, pos_me_y, me_run);

		//장애물 생성
		move_block(0);
		if (time_now - time_start >= interval_update_obstacle * 20) move_block(1);
		if (time_now - time_start >= interval_update_obstacle * 40) move_block(2);

		//장애물에 부딪혔을때
		breakpoint_ = fail();
		if (breakpoint_ == 1) break;

		// 잠시 쉬는 부분
		Stopwatch_SpinLock(1.0 / 240);
	}

	return 0;
}

void score(void) {
	double score;
	score = time_now * 1000;
	Move_Cursor(45, 1);
	printf("Score : %d", (int)score);
}


int fail(void) {
	int breakpoint[3];
	breakpoint[0] = fail_(0);
	breakpoint[1] = fail_(1);
	breakpoint[2] = fail_(2);
	int break_ = (breakpoint[0] || breakpoint[1] || breakpoint[2]);
	return break_;
}

int fail_(int a) {
	int breakpoint_ = 0;
	if (pos_block[a] == pos_me_x) {
		if (what_block[a] == 1) {
			if (pos_me_y <= 14 && 14 <= pos_me_y + 3) breakpoint_ = 1;
		}
		else if (what_block[a] == 2) {
			if (pos_me_y <= 13 && 13 <= pos_me_y + 3) breakpoint_ = 1;
		}
		else if (what_block[a] == 3) {
			if (pos_me_y <= 11 && 11 <= pos_me_y + 3) breakpoint_ = 1;
		}
		else if (what_block[a] == 4) {
			if (pos_me_y <= 7 && 7 <= pos_me_y + 3) breakpoint_ = 1;
		}
	}
	return breakpoint_;
}


void print_me_slide(void) {
	if (time_push_me_slide != -1) {
		print_me(pos_me_x, pos_me_y, space);

		if (time_now <= interval * 3 + time_push_me_slide) {
			pos_me_y = 14;
			Move_Cursor(pos_me_x, pos_me_y);
			printf("%s", me_slide);
		}
		else {
			Move_Cursor(pos_me_x, pos_me_y);
			printf("%s", space);
			pos_me_y = baseposition;
			time_push_me_slide = -1;
		}
	}

}

void print_me_run(void) {
	if (time_push_me_jump != -1) {
		print_me(pos_me_x, pos_me_y, space);

		if (time_now > time_push_me_jump + interval) {
			if (time_now > time_push_me_jump + interval * 2) {
				pos_me_y = baseposition;
				time_push_me_jump = -1;
			}
			else {
				pos_me_y = baseposition - jumphigh + jumphigh * (time_now - time_push_me_jump - interval) / interval;
				print_me(pos_me_x, pos_me_y, me_jump);
			}
		}
		else {
			pos_me_y = baseposition - jumphigh * (time_now - time_push_me_jump) / interval;
			print_me(pos_me_x, pos_me_y, me_jump);
		}
	}
}

void move_block(int a) {
	print_block(pos_block[a], what_block[a]);

	if (pos_block[a] == 0) {
		erase_block(pos_block[a], what_block[a]);
		pos_block[a] = 62;
		srand(time(NULL));
		what_block[a] = rand() % 5;
	}

	interval_update_obstacle_var = 0.9 * interval_update_obstacle / (interval_update_obstacle * (time_now - time_start) + 1);

	if (time_now - time_push_block[a] >= interval_update_obstacle_var) {
		erase_block(pos_block[a], what_block[a]);
		pos_block[a]--;
		print_block(pos_block[a], what_block[a]);
		time_push_block[a] = Stopwatch_Get();
	}

}

void print_me(int a, int b, const char* c) {
	Move_Cursor(a, b);
	printf("%c", c[0]);
	Move_Cursor(a, b + 1);
	printf("%c", c[1]);
	Move_Cursor(a, b + 2);
	printf("%c", c[2]);
	Move_Cursor(a, b + 3);
	printf("%c", c[3]);
}


void print_block(int x, int n) {
	if (n == 1) {
		Move_Cursor(x, 14);
		printf("%c", block1);
	}
	else if (n == 2) {
		Move_Cursor(x, 13);
		printf("%c", block2[0]);
		Move_Cursor(x, 14);
		printf("%c", block2[1]);
	}
	else if (n == 3) {
		Move_Cursor(x, 11);
		printf("%s", block_fly);
	}
	else if (n == 4) {
		Move_Cursor(x, 7);
		printf("%s", block_fly);
	}
}

void erase_block(int x, int n) {
	if (n == 1) {
		Move_Cursor(x, 14);
		printf("%s", space);
	}
	else if (n == 2) {
		Move_Cursor(x, 13);
		printf("%s", space);
		Move_Cursor(x, 14);
		printf("%s", space);
	}
	else if (n == 3) {
		Move_Cursor(x, 11);
		printf("%s", space);
	}
	else if (n == 4) {
		Move_Cursor(x, 7);
		printf("%s", space);
	}
}

