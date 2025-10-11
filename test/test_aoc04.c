#include "unity.h"
#include "../src/aoc04.h"

void setUp(void) {
}

void tearDown(void) {
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

  TEST_ASSERT_EQUAL(expected, actual);
}

void test_aoc04_returns_correct_results(void) {
  TEST_ASSERT_EQUAL(2358, find_aoc04());
}

int main(void) {
  UNITY_BEGIN();

  RUN_TEST(test_count_xmas_returns_correct_value);
  RUN_TEST(test_aoc04_returns_correct_results);

  return UNITY_END();
}
