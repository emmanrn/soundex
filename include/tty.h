#ifndef TTY_H
#define TTY_H

#include "soundex.h"
#include <ncurses.h>

// lifecycle
void tty_init(void);
void tty_cleanup(void);

// rendering
void tty_render(WINDOW* input_win, WINDOW* result_win, char* query, s_vec* results, int cursor);

// main loop
void tty_run(s_vec* sounds);

#endif