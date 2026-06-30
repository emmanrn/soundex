#ifndef TTY_H
#define TTY_H

#include <ncurses.h>

#include "search_handler.h"
#include "soundex.h"
#include "types.h"


// lifecycle
void tty_init(void);
void tty_cleanup(void);

// rendering
void tty_render(WINDOW *input_win, WINDOW *result_win, char *query,
                i32 cursor_pos, u16 hightlight, search_ctx_t *ctx);

// main loop
void tty_run(s_vec *sounds);

#endif
