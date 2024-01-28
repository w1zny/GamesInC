#define _POSIX_C_SOURCE 200201L
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <curses.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

void draw_player(int player[2]) {
    attron(A_BOLD);
    attron(COLOR_PAIR(2));
    mvprintw(player[0]-1, player[1], "A");
    mvprintw(player[0], player[1]-2, "/(@)\\");
    mvprintw(player[0]+1, player[1], "V");
    attroff(A_BOLD);
}

void end_game(int player[2]) {
    int vertical = (LINES-player[0] >= player[0]) ? LINES-player[0] : player[0];

    struct timespec ts = {
            .tv_sec = 0,
            .tv_nsec = 0.01 * 1000000000L
    };

    for (int i = 0; i <= vertical; i++) {
        for (int j = 0; j < COLS; j++) {
            mvprintw(player[0]-i, j, " ");
        }
        for (int j = 0; j < COLS; j++) {
            mvprintw(player[0]+i, j, " ");
        }
        draw_player(player);
        refresh();
        nanosleep(&ts, NULL);
    }
}

void draw_grass(const int start_line, const int end_line) {
    attron(COLOR_PAIR(1));
    char grass = 'W';
    for (int i = LINES; i > start_line; i--) {
        for (int j = 0; j < COLS; j++) {
            mvprintw(i, j, "%c", grass);
        }
    }

    for (int i = end_line; i >= 0; i--) {
        for (int j = 0; j < COLS; j++) {
            mvprintw(i, j, "%c", grass);
        }
    }
}

void draw_log(int log[4], const int log_len) {
    attron(COLOR_PAIR(3));
    attron(A_BOLD);
    for (int i = log[0]-1; i <= log[0]+1; i++) {
        mvprintw(i, log[1]-log_len/2-1, " ");
        for (int j = -log_len/2; j <= log_len/2; j++) {
            mvprintw(i, log[1]+j, "#");
        }
        mvprintw(i, log[1]+log_len/2+1, " ");
    }
    attroff(A_BOLD);
}

bool player_on_log(int player[2], int log[4]) {
    if (player[0] == log[0] && player[1] <= log[1]+log[2]/2 && player[1] >= log[1]-log[2]/2) {
        return true;
    }
    return false;
}

void create_logs(const int start_line, const int num, int logs[num][4]) {
    srand(time(NULL));
    int start = start_line-2;
    for (int i = 0; i < num; i++) {
        logs[i][0] = start-i*4;
        logs[i][1] = rand()%COLS;
        logs[i][2] = rand()%15 + 5;
        logs[i][3] = (i%2==1) ? 1 : -1;
    }
}

bool player_dead(int player[2], const int num, int logs[num][4], const int start_line, const int end_line) {
    if (player[1] <= -1 || player[1] >= COLS) {
        return true;
    }
    for (int i = 0; i < num; i++) {
        if (player[0] == logs[i][0]) {
            if (player[1] < logs[i][1]-logs[i][2]/2 || player[1] > logs[i][1]+logs[i][2]/2) {
                return true;
            }
        }
    }

    return false;
}

int main() {
    initscr();
    curs_set(false);
    noecho();
    nodelay(stdscr, true);
    keypad(stdscr, true);
    start_color();
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_WHITE, COLOR_BLACK);
    init_pair(3, COLOR_RED, COLOR_BLACK);

    int start_line = LINES-5;
    int num_of_logs = start_line/4;
    int end_line = start_line-4*num_of_logs-1;

    int logs[num_of_logs][4];
    create_logs(start_line, num_of_logs, logs);

    int player[2] = {start_line+2, COLS/2};
    int score = 0;

    int input;

    struct timespec ts = {
            .tv_sec = 0,
            .tv_nsec = 0.1 * 1000000000L
    };

    while ( (input = getch()) && !player_dead(player, num_of_logs, logs, start_line, end_line) ) {
        switch (input) {
            case 'q': case 'Q': {
                getchar();
                endwin();
                return EXIT_SUCCESS;
            }
            case KEY_UP: {
                score++;
                player[0] -= 4;
                break;
            }
            case KEY_DOWN: {
                if (score > 0) {
                    player[0] += 4;
                    score--;
                }
                break;
            }
            case KEY_RIGHT: {
                if (player[1]+5 >= COLS) {
                    break;
                }
                player[1] += 5;
                break;
            }
            case KEY_LEFT: {
                if (player[1]-5 <= -1) {
                    break;
                }
                player[1] -= 5;
                break;
            }
        }

        clear();
        draw_grass(start_line, end_line);
        for (int i = 0; i < num_of_logs; i++) {
            draw_log(logs[i], logs[i][2]);
        }
        draw_player(player);
        refresh();

        nanosleep(&ts, NULL);

        for (int i = 0; i < num_of_logs; i++) {
            if (player_on_log(player, logs[i])) {
                player[1] += logs[i][3];
            }
            logs[i][1] += logs[i][3];

            if (logs[i][1] < -logs[i][2]/2) {
                logs[i][2] = rand()%15 + 5;
                logs[i][1] = COLS+logs[i][2]/2;

            }
            else if (logs[i][1] > COLS+logs[i][2]/2) {
                logs[i][2] = rand()%15 + 5;
                logs[i][1] = -logs[i][2]/2;
            }
        }

        if (player[0] < 0) {
            clear();
            napms(200);
            create_logs(start_line, num_of_logs, logs);
            player[0] = start_line+2;
        }
        else if (player[0] > LINES) {
            clear();
            napms(200);
            create_logs(start_line, num_of_logs, logs);
            player[0] = end_line-1;
        }
    }

    end_game(player);
    draw_player(player);

    mvprintw(LINES/2, COLS/2, "%d", score);

    refresh();
    getchar();
    endwin();
    return EXIT_SUCCESS;
}