#include "aoc04.c"

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

void main() {
  int result = count_xmas(example, 10);
  printf("%d\n", result);

  printf("%d\n", find_aoc04());
}
