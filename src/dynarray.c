#include "soundex.h"
#include <stdlib.h>
#include <string.h>


static int vec_grow(s_vec* v) {
    size_t new_cap = 0;

    if (v->cap) new_cap = v->cap * 2;
    else new_cap = 8;

    void *p = malloc(new_cap * v->elem);
    if (!p) return -1;
    if (v->data) {
        (void)memcpy(p, v->data, v->size * v->elem);
        free(v->data);
    }
    v->data = p;
    v->cap = new_cap;
    return 0;
}

int vec_init(s_vec* v, size_t elem_size) {
    v->data = NULL;
    v->size = 0;
    v->cap = 0;
    v->elem = elem_size;

    return 0;
}
void* vec_at(s_vec* v, size_t i) {
    void* s = (char*)v->data + i * v->elem;
    return s;
}

static void entry_free(s_entry* e) {
    for (size_t i = 0; i < e->n_codes; i++) {
        free(e->codes[i]);
        free(e->tokens[i]);
    }
    free((char*)e->codes);
    free((char*)e->original);
    free((char*)e->tokens);
}
void vec_free(s_vec* v) {
    for (size_t i = 0; i < v->size; i++) {
        s_entry* e = (s_entry*)vec_at(v, i);
            entry_free(e);
    }
    free(v->data);
    v->data = NULL;
    v->size = 0;
    v->cap = 0;
    v->elem = 0;
}

int vec_push(s_vec* v, const void* elem) {
    if (v->size >= v->cap && vec_grow(v) != 0) {
        return -1;
    }

    (void)memcpy((char*)v->data + v->size * v->elem, elem, v->elem);
    v->size++;
    return 0;
}



