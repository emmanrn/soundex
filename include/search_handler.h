#ifndef SEARCH_HANDLER_H
#define SEARCH_HANDLER_H

#include "soundex.h"

typedef struct {
    char **split;
    char **original_split;
    size_t n_split;
    size_t n_original_split;
    s_vec results;
    s_vec temp;
    int match_count;
} search_ctx_t;

void search_ctx_init(search_ctx_t *ctx);
void search_ctx_free(search_ctx_t *ctx);
void search_ctx_run(search_ctx_t *ctx, const char *query, s_vec *sounds);

#endif
