#include "tty.h"
#include <ncurses.h>
#include <ctype.h>
#include <string.h>

static void tty_render_input_w(WINDOW* input_win, char* query, int cursor_pos);
static void tty_render_results_w(WINDOW* result_win, s_vec* results);

void tty_init() {
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
}

void tty_cleanup() {
    endwin();
}

void tty_render(WINDOW* input_win, WINDOW* result_win, char* query, s_vec* results, int cursor_pos) {
    tty_render_results_w(result_win, results);
    tty_render_input_w(input_win, query, cursor_pos);
}

static void tty_render_input_w(WINDOW* input_win, char* query, int cursor_pos) {
    werase(input_win);
    wborder(input_win, 0, 0, 0, 0, 0, 0, 0, 0);

    mvwprintw(input_win, 1, 1, "%s", query);
    wmove(input_win, 1, cursor_pos + 1);

    wrefresh(input_win);
}

static void tty_render_results_w(WINDOW* result_win, s_vec* results) {
    (void)results;
    werase(result_win);
    wborder(result_win, 0, 0, 0, 0, 0, 0, 0, 0);

    wrefresh(result_win);
}

void tty_run(s_vec* sounds) {
    (void)sounds;
    tty_init();
    char query[256] = { 0 };
    size_t len = 0;
    size_t cursor_pos = 0;

    // init other vectors
    s_vec results;
    s_vec temp;

    vec_init(&results, sizeof(s_entry));
    vec_init(&temp, sizeof(s_entry));

    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);

    WINDOW* input_win = newwin(3, xMax, yMax - 3, 0);
    keypad(input_win, TRUE);
    WINDOW* result_win = newwin(yMax - 3, xMax, 0, 0);

    while (1) {
        tty_render(input_win, result_win, query, &results, cursor_pos);

        int ch = wgetch(input_win);

        if (ch == 27) break;
        if (ch == KEY_LEFT) {
            if (cursor_pos > 0) cursor_pos--;
        } else if (ch == KEY_RIGHT) {
            if (cursor_pos < len) cursor_pos++;
        } else if (ch == KEY_BACKSPACE || ch == 127) {
            if (cursor_pos > 0) {
                memmove(&query[cursor_pos - 1], &query[cursor_pos], len - cursor_pos + 1);
                cursor_pos--;
                len--;
            }
        } else if (isprint(ch)){
            if (len >= 100) continue;
            if (len < 255) {
                memmove(&query[cursor_pos + 1], &query[cursor_pos], len - cursor_pos + 1);
                query[cursor_pos] = ch;
                cursor_pos++;
                len++;
            }
        }
    }

    tty_cleanup();
}