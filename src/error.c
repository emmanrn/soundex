#include "soundex.h"
#include <unistd.h>

static int g_err = 0;

void err_set_parse() {
    if (!g_err) {
        g_err = 1;
    }
}

const char* err_message() {
    if (g_err == 1) return "soundex parse error";

    return "";
}

void err_print_and_clear() {
    const char* m = err_message();

    if (m && *m) {
        size_t len = 0;

        while(m[len]) len++;

        (void)write(2, m, (unsigned int)len);
        (void)write(2, "\n", 1);
    }
}