#include "soundex.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <alloca.h>

// special thx to christoph in stackoverflow
static int starts_with(const char *restrict string, const char *restrict prefix)
{
    while(*prefix)
    {
        if(*prefix++ != *string++)
            return 0;
    }

    return 1;
}

static void convert_to_lower(char* s) {
    size_t len = strlen(s);
    if (s[len - 1] == '\n') {
        s[len - 1] = '\0';
        len = len - 1;
    }

    for (size_t i = 0; i < len; i++) {
        s[i] = tolower(s[i]);
    }
}

int search(char** input_codes, const char* original_query, size_t n_input_codes, size_t query_split, s_vec *v, s_vec* out_vec) {
  int matches = 0;
  for (size_t i = 0; i < v->size; i++) {
    s_entry* e = (s_entry *)vec_at(v, i);
    char *og = e->original;
    char *temp_og = malloc(strlen(og) + 1);
    memcpy(temp_og, og, strlen(og) + 1);
    size_t match_count = 0;

    if (query_split > e->n_codes) {
      continue;
    }

    convert_to_lower(temp_og);
    // int prefix_matched = starts_with(temp_og , original_query);

    // check first if the input codes is not yet in the out_res
    // if not then push it to the res_vec
    for (size_t j = 0; j < e->n_codes; j++) {
      for (size_t c = 0; c < n_input_codes; c++) {
        if (strcmp(e->codes[j], input_codes[c]) == 0 ||
            starts_with(e->tokens[j], original_query) != 0) {
          match_count++;
          break;
        }
        // } else {
        //   // this is so that for example when two words like dice_co and search is 'c'
        //   // then dice == c would deduct to the match_count
        //   // which would then in turn not pass the check to add to the res_vec
        //   // sooo it has to be all input codes to match or else it doesnt get put in results
        //   match_count--;
        // }
      }
    }

    // if (prefix_matched || match_count == n_input_codes) {
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
    free(temp_og);
  }
  return matches;
}
