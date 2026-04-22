#include "soundex.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int search(const char *input, s_vec *v, char **out_res, s_vec* out_vec) {
  int matches = 0;
  for (size_t i = 0; i < v->size; i++) {
    s_entry* e = (s_entry *)vec_at(v, i);
    char *og = e->original;
    size_t len = strlen(og) + 1;

    for (size_t j = 0; j < e->n_codes; j++) {
      char* s = e->codes[j];
      if (strncmp(input, s, strlen(input)) == 0) {
        out_res[matches] = malloc(len);

        if (out_res[matches]) {
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
          strncpy(out_res[matches], og, len);
          matches++;
        } else {
          for (size_t c = 0; c < e->n_codes; c++) {
            free(out_res[c]);
          }
          free(out_res);
          err_set_parse();
          return -1;
        }
      }
    }
  }
  return matches;
}
