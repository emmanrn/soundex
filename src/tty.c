#include <ctype.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "search_handler.h"
#include "soundex.h"
#include "tty.h"
#include "types.h"

// for detecting ctrl+ combinations
#define ctrl(x) (x & 0x1F)

static void tty_render_input_w(WINDOW *input_win, char *query, i32 cursor_pos);
static void tty_render_results_w(WINDOW *result_win, u16 highlight, search_ctx_t *ctx);
static void tty_open_txt_editor(char* filepath);

void tty_init() {
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
}

void tty_cleanup() { endwin(); }

void tty_render(WINDOW *input_win, WINDOW *result_win, char *query,
                i32 cursor_pos, u16 highlight, search_ctx_t *ctx) {
    tty_render_results_w(result_win, highlight, ctx);
    tty_render_input_w(input_win, query, cursor_pos);
}

static void tty_render_input_w(WINDOW *input_win, char *query, i32 cursor_pos) {
    werase(input_win);
    wborder(input_win, 0, 0, 0, 0, 0, 0, 0, 0);

    mvwprintw(input_win, 1, 1, "%s", query);
    wmove(input_win, 1, cursor_pos + 1);

    wrefresh(input_win);
}

static void tty_render_results_w(WINDOW *result_win, u16 highlight, search_ctx_t *ctx) {
    werase(result_win);
    wborder(result_win, 0, 0, 0, 0, 0, 0, 0, 0);

    u32 yMax, xMax;
    getmaxyx(result_win, yMax, xMax);
    (void)xMax;

    u8 match_count = ctx->match_count;
    u8 display_count = match_count;

    if (display_count > yMax - 2)
        display_count = yMax - 2;

    if (match_count > 0) {
        for (u8 i = 0; i < display_count; i++) {
            s_entry *e = (s_entry *)vec_at(&ctx->results, i);
            mvwprintw(result_win, yMax - 2 - i, 1, "%s", e->filepath);
            if (i == highlight)
                mvwchgat(result_win, yMax - 2 - i, 1, xMax - 2, A_REVERSE, 0, NULL);
        }
    }
    wrefresh(result_win);
}

void tty_run(s_vec *sounds) {
    tty_init();
    char query[256] = {0};
    size_t len = 0;
    size_t cursor_pos = 0;

    // init context_v
    search_ctx_t ctx;

    search_ctx_init(&ctx);

    u32 yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);

    WINDOW *input_win = newwin(3, xMax, yMax - 3, 0);
    keypad(input_win, TRUE);
    WINDOW *result_win = newwin(yMax - 3, xMax, 0, 0);

    read_files(sounds, ".");

    soundex(sounds);

    search_ctx_run(&ctx, query, sounds);


    u16 highlight = 0;
    u16 choice = 0;

    while (1) {
        tty_render(input_win, result_win, query, cursor_pos, highlight, &ctx);

        i32 ch = wgetch(input_win);

        if (ch == 27)
            break;
        if (ch == 10) {
            choice = highlight;
            s_entry* e = (s_entry*)vec_at(&ctx.results, choice);
            tty_open_txt_editor(e->filepath);
            break;
        }
        if (ch == KEY_LEFT) {
            if (cursor_pos > 0)
                cursor_pos--;
        } else if (ch == KEY_RIGHT) {
            if (cursor_pos < len)
                cursor_pos++;
        } else if (ch == KEY_BACKSPACE || ch == 127) {
            if (cursor_pos > 0) {
                memmove(&query[cursor_pos - 1], &query[cursor_pos],
                        len - cursor_pos + 1);
                cursor_pos--;
                len--;

                search_ctx_run(&ctx, query, sounds);
                highlight = 0;
            }
        } else if (ch == 78) {
            highlight = ((highlight) + 1) % (int)ctx.results.size;
        } else if (isprint(ch)) {
            if (len >= 100)
                continue;
            if (len < 255) {
                memmove(&query[cursor_pos + 1], &query[cursor_pos],
                        len - cursor_pos + 1);
                query[cursor_pos] = ch;
                cursor_pos++;
                len++;

                search_ctx_run(&ctx, query, sounds);
                highlight = 0;
            }
        } else if (ch == ctrl('n')) {
            highlight = ((highlight - 1) + (int)ctx.results.size) % (int)ctx.results.size;
        }
    }

    search_ctx_free(&ctx);
    tty_cleanup();
}

static void tty_open_txt_editor(char* filepath) {
    tty_cleanup();
    execl("/usr/bin/vim", "vim", filepath, NULL);
}