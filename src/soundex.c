#include "soundex.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void remove_adjacent(char* s, const char* original) {
    size_t i = 1;

    while (s[i]) {
        // this is for checking adjacent nums
        if (s[i] == s[i - 1]) {
            size_t j = i;

            while (s[j] == s[i]) {
                s[j] = s[j + 1];
                j++;
            }
        } else if (i + 2 < strlen(s) && s[i] == s[i + 2]) {
            if (original[i + 1] == 'h' || original[i + 1] == 'w' || original[i + 1] == 'y') {
                size_t j = i;

                while (s[j] == s[i]) {
                    s[j] = s[j + 1];
                    j++;
                }
            } else {
                i++;
            }
        } else {
            i++;
        }
    }
}

static void remove_zeros(char* s) {
    size_t pos = 0;

    while (s[pos] != '\0') {
        if (s[pos] == '0') {
            size_t newPos = pos;

            while (s[newPos] != '\0') {
                s[newPos] = s[newPos + 1];
                newPos++;
            }
        } else {
            pos++;
        }
    }
}

static void remove_same_digit(char* s) {
    if (s[0] == s[1]) {
        size_t i = 1;
        while (s[i]) {
            s[i] = s[i + 1];
            i++;
        }
    }
}

static void padding(char* s) {
    char zero = '0';
    while (strlen(s) < 4) {
        strncat(s, &zero, 1);
    }

    if (strlen(s) > 4) {
        s[4] = '\0';
    }
}

static void change_to_score(char* s) {
    size_t i = 0;
    // char* s = s;
    while (s[i]) {
        if (!isalpha((unsigned char)s[i])) {
            i++;
            continue;
        }

        if (s[i] == 'b' || s[i] == 'f' || s[i] == 'p' || s[i] == 'v') {
            s[i] = '1';
            i++;
        } else if (s[i] == 'c' || s[i] == 'g' || s[i] == 'j' || 
            s[i] == 'k' || s[i] == 'q' || s[i] == 's' ||
            s[i] == 'x' || s[i] == 'z') {
            s[i] = '2';
            i++;
        } else if (s[i] == 'd' || s[i] == 't') {
            s[i] = '3';
            i++;
        } else if (s[i] == 'l') {
            s[i] = '4';
            i++;
        } else if (s[i] == 'm' || s[i] == 'n') {
            s[i] = '5';
            i++;
        } else if (s[i] == 'r') {
            s[i] = '6';
            i++;
        } else {
            s[i] = '0';
            i++;
        }
    }
}


int soundex(s_vec* out_sounds) {

    for (size_t i = 0; i < out_sounds->size; i++) {
        s_entry* e = (s_entry*)vec_at(out_sounds, i);
        for (size_t j = 0; j < e->n_codes; j++) {
            char* s = e->codes[j];
            const char* og = e->tokens[j];
            char first = toupper(s[0]);
            change_to_score(s);

            remove_adjacent(s, og);

            remove_same_digit(s);
            remove_zeros(s);

            padding(s);

            s[0] = first;
        }
    }

    return 0;
}

int soundex_str(char* query) {
    char* og = malloc(strlen(query) + 1);
    strncpy(og, query, strlen(query) + 1);

    change_to_score(query);

    remove_adjacent(query, og);

    remove_same_digit(query);
    remove_zeros(query);

    padding(query);

    query[0] = toupper(og[0]);

    return 0;
}