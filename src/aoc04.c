#include <assert.h>
#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 200

int xmascmp(char *str) {
  if (strcmp("XMAS", str) == 0 || strcmp("SAMX", str) == 0) {
    return 0;
  }
  return -1;
}

int x_mascmp(char *str) {
  if (strcmp("MAS", str) == 0 || strcmp("SAM", str) == 0) {
    return 0;
  }
  return -1;
}

int count_xmas(const char data[BUFFER_SIZE][BUFFER_SIZE], size_t size) {
  int count = 0;
  const int len = 4; //xmas
  char buf[5];
  buf[len] = '\0';

  for (int row = 0; row < size; row++) {
    for (int j = 0; j < size; j++) {

      if (j <= size - len) {
        for (int n = 0; n < len; n++) {
          buf[n] = data[row][j + n];
        }
        if (xmascmp(buf) == 0) {
          count++;
        }
      }

      if (row <= size - len) {
        for (int n = 0; n < len; n++) {
          buf[n] = data[row + n][j];
        }
        if (xmascmp(buf) == 0) {
          count++;
        }

        if (j <= size - len) {
          for (int n = 0; n < len; n++) {
            buf[n] = data[row + n][j + n];
          }
          if (xmascmp(buf) == 0) {
            count++;
          }
        }

        if (j >= len - 1) {
          for (int n = 0; n < len; n++) {
            buf[n] = data[row + n][j - n];
          }
          if (xmascmp(buf) == 0) {
            count++;
          }
        }
      }
    }
  }
  return count;
}

int count_x_mas(const char data[BUFFER_SIZE][BUFFER_SIZE], size_t size) {
  int count = 0;
  const int len = 3; //mas
  char buf1[4];
  char buf2[4];
  buf1[len] = '\0';
  buf2[len] = '\0';

  for (int row = 0; row < size - len + 1; row++) {
    for (int j = len - 2; j < size - len + 2; j++) {
      for (int n = 0; n < len; n++) {
        buf1[n] = data[row + n][j - 1 + n];
      }
      for (int n = 0; n < len; n++) {
        buf2[n] = data[row + n][j + 1 - n];
      }
      if (x_mascmp(buf1) == 0 && x_mascmp(buf2) == 0) {
        count++;
      }
    }
  }
  return count;
}

int find_aoc04(char *input, int step) {
  FILE *fptr;
  size_t size = 0;
  char matrix[BUFFER_SIZE][BUFFER_SIZE] = { { '\0' } };

  char buf[1000];

  fptr = fopen(input, "r");
  while (fgets(buf, 1000, fptr)) {
    int len = strlen(buf);
    memcpy(matrix[size], buf, len - 1);
    matrix[size][len - 1] = '\0';
    size++;
  }
  fclose(fptr);

  if (step == 1) {
    return count_xmas(matrix, size);
  }

  return count_x_mas(matrix, size);
}

void test_count_xmas_returns_correct_value(void) {
  const char example[BUFFER_SIZE][BUFFER_SIZE] = {
    "MMMSXXMASM",
    "MSAMXMSMSA",
    "AMXSXMAAMM",
    "MSAMASMSMX",
    "XMASAMXAMM",
    "XXAMMXXAMA",
    "SMSMSASXSS",
    "SAXAMASAAA",
    "MAMMMXMMMM",
    "MXMXAXMASX",
  };
  int expected = 18;
  int actual = count_xmas(example, 10);

  assert(expected == actual);
}

void test_count_x_mas_returns_correct_value_for_one_x(void) {
  const char example[BUFFER_SIZE][BUFFER_SIZE] = {
    "M.S",
    ".A.",
    "M.S",
  };
  int expected = 1;
  int actual = count_x_mas(example, 3);

  assert(expected == actual);
}

void test_count_x_mas_returns_correct_value(void) {
  const char example[BUFFER_SIZE][BUFFER_SIZE] = {
    ".M.S......",
    "..A..MSMS.",
    ".M.S.MAA..",
    "..A.ASMSM.",
    ".M.S.M....",
    "..........",
    "S.S.S.S.S.",
    ".A.A.A.A..",
    "M.M.M.M.M.",
    "..........",
  };
  int expected = 9;
  int actual = count_x_mas(example, 10);

  assert(expected == actual);
}

void print_result(char *input) {
  printf("%d\n", find_aoc04(input, 1));
  printf("%d\n", find_aoc04(input, 2));
}

void main(int argc, char **argv) {
  test_count_xmas_returns_correct_value();
  test_count_x_mas_returns_correct_value();

  if (argc == 2) print_result(argv[1]);
}
