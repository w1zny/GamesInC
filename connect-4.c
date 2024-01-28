#include <stdio.h>
#include <ctype.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>

void initialize_board(int rows, int cols, char board[rows][cols]);
void print_board(int rows, int cols, const char board[rows][cols]);
int is_valid_move(int rows, int cols, const char board[rows][cols], int col);
int drop_piece(int rows, int cols, char board[rows][cols], int col, char player_piece);
int check_win(int rows, int cols, const char board[rows][cols], int row, int col, char player_piece);
int is_board_full(int rows, int cols, const char board[rows][cols]);

void initialize_board(int rows, int cols, char board[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            board[i][j] = '.';
        }
    }
}

void print_board(int rows, int cols, const char board[rows][cols]) {
    printf("\n");
    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < cols; x++) {
            printf("\033[1;30m| ");
            if (board[y][x] == 'X') {
                printf("\033[1;36m%c ", board[y][x]);
            }
            else if (board[y][x] == 'O') {
                printf("\033[1;35m%c ", board[y][x]);
            }
            else {
                printf("\033[1;37m. ");
            }
        }
        printf("\033[1;30m|\n");
    }
    for (int i = 0; i < cols; i++) {
        printf("\033[1;30m----");
    }
    printf("\033[1;30m-\n");
    for (int i = 0; i < cols; i++) {
        printf("\033[1;30m  %d ", i + 1);
    }
    printf(" \n\n");
}

int is_valid_move(int rows, int cols, const char board[rows][cols], int col) {
    if (col < 1 || col > cols || board[0][col-1] != '.') {
        printf("\033[1;31mInvalid move!\n");
        return 0;
    }

    return 1;
}

int drop_piece(int rows, int cols, char board[rows][cols], int col, char player_piece) {
    if (board[0][col-1] != '.') {
        return -1;
    }

    for (int i = rows-1; i >= 0; i--) {
        if (board[i][col-1] == '.') {
            board[i][col-1] = player_piece;
            return i;
        }
    }
    return 0;
}

int check_win(int rows, int cols, const char board[rows][cols], int row, int col, char player_piece) {
    if (rows < row) {
        row = 0;
    }
    if (cols < col) {
        col = 0;
    }

    // horizontal
    for (int i = 0; i < cols-3; i++) {
        if (board[row][i] != '.' && board[row][i] == board[row][i+1] && board[row][i] == board[row][i+2] && board[row][i] == board[row][i+3]){
            return 1;
        }
    }

    // vertical
    for (int i = 0; i < rows-3; i++) {
        if (board[i][col] != '.' && board[i][col] == board[i+1][col] && board[i][col] == board[i+2][col] && board[i][col] == board[i+3][col]){
            return 1;
        }
    }

    // horizontal
    for (int i = 0; i < cols-3; i++) {
        for (int j = 0; j < rows-3; j++) {
            if (board[j][i] != '.' && board[j][i] == board[j+1][i+1] && board[j][i] == board[j+2][i+2] && board[j][i] == board[j+3][i+3]) {
                return 1;
            }
            else if (board[j][cols-i-1] != '.' && board[j][cols-i-1] == board[j+1][cols-i-2] && board[j][cols-i-1] == board[j+2][cols-i-3] && board[j][cols-i-1] == board[j+3][cols-i-4]) {
                return 1;
            }
        }
    }

    return 0;
}

int is_board_full(int rows, int cols, const char board[rows][cols]) {
    int count = 0;
    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < cols; x++) {
            if (board[y][x] != '.') {
                count++;
            }
        }
    }
    if (count != rows*cols) {
        return 0;
    }
    return 1;
}


int main() {
 	srand(time(NULL));
 	bool end_game = false;
 	int rows = rand()%6 + 4;
 	int cols = rand()%6 + 4;
 	int bigger_num = (rows > cols) ? rows : cols;
 	char board[rows][cols];
 	initialize_board(rows, cols, board);
 	int player = 1;
 	char player_piece;
 	while (!end_game) {
 		signed int input = -1;
 		print_board(rows, cols, board);
 		if (player == 1) {
 			printf("\033[1;36mPlayer %d", player);
 		}
 		else if (player == 2) {
 			printf("\033[1;35mPlayer %d", player);
 		}
 		printf("\033[1;0m, enter your move (1-%d): ", cols);
 		scanf("%d", &input);
 		fflush(stdin);
 		if (is_valid_move(rows, cols, board, input)) {
 			if (player == 2) {
 				player_piece = 'O';
 				player = 1;
 			}
 			else if (player == 1) {
 				player_piece = 'X';
 				player = 2;
 			}
 			drop_piece(rows, cols, board, input, player_piece);
 		}

 		for (int i = 0; i < bigger_num; i++) {
 			if (check_win(rows, cols, board, i, i, 'X') || check_win(rows, cols, board, i, i, 'O')) {
 				end_game = true;
 			}
 		}
 		if (is_board_full(rows, cols, board)) {
 			end_game = true;
 		}

 	}
 	print_board(rows, cols, board);

 	for (int i = 0; i < bigger_num; i++) {
 		if (check_win(rows, cols, board, i, i, 'X') || check_win(rows, cols, board, i, i, 'O')) {
 			if (player == 2) {
 				printf("\033[1;36mPlayer 1 won!\n");
 				break;
 			}
 			else if (player == 1) {
 				printf("\033[1;35mPlayer 2 won!\n");
				break;
			}
		}
	}

	if (is_board_full(rows, cols, board)) {
		printf("\033[1;37mIt is a draw!\n");
	}
	printf("\033[1;0m\n");
}
