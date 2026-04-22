#include "soundex.h"
#include <string.h>
#include <stdio.h>

static int count_common(char *str1, size_t len1, char *str2,
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

  int count = 0;

  for (size_t i = 0; i < len2; i++) {
    if (exists_in_strings[(unsigned char)str2[i]] == 1) {
      exists_in_strings[(unsigned char)str2[i]] = 2;
      count++;
    }
  }

  return count;
}

static void push_score(s_entry* out, double value) {
    out->score = value;
}

void get_dice_score(s_entry* out, char *input, char *og) {
  // do some dice coefficient math and set the score to it
    double dice = 0;
  // check if strings are literally the same
  if (strlen(input) == strlen(og)) {
    if (strncmp(input, og, strlen(og)) == 0) {
      dice = 1;
      push_score(out, dice);
      return;
    }
  }

  double sum = (double)(strlen(input) + strlen(og));

  double common = (double)count_common(input, strlen(input), og, strlen(og));
  if (common == -1) {
    err_set_parse();
    printf("Null strings\n");
    return;
  }

  dice = (2.0 * common) / sum;
  if (dice > out->score) {
    push_score(out, dice);
  }

  return;
}