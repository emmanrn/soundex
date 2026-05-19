#include <ncurses.h>
#include <stdlib.h>

#include "soundex.h"
#include "tty.h"

int main() {
    // init the vectors
    s_vec sounds;
    
    vec_init(&sounds, sizeof(s_entry));
  
    tty_run(&sounds);
   
    vec_free(&sounds);

}
