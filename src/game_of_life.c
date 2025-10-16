#include <ncurses.h>
#include <stdio.h>

#define HEIGHT 25
#define WIDTH 80

int input(int old_gen[][WIDTH]);
void draw_field(int old_gen[][WIDTH]);
int count_neighbors(int old_gen[][WIDTH], int y, int x);
int new_state(int old_gen[][WIDTH], int i, int j);
void ncurses_init();

int main() {
    int old_gen[HEIGHT][WIDTH] = {0};
    int speed = 100;
    int flag = input(old_gen);
    ncurses_init();
    // GAME
    while (flag) {
        int next_gen[HEIGHT][WIDTH] = {0};
        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                next_gen[i][j] = new_state(old_gen, i, j);
            }
        }
        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                old_gen[i][j] = next_gen[i][j];
            }
        }
        // Ввод с клавы
        char buff = getch();
        if (buff == ' ') {
            flag = 0;
        } else if (buff == 'a' && speed >= 30) {
            speed -= 30;
        } else if (buff == 'z' && speed <= 380) {
            speed += 50;
        }
        napms(speed);
        clear();
        draw_field(old_gen);
    }
    endwin();
    return 0;
}

int input(int old_gen[][WIDTH]) {
    int is_correct = 1;
    if (freopen("1_glider.txt", "r", stdin)) {
        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                scanf("%1d", &old_gen[i][j]);
            }
        }
    } else {
        printf("n/a");
        is_correct = 0;
    }
    if (!freopen("/dev/tty", "r", stdin)) {
        is_correct = 0;
    }
    return is_correct;
}

void draw_field(int old_gen[][WIDTH]) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (old_gen[i][j] == 1) {
                printw("o");
            } else {
                printw(" ");
            }
        }
        printw("\n");
    }
}

int count_neighbors(int old_gen[][WIDTH], int y, int x) {
    int counter = 0;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) continue;
            int n_y = (y + i + HEIGHT) % HEIGHT;
            int n_x = (x + j + WIDTH) % WIDTH;
            counter += old_gen[n_y][n_x];
        }
    }
    return counter;
}

int new_state(int old_gen[][WIDTH], int i, int j) {
    int state = 0;
    int count = count_neighbors(old_gen, i, j);
    if (old_gen[i][j] == 1) {
        if (count == 2 || count == 3) {
            state = 1;
        }
    } else {
        if (count == 3) {
            state = 1;
        }
    }
    return state;
}

void ncurses_init() {
    initscr();
    noecho();
    cbreak();
    curs_set(FALSE);
    nodelay(stdscr, TRUE);
}
