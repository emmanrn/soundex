#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "soundex.h"

// Copy a section of the array a into array b (from begin to end - 1)
static void copyArray(s_vec* a, size_t begin, size_t end, s_vec* b) {
    for (size_t k = begin; k < end; k++) {
        s_entry* ea = (s_entry*)vec_at(a, k);

        if (vec_push(b, ea) == -1) {
            err_set_parse();
            return;
        }
    }
}
// Merge two sorted halves (from a) into a single sorted run (into b)
static void topDownMerge(s_vec* a, size_t begin, size_t middle, size_t end, s_vec* b) {
    size_t i = begin;
    size_t j = middle;

    for (size_t k = begin; k < end; k++) {
        s_entry* ea1 = (s_entry*)vec_at(a, i);
        s_entry* ea2 = (s_entry*)vec_at(a, j);
        s_entry* eb = (s_entry*)vec_at(b, k);

        if (i < middle && (j >= end || ea1->score >= ea2->score)) {
            *eb = *ea1;
            i++;
        } else {
            *eb = *ea2;
            j++;
        }
    }
}

// Split the array a into two halves, sort both halves into b,
// and merge the sorted halves back into a
static void topDownSplitMerge(s_vec* a, size_t begin, size_t end, s_vec* b) {
    if (end - begin <= 1) {
        return; // Base case: run size is 1, so it's already sorted
    }

    size_t middle = (begin + end) / 2;

    topDownSplitMerge(b, begin, middle, a);
    topDownSplitMerge(b, middle, end, a);

    topDownMerge(b, begin, middle, end, a);
}


void topDownMergeSort(s_vec* a, s_vec* b, size_t len) {
    copyArray(a, 0, len, b);
    topDownSplitMerge(a, 0, len, b);
}