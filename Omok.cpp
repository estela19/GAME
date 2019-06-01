#include<stdio.h>
#include<stack>
#include<string.h>

char* undo(int player_num, int turn);
void print_board(void);
void init_board(void);
void player1_turn(int turn);
void player2_turn(int turn);
void player_turn_input(int player_num, int turn);

typedef struct {
	int x, y;
}coord;

std::stack<coord>pre_turn;

coord player[3][100];
coord temp;

int i, j, k;
int turn = 1;
char board[21][21];
char save_score[21][21];
int board_index[21];


int main() {
	init_board();
	print_board();
	while (1) {
		char* tmp;
		do {
			player1_turn(turn);
			tmp = undo(1, turn);
			print_board();
		} while (!strcmp( tmp, "yes"));

			player2_turn(turn);
		undo(2, turn);
		print_board();
		turn++;

	}
}

void check(void) {
	
}

char* undo(int player_num, int turn) {
	static char undo_input[10];
	char* tmp = undo_input;
	printf("< UNDO >\n");
	printf("If you want undo type 'yes' :");
	scanf("%s", undo_input);
	if (!strcmp(undo_input, "yes")) board[player[player_num][turn].x][player[player_num][turn].y] = '.';
	return tmp;
}

void player1_turn(int turn) {
	player_turn_input(1, turn);
	board[player[1][turn].x][player[1][turn].y] = '@';
	//pre_turn.push(player[1][turn]);	
}

void player2_turn(int turn) {
	player_turn_input(2, turn);
	board[player[2][turn].x][player[2][turn].y] = '#';
	//pre_turn.push(player[2][turn]);
}


void player_turn_input(int player_num, int turn) {
	//char* now_coord = &board[player[player_num][turn].x][player[player_num][turn].y];

	printf("\n");
	printf("  Player %d", player_num);
	printf("			turn %d\n", turn);
	printf("Where is your next choice? : ");
	scanf("%d %d", &player[player_num][turn].x, &player[player_num][turn].y);

	while (board[player[player_num][turn].x][player[player_num][turn].y] != '.') {
		printf("You can't choice here! ");
		printf("You should coice another coord : ");
		scanf("%d %d", &player[player_num][turn].x, &player[player_num][turn].y);
	}
}

void print_board(void) {
	for (i = 0; i < 21; i++) printf("%3d", board_index[i]);
	printf("\n");

	for (i = 1; i < 21; i++) {
		printf("%3d", board_index[21 - i]);

		for (j = 1; j < 21; j++)
			printf("%3c", board[i][j]);
		printf("\n");
	}

}

void init_board(void) {
	for (i = 0; i < 21; i++) board_index[i] = i;
	for (i = 1; i < 21; i++) {
		for (j = 1; j < 21; j++) {
			board[i][j] = '.';
		}
	}
}
