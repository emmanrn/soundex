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

// fuzzy search: return 0 on found match, negative: error, positive: not found
int search(const char* input, s_vec* v, char** out_res, s_vec* out_vec);

// dice coefficient
void get_dice_score(s_entry* out, char* input, char* og);

//merge sort
void topDownMergeSort(s_vec* a, s_vec* b, size_t len);



#endif /* SOUNDEX_H */