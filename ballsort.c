#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

void game_field(const int rows, const int columns, char field[rows][columns]);
bool check(const int rows, const int columns, char field[rows][columns]);
void down_possible(const int rows, const int columns, char field[rows][columns], int x, int y);
void generator(const int rows, const int columns, char field[rows][columns]);
bool is_on_same_symbol(const int rows, const int columns, char field[rows][columns], const char char_in_col, const int col_to_check);

int main() {
    srand(time(NULL));
    int rows = rand()%4 + 3;
    int cols = rand()%4 + 6;
    char field[rows][cols];
    generator(rows, cols, field);
    while (!check(rows, cols, field)) {
        game_field(rows, cols, field);
        int input_from, input_where;
        printf("\033[1;0mEnter what: ");
        scanf("%d", &input_from);
        printf("\033[1;0mEnter where: ");
        scanf("%d", &input_where);
        down_possible(rows, cols, field, input_from, input_where);
    }
    game_field(rows, cols, field);
    printf("\033[1;31mY");
    printf("\033[1;32mO");
    printf("\033[1;33mU");
    printf(" ");
    printf("\033[1;34mW");
    printf("\033[1;35mO");
    printf("\033[1;36mN");
    printf("\033[1;37m!");
    printf("\033[1;0m\n\n");
    return 0;
}

void generator(const int rows, const int columns, char field[rows][columns]) {
    srand(time(NULL));
    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < columns; x++) {
            field[y][x] = ' ';
        }
    }
    char all_chars[columns - 2];

    for (int i = 0; i < columns - 2; i++) {
        all_chars[i] = i + '!';
    }

    size_t all_chars_len = sizeof(all_chars);
    int banned_cols[2];
    banned_cols[0] = rand()%columns+1;

    while ((banned_cols[1] = rand()%columns+1) == banned_cols[0]) {
        continue;
    }

    for (int char_type = 0; char_type < all_chars_len; char_type++) {
        char curr_char = all_chars[char_type];
        for (int y = 0; y < rows; y++) {
            int char_x, char_y;
            do {
                char_x = rand()%columns;
                char_y = rand()%rows;
            } while (char_x == banned_cols[0]-1 || char_x == banned_cols[1]-1 || field[char_y][char_x] != ' ');
            field[char_y][char_x] = curr_char;
        }
    }
}

void game_field(const int rows, const int columns, char field[rows][columns]) {
    printf("\n");
    for (int y = 0; y < rows; y++) {
        printf("\033[1;30m%d ", y+1);
        for (int x = 0; x < columns; x++) {
            printf("\033[1;30m| ");
            switch (field[y][x]) {
                case '!': {
                    printf("\033[1;31m%c ", field[y][x]);
                    break;
                }case 1+'!': {
                    printf("\033[1;32m%c ", field[y][x]);
                    break;
                }case 2+'!': {
                    printf("\033[1;33m%c ", field[y][x]);
                    break;
                }case 3+'!': {
                    printf("\033[1;34m%c ", field[y][x]);
                    break;
                }case 4+'!': {
                    printf("\033[1;35m%c ", field[y][x]);
                    break;
                }case 5+'!': {
                    printf("\033[1;36m%c ", field[y][x]);
                    break;
                }case 6+'!': {
                    printf("\033[1;37m%c ", field[y][x]);
                    break;
                }case 7+'!': {
                    printf("\033[1;38m%c ", field[y][x]);
                    break;
                }case ' ': {
                    printf("%c ", field[y][x]);
                }
            }
        }
        printf("\033[1;30m|\n");
    }
    printf("   ");
    for (int x = 0; x < columns; x++) {
        printf("\033[1;30m--- ");
    }
    printf("\n   ");
    for (int x = 0; x < columns; x++) {
        printf("\033[1;30m %d  ", x+1);
    }
    printf("\n\n");
}

bool check(const int rows, const int columns, char field[rows][columns]) {
    int count_full_col = 0;
    int count;
    char first_char;
    for (int i = 0; i < columns; i++) {
        count = 0;
        if ( (first_char = field[rows-1][i]) != ' ' ) {
            count++;
            for (int j = rows-2; j >= 0; j--) {
                if (field[j][i] == first_char) {
                    count++;
                }
            }
        }
        if (count == rows) {
            count_full_col++;
        }
    }

    if (count_full_col == columns - 2) {
        return true;
    }
    return false;
}

void down_possible(const int rows, const int columns, char field[rows][columns], int x, int y) {
    if (x == y || x < 1 || x > columns || y < 1 || y > columns || field[0][y-1] != ' ') {
        printf("\033[1;31mInvalid move!\n");
        return;
    }
    x--;
    y--;

    char tmp = ' ';
    int i;
    for (i = 0; i < rows; i++) {
        if (field[i][x] != ' ') {
            tmp = field[i][x];
            field[i][x] = ' ';
            break;
        }
    }

    if (!is_on_same_symbol(rows, columns, field, tmp, y)) {
        field[i][x] = tmp;
        printf("\033[1;31mInvalid move!\n");
        return;
    }

    for (int i = 0; i < rows; i++) {
        if (field[i][y] != ' ') {
            if (tmp == field[i][y]) {
                field[i-1][y] = tmp;
            }
            break;
        }
        if (i == rows-1) {
            field[i][y] = tmp;
        }
    }
}

bool is_on_same_symbol(const int rows, const int columns, char field[rows][columns], const char char_in_col, const int col_to_check) {
    for (int i = 0; i < rows; i++) {
        if (field[i][col_to_check] != ' ') {
            if (field[i][col_to_check] == char_in_col) {
                return true;
            }
            return false;
        }
    }
    return true;
}
