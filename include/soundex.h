#ifndef SOUNDEX_H
#define SOUNDEX_H

#include <stdint.h>
#include <stddef.h>

#define MAX_LINES 256
#define MAX_LEN 256

// error handling
void err_set_parse();
void err_print_and_clear();

//simple dynamic array
typedef struct s_vec {
    void* data;
    size_t size;
    size_t cap;
    size_t elem;
} s_vec;

typedef struct s_entry {
    char* original;
    char** tokens;
    char** codes;
    size_t n_codes;
    double score;
} s_entry;

int vec_init(s_vec* v, size_t elem_size);
int vec_push(s_vec* v, const void* elem);
void* vec_at(s_vec* v, size_t i);
void vec_free(s_vec* v);

// file handling
int read_and_get_names(s_vec* v, const char* file_name);
void read_files(s_vec* v, const char* dirname);



// soundex: return 0 on success, non-zero: error
int soundex(s_vec* out_sounds);
int soundex_str(char* query);

// fuzzy search: return match_count if matches found, negative: error, 0 if no match
int search(char** input_codes, const char* original_query, size_t n_input_codes, size_t query_split, s_vec* v, s_vec* out_vec);

// dice coefficient
double get_dice_score(char* input, char* og);

//merge sort
void topDownMergeSort(s_vec* a, s_vec* b, size_t len);



#endif /* SOUNDEX_H */