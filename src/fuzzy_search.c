#include "soundex.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int search(char** input_codes, size_t n_input_codes, size_t query_split, s_vec *v, s_vec* out_vec) {
  int matches = 0;
  for (size_t i = 0; i < v->size; i++) {
    s_entry* e = (s_entry *)vec_at(v, i);
    char *og = e->original;
    size_t match_count = 0;

    if (query_split > e->n_codes) {
      continue;
    }

    // check first if the input codes is not yet in the out_res
    // if not then push it to the res_vec
    for (size_t j = 0; j < e->n_codes; j++) {
      for (size_t c = 0; c < n_input_codes; c++) {
        if (strcmp(e->codes[j], input_codes[c]) == 0) {
          match_count++;
          break;
        }
      }
    }

    if (match_count == n_input_codes) {
          // push the matched soundex entry to the results vector
          size_t len1 = strlen(og) + 1;

          s_entry new_e = {
            .original = malloc(len1),
            .tokens = malloc(sizeof(char*) * e->n_codes),
            .codes = malloc(sizeof(char*) * e->n_codes),
            .n_codes = e->n_codes,
            .score = e->score,
          };

          if (!new_e.original || !new_e.codes) {
            free(new_e.original);
            free(new_e.codes);
            free(new_e.tokens);
            err_set_parse();
            return -1;
          }

          strncpy(new_e.original, og, len1);

          for (size_t c = 0; c < e->n_codes; c++) {
            new_e.codes[c] = malloc(strlen(e->codes[c]) + 1);
            new_e.tokens[c] = malloc(strlen(e->tokens[c]) + 1);
            strncpy(new_e.codes[c], e->codes[c], strlen(e->codes[c]) + 1);
            strncpy(new_e.tokens[c], e->tokens[c], strlen(e->tokens[c]) + 1);
          }


          if (vec_push(out_vec, &new_e) == -1) {
            for (size_t c = 0; c < new_e.n_codes; c++) {
              free(new_e.codes[c]);
              free(new_e.tokens[c]);
            }
            free(new_e.original);
            free(new_e.tokens);
            free(new_e.codes);
            err_set_parse();
            return -1;
          }
          matches++;
    }
  }
  return matches;
}
