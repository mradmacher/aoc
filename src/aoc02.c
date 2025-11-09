#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define BUFFER_SIZE 1000
#define MAX_LEN 100

enum tolerance {
  NO, YES
};

size_t split_line(char *input, int *buffer) {
  char buf[MAX_LEN];
  strcpy(buf, input);
  char *sub = strtok(buf, " ");
  int count = 0;
  while (sub != NULL) {
    buffer[count] = atoi(sub);
    sub = strtok(NULL, " ");
    count++;
  }

  return count;
}

int find_error_index(int *levels, int len) {
    int sign = 0;
    for (int i = 1; i < len; i++) {
      int diff = 0;
      int new_sign = 0;

      if (levels[i - 1] > levels[i]) {
          new_sign = -1;
          diff = levels[i - 1] - levels[i];
      } else {
          new_sign = 1;
          diff = levels[i] - levels[i - 1];
      }
      if (sign == 0) sign = new_sign;

      if ((new_sign != sign) || (diff == 0) || (diff > 3)) {
        return i;
      }
    }
    return -1;
}

void copy_without_index(int *buffer, int *levels, int len, int index) {
  int j = 0;
  for (int i = 0; i < len; i++) {
    if (i != index) {
      buffer[j] = levels[i];
      j++;
    }
  }
}

int is_safe(int *levels, int len, enum tolerance t) {
  int index = -1;

  index = find_error_index(levels, len);
  if (index == -1) {
    return 1;
  } else if (t == YES) {
    int buffer[100];
    int new_index = -1;
    for (int i = 0; i < len; i++) {
      copy_without_index(buffer, levels, len, i);
      new_index = find_error_index(buffer, len - 1);

      if (new_index == -1) {
          return 1;
      }
    }
  }
  return 0;
}

int count_safe(size_t rows, char lines[rows][MAX_LEN], enum tolerance t) {
  int buffer[MAX_LEN];
  int count = 0;
  for (int i = 0; i < rows; i++) {
    size_t cols = split_line(lines[i], buffer);
    if (is_safe(buffer, cols, t)) {
        count += 1;
    }
  }
  return count;
}


void test_split_line() {
  char input[] = "7 6 4 2 1";
  int expected[] = { 7, 6, 4, 2, 1 };
  int result[5];
  size_t count = split_line(input, result);

  assert(5 == count);
  for (int i = 0; i < count; i++) {
    assert(expected[i] == result[i]);
  }
}

void test_is_safe_without_tolerance() {
  int given[6][5] = {
    { 7, 6, 4, 2, 1 },
    { 1, 2, 7, 8, 9 },
    { 9, 7, 6, 2, 1 },
    { 1, 3, 2, 4, 5 },
    { 8, 6, 4, 4, 1 },
    { 1, 3, 6, 7, 9 },
  };

  assert(is_safe(given[0], 5, NO));
  assert(!is_safe(given[1], 5, NO));
  assert(!is_safe(given[2], 5, NO));
  assert(!is_safe(given[3], 5, NO));
  assert(!is_safe(given[4], 5, NO));
  assert(is_safe(given[5], 5, NO));
}

void test_is_safe_with_tolerance() {
  int given[7][5] = {
    { 7, 6, 4, 2, 1 },
    { 1, 2, 7, 8, 9 },
    { 9, 7, 6, 2, 1 },
    { 1, 3, 2, 4, 5 },
    { 8, 6, 4, 4, 1 },
    { 1, 3, 6, 7, 9 },
    { 5, 3, 6, 7, 9 },
  };

  assert(is_safe(given[0], 5, YES));
  assert(!is_safe(given[1], 5, YES));
  assert(!is_safe(given[2], 5, YES));
  assert(is_safe(given[3], 5, YES));
  assert(is_safe(given[4], 5, YES));
  assert(is_safe(given[5], 5, YES));
  assert(is_safe(given[6], 5, YES));
}

void test_part_one() {
  char given[6][MAX_LEN] = {
    "7 6 4 2 1",
    "1 2 7 8 9",
    "9 7 6 2 1",
    "1 3 2 4 5",
    "8 6 4 4 1",
    "1 3 6 7 9",
  };

  int result = count_safe(6, given, NO);
  assert(2 == result);
}

void test_part_two() {
  char given[6][MAX_LEN] = {
    "7 6 4 2 1",
    "1 2 7 8 9",
    "9 7 6 2 1",
    "1 3 2 4 5",
    "8 6 4 4 1",
    "1 3 6 7 9",
  };

  int result = count_safe(6, given, YES);
  assert(4 == result);
}

void run_tests() {
  test_split_line();
  test_is_safe_without_tolerance();
  test_is_safe_with_tolerance();
  test_part_one();
  test_part_two();
}

void print_results(char *input) {
  char string[MAX_LEN];
  char report[BUFFER_SIZE][MAX_LEN];

  FILE *fptr;
  fptr = fopen(input, "r");
  size_t size = 0;
  while (fgets(string, MAX_LEN, fptr)) {
    size_t len = strlen(string);
    memcpy(report[size], string, len - 1);
    report[size][len - 1] = '\0';
    size++;
  }
  fclose(fptr);

  printf("Safe levels: %d\n", count_safe(size, report, NO));
  printf("Safe levels with tolerance: %d\n", count_safe(size, report, YES));

}

void main(int argc, char **argv) {
  run_tests();

  if (argc == 2) print_results(argv[1]);
}
