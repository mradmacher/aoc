#include <stdio.h>
#include <string.h>
#include "aoc04.h"

int xmascmp(char *str) {
  if (strcmp("XMAS", str) == 0 || strcmp("SAMX", str) == 0) {
    return 0;
  }
  return -1;
}

int count_xmas(const char data[BUFFER_SIZE][BUFFER_SIZE], size_t size) {
  int count = 0;
  const int len = 4;
  char buf[5];

  for (int row = 0; row < size; row++) {
    for (int j = 0; j <= size - len; j++) {
      for (int n = 0; n < len; n++) {
        buf[n] = data[row][j + n];
      }
      buf[len] = '\0';
      if (xmascmp(buf) == 0) {
        count++;
      }
    }
    if (row + len <= size) {
      for (int j = 0; j < size; j++) {
        for (int n = 0; n < len; n++) {
          buf[n] = data[row + n][j];
        }
        buf[len] = '\0';
        if (xmascmp(buf) == 0) {
          count++;
        }
      }

      for (int j = 0; j <= size - len; j++) {
        for (int n = 0; n < len; n++) {
          buf[n] = data[row + n][j + n];
        }
        buf[len] = '\0';
        if (xmascmp(buf) == 0) {
          count++;
        }
      }

      for (int j = len - 1; j < size; j++) {
        for (int n = 0; n < len; n++) {
          buf[n] = data[row + n][j - n];
        }
        buf[len] = '\0';
        if (xmascmp(buf) == 0) {
          count++;
        }
      }
    }
  }
  return count;
}

int find_aoc04(void) {
  FILE *fptr;
  size_t size = 0;
  char matrix[BUFFER_SIZE][BUFFER_SIZE] = { { '\0' } };

  char buf[1000];

  fptr = fopen("input/04", "r");
  while (fgets(buf, 1000, fptr)) {
    int len = strlen(buf);
    memcpy(matrix[size], buf, len - 1);
    matrix[size][len - 1] = '\0';
    size++;
  }
  fclose(fptr);

  return count_xmas(matrix, size);
}
