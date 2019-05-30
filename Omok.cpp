#include<stdio.h>
#include<stack>

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

int i, j, k;
int turn = 1;
char board[21][21];
int board_index[21];


int main() {
	init_board();
	while (1) {
		print_board();
		player1_turn(turn);
		print_board();
		player2_turn(turn);
		print_board();
		turn++;

	}
}

void player1_turn(int turn) {
	player_turn_input( 1, turn);
	board[player[1][turn].x][player[1][turn].y] = '@';
	pre_turn.push(player[1][turn]);
	
}

void player2_turn(int turn) {
	player_turn_input(2, turn);
	board[player[2][turn].x][player[2][turn].y] = '#';
	pre_turn.push(player[2][turn]);
}


void player_turn_input(int player_num, int turn) {
	//char* now_coord = &board[player[player_num][turn].x][player[player_num][turn].y];

	printf("\n");
	printf("  Player %d", player_num);
	printf("			turn %d", turn);
	printf("Where is your next choice? : ");
	scanf("%d %d", &player[player_num][turn].x, &player[player_num][turn].y);
	while (board[player[player_num][turn].x][player[player_num][turn].y] != '.') {
		printf("You can't choice here! ");
		printf("You should coice another coord : ");
		scanf("%d %d", &player[player_num][turn].x, &player[player_num][turn].y);
	}
}

void print_board(void) {
	for (i = 1; i < 21; i++) {
		printf("%3d", board_index[21 - i]);

		for (j = 1; j < 21; j++) 
			printf("%3c", board[i][j]);
		printf("\n");
	}

	for (i = 0; i < 21; i++) printf("%3d", board_index[i]);
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