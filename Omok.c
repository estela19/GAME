#include<stdio.h>
#include<string.h>

char* undo(int player_num, int turn);
int check(int player_num, int turn);
int check_thr(int player_num, int turn);
void print_board(void);
void init_board(void);
void player1_turn(int turn);
void player2_turn(int turn);
void player_turn_input(int player_num, int turn);

typedef struct {
	int x, y;
}coord;


coord player[3][100];
coord temp;

int i, j, k;
int turn = 1;
char board[21][21];
int save_score[3][21][21];
int board_index[21];


int main() {
	init_board();
	print_board();
	while (1) {
		char* tmp;
		do {
			player1_turn(turn);
			system("cls");
			print_board();
			tmp = undo(1, turn);
			system("cls");
			print_board();
		} while (!strcmp(tmp, "yes") && !check_thr(1, turn));


		if (check(1, turn)) {
			printf("congraturation!!\n"
				"player 1 is Win!!");
			break;
		}

		do {
			player2_turn(turn);
			system("cls");
			print_board();
			tmp = undo(2, turn);
			system("cls");
			print_board();
		} while (!strcmp(tmp, "yes"));

		if (check(2, turn)) {
			printf("congraturation!!\n"
				"player 2 is Win!!");
			break;
		}
		turn++;
	}
	printf("\n");
	return 0;
}

int check_thr(int player_num, int turn) {
	const int add_y[8] = { 1, 1, 1, 0, -1, -1, -1, 0 };
	const int add_x[8] = { 1, 0, -1 , -1, -1, 0, 1, 1 };
	int pos_x = player[player_num][turn].x;
	int pos_y = player[player_num][turn].y;
	int new_x = pos_x;
	int new_y = pos_y;
	int cnt[8] = { 0 };
	int thr_cnt = 0;
	int result = 0;
	save_score[player_num][pos_x][pos_y] = 1;

	for (i = 0; i < 4; i++) {
		new_x = pos_x + add_x[i];
		new_y = pos_y + add_y[i];

		while (save_score[player_num][new_x][new_y] == 1) {
			new_x += add_x[i];
			new_y += add_y[i];
			cnt[i]++;
		}


		new_x = pos_x + add_x[i + 4];
		new_y = pos_y + add_y[i + 4];

		while (save_score[player_num][new_x][new_y] == 1) {
			new_x += add_x[i + 4];
			new_y += add_y[i + 4];
			cnt[i + 4]++;
		}
	}

	for (j = 0; j < 8; j++) {
		if (cnt[j] == 2 && cnt[j + 4] == 2) return 0;
		else if (cnt[j] == 2) thr_cnt++;
	}

	if (thr_cnt > 1) result = 1;

	return result;
}

int check(int player_num, int turn) {
	const int add_y[8] = { 1, 1, 1, 0, -1, -1, -1, 0 };
	const int add_x[8] = { 1, 0, -1 , -1, -1, 0, 1, 1 };
	int pos_x = player[player_num][turn].x;
	int pos_y = player[player_num][turn].y;
	int new_x = pos_x;
	int new_y = pos_y;
	int cnt = 0;
	int result = 0;
	save_score[player_num][pos_x][pos_y] = 1;

	for (i = 0; i < 4; i++) {
		new_x = pos_x + add_x[i];
		new_y = pos_y + add_y[i];

		while (save_score[player_num][new_x][new_y] == 1) {
			new_x += add_x[i];
			new_y += add_y[i];
			cnt++;
		}

		new_x = pos_x + add_x[i + 4];
		new_y = pos_y + add_y[i + 4];

		while (save_score[player_num][new_x][new_y] == 1) {
			new_x += add_x[i + 4];
			new_y += add_y[i + 4];
			cnt++;
		}

		if (cnt == 4) {
			result = 1;
			break;
		}

		cnt = 0;
	}
	return result;
}

char* undo(int player_num, int turn) {
	static char undo_input[10];
	char* tmp = undo_input;
	printf("  Player %d", player_num);
	printf("			turn %d\n\n", turn);
	printf("< UNDO >\n");
	printf("If you want undo type 'yes' :");
	scanf(" %s", undo_input);
	if (!strcmp(undo_input, "yes")) board[player[player_num][turn].x][player[player_num][turn].y] = '.';
	return tmp;
}

void player1_turn(int turn) {
	player_turn_input(1, turn);
	board[player[1][turn].x][player[1][turn].y] = '@';
}

void player2_turn(int turn) {
	player_turn_input(2, turn);
	board[player[2][turn].x][player[2][turn].y] = '#';
}


void player_turn_input(int player_num, int turn) {
	//char* now_coord = &board[player[player_num][turn].x][player[player_num][turn].y];

	printf("  Player %d", player_num);
	printf("			turn %d\n\n", turn);
	printf("  Where is your next choice? : ");
	scanf(" %d %d", &player[player_num][turn].x, &player[player_num][turn].y);

	while (board[player[player_num][turn].x][player[player_num][turn].y] != '.' || player[player_num][turn].x > 20 || player[player_num][turn].y > 20 || check_thr(player_num, turn)) {
		printf("You can't choice here! \n");
		if (check_thr(player_num, turn)) {
			printf("<There is thr-thr spot>\n");
		}
		printf("You should coice another coord : ");
		scanf(" %d %d", &player[player_num][turn].x, &player[player_num][turn].y);
	}
}

void print_board(void) {
	for (i = 0; i < 21; i++) printf("%3d", board_index[i]);
	printf("\n");

	for (i = 1; i < 21; i++) {
		printf("%3d", board_index[i]);

		for (j = 1; j < 21; j++)
			printf("%3c", board[i][j]);
		printf("\n");
	}
	printf("\n");
}

void init_board(void) {
	for (i = 0; i < 21; i++) board_index[i] = i;
	for (i = 1; i < 21; i++) {
		for (j = 1; j < 21; j++) {
			board[i][j] = '.';
		}
	}
}