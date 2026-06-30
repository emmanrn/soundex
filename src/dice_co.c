#include <string.h>
#include <stdio.h>

#include "soundex.h"
#include "types.h"

static u16 count_common(char *str1, size_t len1, char *str2,
                        size_t len2) {
  if (str1 == NULL || str2 == NULL) {
    return -1;
  }

  // in here we like use the ascii values as the indexes and just increase the
  // index based on the ascii value that was common lets say we found 'A', 'A'
  // in ascii is 65 -> exists_in_strings['A'] = 1 to mark we found this
  // character
  int exists_in_strings[256] = {0};

  for (size_t i = 0; i < len1; i++) {
    exists_in_strings[(unsigned char)str1[i]] = 1;
  }

  u16 count = 0;

  for (size_t i = 0; i < len2; i++) {
    if (exists_in_strings[(unsigned char)str2[i]] == 1) {
      exists_in_strings[(unsigned char)str2[i]] = 2;
      count++;
    }
  }

  return count;
}


f64 get_dice_score(char *input, char *og) {
  // do some dice coefficient math and set the score to it
  // check if strings are literally the same
  if (strcmp(input, og) == 0) {
    return 1.0;
  }

  f64 sum = (f64)(strlen(input) + strlen(og));

  f64 common = (f64)count_common(input, strlen(input), og, strlen(og));
  if (common == -1) {
    return -1.0;
  }

  return (2.0 * common) / sum;
}