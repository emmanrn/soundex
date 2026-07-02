#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "match.h"
#include "search_handler.h"
#include "soundex.h"
#include "types.h"

// #define MIN_SCORE_THRESHOLD 100.0

static int push_entry_copy(s_vec *out, s_entry *e) {
    s_entry new_e = {
        .original = malloc(strlen(e->original) + 1),
        .filepath = malloc(e->n_filepath),
        .tokens = malloc(sizeof(char *) * e->n_codes),
        .codes = malloc(sizeof(char *) * e->n_codes),
        .n_codes = e->n_codes,
        .n_filepath = e->n_filepath,
        .score = e->score,
    };

    if (!new_e.original || !new_e.filepath || !new_e.tokens || !new_e.codes) {
        free(new_e.original);
        free(new_e.filepath);
        free(new_e.tokens);
        free(new_e.codes);
        return -1;
    }

    strncpy(new_e.original, e->original, strlen(e->original) + 1);
    strncpy(new_e.filepath, e->filepath, e->n_filepath);

    for (size_t c = 0; c < e->n_codes; c++) {
        new_e.codes[c] = malloc(strlen(e->codes[c]) + 1);
        new_e.tokens[c] = malloc(strlen(e->tokens[c]) + 1);
        strncpy(new_e.codes[c], e->codes[c], strlen(e->codes[c]) + 1);
        strncpy(new_e.tokens[c], e->tokens[c], strlen(e->tokens[c]) + 1);
    }

    return vec_push(out, &new_e);
}

// static size_t split_query(const char *query, char ***out_split) {
//     size_t len = strlen(query) + 1;
//     char *tmp = malloc(len);
//     strncpy(tmp, query, len);
//
//     size_t n_split = 0;
//
//     char *token = strtok(tmp, "_-.");
//     while (token != NULL) {
//         n_split++;
//         token = strtok(NULL, "_-.");
//     }
//
//     free(tmp);
//
//     *out_split = malloc(sizeof(char *) * n_split);
//
//     char *tmp2 = malloc(len);
//     strncpy(tmp2, query, len);
//
//     token = strtok(tmp2, "_-.");
//     size_t i = 0;
//     while (token != NULL && i < n_split) {
//         (*out_split)[i] = malloc(strlen(token) + 1);
//         strncpy((*out_split)[i], token, strlen(token) + 1);
//         token = strtok(NULL, "_-.");
//         i++;
//     }
//
//     free(tmp2);
//
//     return n_split;
// }
//
// static u8 fuzzy_search(const char *query, char **split, char
// **original_split,
//                        size_t n_split, size_t n_original_split, s_vec
//                        *sounds, s_vec *results, s_vec *temp) {
//
//     u8 matches = 0;
//
//     // looping through each string in the query
//     matches = search(split, query, n_split, n_original_split, sounds,
//     results);
//
//     for (size_t i = 0; i < results->size; i++) {
//         f64 total = 0.0;
//         s_entry *e = (s_entry *)vec_at(results, i);
//
//         for (size_t c = 0; c < n_original_split; c++) {
//             f64 best = 0.0;
//             size_t j = 0;
//             while (j < e->n_codes) {
//                 f64 d = get_dice_score(original_split[c], e->tokens[j]);
//                 if (d > best)
//                     best = d;
//                 j++;
//             }
//             total += best;
//         }
//
//         f64 penalty = (f64)n_original_split / (f64)e->n_codes;
//         e->score = (total / (f64)n_original_split) * penalty;
//     }
//
//     topDownMergeSort(results, temp, results->size);
//
//     return matches;
// }

void search_ctx_init(search_ctx_t *ctx) {
    vec_init(&ctx->temp, sizeof(s_entry));
    vec_init(&ctx->results, sizeof(s_entry));
    ctx->match_count = 0;
    // ctx->n_split = 0;
    // ctx->n_original_split = 0;
    // ctx->split = NULL;
    // ctx->original_split = NULL;
}

void search_ctx_free(search_ctx_t *ctx) {
    vec_free(&ctx->results);
    free(ctx->temp.data);

    // for (size_t i = 0; i < ctx->n_split; i++) {
    //     free(ctx->split[i]);
    // }
    // free(ctx->split);
    //
    // for (size_t i = 0; i < ctx->n_original_split; i++) {
    //     free(ctx->original_split[i]);
    // }
    // free(ctx->original_split);
}

void search_ctx_run(search_ctx_t *ctx, const char *query, s_vec *sounds) {
    FILE *dbg = fopen("debug.log", "w");
    if (dbg)
        fclose(dbg);

    // free previous runs
    search_ctx_free(ctx);

    vec_init(&ctx->results, sizeof(s_entry));
    vec_init(&ctx->temp, sizeof(s_entry));

    u8 matches = 0;
    size_t qlen = strlen(query);
    f64 min_score = (f64)qlen * 20.0;
    if (qlen <= 1)
        min_score = 16.0;
    else if (min_score < 40.0)
        min_score = 40.0;

    for (size_t i = 0; i < sounds->size; i++) {
        s_entry *e = (s_entry *)vec_at(sounds, i);
        f64 score = fzn_score(query, e->filepath);

        if (qlen == 0) {
            e->score = 0.0;
            if (push_entry_copy(&ctx->results, e) == 0)
                matches++;
            continue;
        }

        if (score >= min_score) {
            e->score = score;
            if (push_entry_copy(&ctx->results, e) == 0) {
                matches++;
            }
        }
    }
    topDownMergeSort(&ctx->results, &ctx->temp, ctx->results.size);
    ctx->match_count = matches;

    // recompute splits from new query
    // ctx->n_split = split_query(query, &ctx->split);
    // ctx->n_original_split = split_query(query, &ctx->original_split);

    // soundex the new splits
    // for (size_t i = 0; i < ctx->n_split; i++) {
    //     soundex_str(ctx->split[i]);
    // }

    // run search
    // ctx->match_count =
    //     fuzzy_search(query, ctx->split, ctx->original_split, ctx->n_split,
    //                  ctx->n_original_split, sounds, &ctx->results,
    //                  &ctx->temp);
}
