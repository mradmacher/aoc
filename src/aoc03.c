#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "aoc03.h"

regex_t rx;
regex_t dorx;
regex_t dontrx;

int find_numbers(int *numbers, char *string, regex_t *rx) {
  size_t nmatch = 3;
  regmatch_t pmatch[3];
  int offset = 0;

  int result = regexec(rx, string, nmatch, pmatch, 0);
  if (result == 0) {
    offset = pmatch[0].rm_eo;
    for (int i = 1; i < nmatch; i++) {
      int len = pmatch[i].rm_eo - pmatch[i].rm_so;

      char buf[12];
      memcpy(buf, &string[pmatch[i].rm_so], len);
      buf[len] = '\0';
      numbers[i-1] = atoi(buf);
    }
  } else {
    offset = -1;
  }
  return offset;
}

void clear(char *string, enum op_mode *mode) {
  regex_t *moderx;
  regmatch_t pmatch[1];

  if (*mode == DO) {
    moderx = &dontrx;
  } else {
    moderx = &dorx;
  }

  while (string[0] != '\0') {
    int result = regexec(moderx, string, 1, pmatch, 0);

    int pos = 0;
    if (result == 0) {
      pos = pmatch[0].rm_eo;
      if (*mode == DO) {
        *mode = DONT;
      } else {
        *mode = DO;
      }
    } else {
      pos = strlen(string);
    }

    if (moderx == &dorx) {
      for (int i = 0; i < pos-4; i++) {
        string[i] = ' ';
      }
      moderx = &dontrx;
    } else {
      moderx = &dorx;
    }
    string += pos;
  }
}

unsigned long sum_mul(char *string, enum op_mode *mode) {
  unsigned int result = 0;

  if (*mode != ALL) {
    clear(string, mode);
  }
  printf("%s\n", string);
  int offset = 0;
  while (offset > -1) {
    string = string + offset;

    unsigned int numbers[] = {0, 0};
    offset = find_numbers(numbers, string, &rx);
    if (offset > -1) {
      result += numbers[0] * numbers[1];
    }
  }

  return result;
}

int find_aoc03_result(int step) {
  if (regcomp(&rx, MUL_REGEX, REG_EXTENDED) != 0) {
    printf("Compilation error 1.");
    return 1;
  }
  if (regcomp(&dorx, DO_REGEX, REG_EXTENDED) != 0) {
    printf("Compilation error 2.");
    return 2;
  }
  if (regcomp(&dontrx, DONT_REGEX, REG_EXTENDED) != 0) {
    printf("Compilation error 3.");
    return 3;
  }

  FILE *fptr;
  size_t size = 10000;
  char string[10000];
  enum op_mode mode;
  if (step == 1) {
    mode = ALL;
  } else {
    mode = DO;
  }

  fptr = fopen("input/03", "r");
  unsigned int result = 0;
  while (fgets(string, size, fptr)) {
    result += sum_mul(string, &mode);
  }
  printf("Result: %d\n", result);

  fclose(fptr);

  regfree(&rx);
  regfree(&dorx);
  regfree(&dontrx);

  return result;
}
