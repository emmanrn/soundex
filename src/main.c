#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <ncurses.h>


#include "soundex.h"
#include "tty.h"
// static void print_file_res(s_entry* s) {
//         printf("RESULT ORIGINAL: %s\n", s->original);
//         printf("RESULT CONVERTED: %s\n", s->converted);
// }



// splits separated queries into tokens, and returns number of splits
// static size_t split_query(const char* query, char*** out_split) {
//     size_t len = strlen(query) + 1;
//     char* tmp = malloc(len);
//     strncpy(tmp, query, len);

//     size_t n_split = 0;
    
//     char* token = strtok(tmp, "_-.");
//     while (token != NULL) {
//         n_split++;
//         token = strtok(NULL, "_-.");
//     }

//     free(tmp);


//     *out_split = malloc(sizeof(char*) * n_split);

//     char* tmp2 = malloc(len);
//     strncpy(tmp2, query, len);

//     token = strtok(tmp2, "_-.");
//     size_t i = 0;
//     while (token != NULL && i < n_split) {
//         (*out_split)[i] = malloc(strlen(token) + 1);
//         strncpy((*out_split)[i], token, strlen(token) + 1);
//         token = strtok(NULL, "_-.");
//         i++;
//     }

//     free(tmp2);

//     return n_split;
// }

// static int fuzzy_search(const char* query, s_vec* sounds, s_vec* res_vec, s_vec* temp) {
//     char** split = NULL;
//     size_t n_split = split_query(query, &split);

//     char** original_split = NULL;
//     size_t n_original_split = split_query(query, &original_split);

//     // read_and_get_names(sounds, "names.txt");
//     read_files(sounds, ".");

//     soundex(sounds);
//     for (size_t i = 0; i < n_split; i++) {
//         soundex_str(split[i]);
//     }

//     int matches = 0;

//     // looping through each string in the query
//     matches = search(split, query, n_split, n_original_split, sounds, res_vec);

//     for (size_t i = 0; i < res_vec->size; i++) {
//         double total = 0.0;
//         s_entry* e = (s_entry*)vec_at(res_vec, i);

//             for (size_t c = 0; c < n_original_split; c++) {
//                 double best = 0.0;
//                 size_t j = 0;
//                 while (j < e->n_codes) {
//                     double d = get_dice_score(original_split[c], e->tokens[j]);
//                     if (d > best) best = d;
//                     j++;
//                 }
//                 total += best;
//             }

//         double penalty = (double)n_original_split / (double)e->n_codes;
//         e->score = (total / (double)n_original_split) * penalty;
//     }

//     topDownMergeSort(res_vec, temp, res_vec->size);

//     return matches;
// }


int main(int argc, char* argv[]) {

    // if (argc != 2) {
    //     err_set_parse();
    //     err_print_and_clear();
    //     return 1;
    // }
    (void)argc;
    (void)argv;
    

    // init the vectors
    s_vec sounds;
    // s_vec res_vec;
    // s_vec temp;
    vec_init(&sounds, sizeof(s_entry));
    // vec_init(&res_vec, sizeof(s_entry));
    // vec_init(&temp, sizeof(s_entry));
    tty_run(&sounds);

    // // start fuzzy searching
    // int match_count = fuzzy_search(argv[1], &sounds, &res_vec, &temp);

    // if (match_count > 0) {
    //     printf("'%d' Matches Found for '%s' ", match_count, argv[1]);

    //     printf("\n");

    //     for (int i = 0; i < match_count; i++) {
    //         s_entry* e = (s_entry*)vec_at(&res_vec, i);
    //         printf("%s\n", e->original);
    //     }
    // } else if (match_count == 0) {
    //     printf("no match found for '%s'\n", argv[1]);
    // }


    // vec_free(&sounds);
    // vec_free(&res_vec);
    // free(temp.data);
}


