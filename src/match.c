#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "match.h"
#include "types.h"

#define MATCH_SCORE 16.0 // base score for any matched character
#define CONSECUTIVE_BONUS                                                      \
    16.0 // extra if this match follows another match directly
#define BOUNDARY_BONUS                                                         \
    8.0 // extra if match is at start of target or after a separator (_ - .)
#define GAP_PENALTY 1.0 // cost per skipped character in target between matches
#define NEG_INF -1e9

static int is_subsequence(const char *query, const char *target) {
    int qi = 0;
    int qlen = (int)strlen(query);
    int ti = 0;
    int tlen = (int)strlen(target);

    while (qi < qlen && ti < tlen) {
        if (tolower((unsigned char)query[qi]) ==
            tolower((unsigned char)target[ti])) {
            qi++;
        }

        ti++;
    }
    return qi == qlen;
}

f64 fzn_score(const char *query, const char *target) {
    i32 qlen = (i32)strlen(query);
    i32 tlen = (i32)strlen(target);

    if (qlen == 0 || tlen == 0) {
        return 0.0;
    }

    if (!is_subsequence(query, target))
        return 0.0;

    i32 rows = qlen + 1;
    i32 cols = tlen + 1;

    f64 *M = malloc(sizeof(f64) * rows * cols);
    f64 *D = malloc(sizeof(f64) * rows * cols);

    if (!M || !D) {
        free(M);
        free(D);
        return 0.0;
    }

// initialize row 0 and column 0
#define IDX(i, j) ((i) * cols + (j))
    for (i32 j = 0; j < cols; j++) {
        M[IDX(0, j)] = 0.0;
        D[IDX(0, j)] = NEG_INF;
    }

    for (i32 i = 0; i < rows; i++) {
        M[IDX(i, 0)] = 0.0;
        D[IDX(i, 0)] = NEG_INF;
    }
    // fill the rest of the matrix
    for (i32 i = 1; i < rows; i++) {
        char qc = (char)tolower((unsigned char)query[i - 1]);

        for (i32 j = 1; j < cols; j++) {
            char tc = (char)tolower((unsigned char)target[j - 1]);
            if (qc == tc) {
                f64 boundary = 0.0;

                if (j == 1 || !isalnum((unsigned char)target[j - 2])) {
                    boundary = BOUNDARY_BONUS;
                }

                // option 1: previous query char also matched right before this
                // position (consecutive)
                f64 consecutive_option = NEG_INF;
                if (D[IDX(i - 1, j - 1)] > NEG_INF) {
                    consecutive_option =
                        D[IDX(i - 1, j - 1)] + MATCH_SCORE + CONSECUTIVE_BONUS;
                }

                f64 fresh_option =
                    M[IDX(i - 1, j - 1)] + MATCH_SCORE + boundary;

                D[IDX(i, j)] = (consecutive_option > fresh_option)
                                   ? consecutive_option
                                   : fresh_option;
            } else {
                // no match possible at this cell
                D[IDX(i, j)] = NEG_INF;
            }

            f64 skip_option = M[IDX(i, j - 1)] - GAP_PENALTY;
            M[IDX(i, j)] =
                (D[IDX(i, j)] > skip_option) ? D[IDX(i, j)] : skip_option;
        }
    }

    f64 max_score = 0.0;
    for (i32 j = 0; j < cols; j++) {
        if (M[IDX(qlen, j)] > max_score) {
            max_score = M[IDX(qlen, j)];
        }
    }
// free and return
#undef IDX

    free(M);
    free(D);
    return max_score;
}
