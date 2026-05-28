#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "platform.h"

void platform_open_editor(const char* filepath) {
    const char* editor = getenv("EDITOR");
    if (!editor) editor = getenv("VISUAL");
    if (!editor) editor = "nano";
    execlp(editor, editor, filepath, NULL);
}