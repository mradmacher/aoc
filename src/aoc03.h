#include <regex.h>

#ifndef AOC03_H
#define AOC03_H

#define MUL_REGEX "mul\\(([0-9]{1,3}),([0-9]{1,3})\\)"
#define DO_REGEX "do\\(\\)"
#define DONT_REGEX "don't\\(\\)"

enum op_mode {
  DONT, DO, ALL
};

int find_numbers(int *numbers, char *string, regex_t *rx);
int find_aoc03_result(int step);

#endif // AOC03_H
