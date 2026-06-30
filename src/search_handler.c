#include <stdlib.h>
#include <string.h>

#include "search_handler.h"
#include "soundex.h"
#include "types.h"

static size_t split_query(const char *query, char ***out_split) {
    size_t len = strlen(query) + 1;
    char *tmp = malloc(len);
    strncpy(tmp, query, len);

    size_t n_split = 0;

    char *token = strtok(tmp, "_-.");
    while (token != NULL) {
        n_split++;
        token = strtok(NULL, "_-.");
    }

    free(tmp);

    *out_split = malloc(sizeof(char *) * n_split);

    char *tmp2 = malloc(len);
    strncpy(tmp2, query, len);

    token = strtok(tmp2, "_-.");
    size_t i = 0;
    while (token != NULL && i < n_split) {
        (*out_split)[i] = malloc(strlen(token) + 1);
        strncpy((*out_split)[i], token, strlen(token) + 1);
        token = strtok(NULL, "_-.");
        i++;
    }

    free(tmp2);

    return n_split;
}

static u8 fuzzy_search(const char *query, char **split, char **original_split,
                        size_t n_split, size_t n_original_split, s_vec *sounds,
                        s_vec *results, s_vec *temp) {

    u8 matches = 0;

    // looping through each string in the query
    matches = search(split, query, n_split, n_original_split, sounds, results);

    for (size_t i = 0; i < results->size; i++) {
        f64 total = 0.0;
        s_entry *e = (s_entry *)vec_at(results, i);

        for (size_t c = 0; c < n_original_split; c++) {
            f64 best = 0.0;
            size_t j = 0;
            while (j < e->n_codes) {
                f64 d = get_dice_score(original_split[c], e->tokens[j]);
                if (d > best)
                    best = d;
                j++;
            }
            total += best;
        }

        f64 penalty = (f64)n_original_split / (f64)e->n_codes;
        e->score = (total / (f64)n_original_split) * penalty;
    }

    topDownMergeSort(results, temp, results->size);

    return matches;
}

void search_ctx_init(search_ctx_t *ctx) {
    vec_init(&ctx->temp, sizeof(s_entry));
    vec_init(&ctx->results, sizeof(s_entry));
    ctx->match_count = 0;
    ctx->n_split = 0;
    ctx->n_original_split = 0;
    ctx->split = NULL;
    ctx->original_split = NULL;
}

void search_ctx_free(search_ctx_t *ctx) {
    vec_free(&ctx->results);
    free(ctx->temp.data);

    for (size_t i = 0; i < ctx->n_split; i++) {
        free(ctx->split[i]);
    }
    free(ctx->split);

    for (size_t i = 0; i < ctx->n_original_split; i++) {
        free(ctx->original_split[i]);
    }
    free(ctx->original_split);
}

void search_ctx_run(search_ctx_t *ctx, const char *query, s_vec *sounds) {

    // free previous runs
    search_ctx_free(ctx);

    vec_init(&ctx->results, sizeof(s_entry));
    vec_init(&ctx->temp, sizeof(s_entry));

    // recompute splits from new query
    ctx->n_split = split_query(query, &ctx->split);
    ctx->n_original_split = split_query(query, &ctx->original_split);

    // soundex the new splits
    for (size_t i = 0; i < ctx->n_split; i++) {
        soundex_str(ctx->split[i]);
    }

    // run search
    ctx->match_count =
        fuzzy_search(query, ctx->split, ctx->original_split, ctx->n_split,
                     ctx->n_original_split, sounds, &ctx->results, &ctx->temp);
}
