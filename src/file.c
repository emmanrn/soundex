#define _DEFAULT_SOURCE
#include "soundex.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <dirent.h>

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

static int push_str(s_vec* out, char* value) {
    size_t len = strlen(value) + 1;

    char* tmp = malloc(len);
    strncpy(tmp, value, len);
    
    size_t n_codes = 0;

    // count number of separated words
    char* token = strtok(tmp, "_-.");
    while (token != NULL) {
        n_codes++;
        token = strtok(NULL, "_-.");
    }

    free(tmp);
    
    s_entry e = {
        .original = malloc(len),
        .tokens = malloc(sizeof(char*) * n_codes),
        .codes = malloc(sizeof(char*) * n_codes),
        .n_codes = n_codes,
        .score = 0.0
    };

    // if failed to allocate memory
    if (!e.original || !e.codes) {
        free(e.original);
        free(e.tokens);
        free(e.codes);
        err_set_parse();
        return -1;
    }

    strncpy(e.original, value, len);

    //copy words into the codes array
    char* tmp2 = malloc(len);
    strncpy(tmp2, value, len);
    convert_to_lower(tmp2);

    token = strtok(tmp2, "_-.");
    size_t i = 0;
    while (token != NULL && i < n_codes) {
        e.codes[i] = malloc(strlen(token) + 1);
        e.tokens[i] = malloc(strlen(token) + 1);
        strncpy(e.codes[i], token, strlen(token) + 1);
        strncpy(e.tokens[i], token, strlen(token) + 1);
        token = strtok(NULL, "_-.");
        i++;
    }
    free(tmp2);

    return vec_push(out, &e);
}

int read_and_get_names(s_vec* out_sounds, const char* file_name) {
    FILE* file = fopen(file_name, "r");

    if (file == NULL) {
        err_set_parse();
        return -1;
    }

    size_t line = 0;

    while(!feof(file) && !ferror(file)) {
        char s[MAX_LEN];
        if (fgets(s, MAX_LEN, file) != NULL) {
            push_str(out_sounds, s);
            line++;
        }
    }


    return 0;
}

void read_files(s_vec* out_sounds, const char* dirname) {
    (void)out_sounds;
    DIR* dir = opendir(dirname);

    if (dir == NULL) {
        err_set_parse();
        return;
    }

    struct dirent* entry;
    entry = readdir(dir);
    while (entry != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0 || strcmp(entry->d_name, ".git") == 0) {
            entry = readdir(dir);
            continue;
        }
    
        if (entry->d_type == DT_DIR) {
            char path[100] = { 0 };
            strncat(path, dirname, strlen(path) + strlen(dirname));
            strncat(path, "/", strlen(path) + 1);
            strncat(path, entry->d_name, strlen(path) + strlen(entry->d_name));
            read_files(out_sounds, path);
        }

        if (entry->d_type == DT_REG) {
            push_str(out_sounds, entry->d_name);
        }
        entry = readdir(dir);
    }

    closedir(dir);
}
