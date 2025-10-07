#include "unity.h"
#include "../src/aoc03.h"

regex_t mul_regex;

void setUp(void) {
  regcomp(&mul_regex, MUL_REGEX, REG_EXTENDED);
}

void tearDown(void) {
  regfree(&mul_regex);
}

void test_find_numbers_extracts_numbers_from_first_instruction(void) {
  char *instructions = "xmul(2,4)&mul[3,7]!^don't()_mul(5,5)+mul(32,64](mul(11,8)undo()?mul(8,5))";
  int expected[] = { 2, 4 };

  int actual[2];
  find_numbers(actual, instructions, &mul_regex);

  TEST_ASSERT_EQUAL_INT_ARRAY(expected, actual, 2);
}

void test_find_numbers_returns_offset_after_first_instruction(void) {
  char *instructions = "xmul(2,4)&mul[3,7]!^don't()_mul(5,5)+mul(32,64](mul(11,8)undo()?mul(8,5))";
  int numbers[2];
  int expected = 9;

  int actual = find_numbers(numbers, instructions, &mul_regex);

  TEST_ASSERT_EQUAL(expected, actual);
}

void test_find_numbers_returns_nothing_for_empty_string(void) {
  char *instructions = "";
  int numbers[2];
  int expected = -1;

  int actual = find_numbers(numbers, instructions, &mul_regex);

  TEST_ASSERT_EQUAL(expected, actual);
}

void test_find_numbers_returns_nothing_for_string_without_instructions(void) {
  char *instructions = "&mul[3,7]!^don't()_mul[5,5)+mul(32,64](mul<11,8>undo()?mul/8,5/)";
  int numbers[2];
  int expected = -1;

  int actual = find_numbers(numbers, instructions, &mul_regex);

  TEST_ASSERT_EQUAL(expected, actual);
}

void test_aoc03_returns_correct_results(void) {
  TEST_ASSERT_EQUAL(174336360, find_aoc03_result(1));
  TEST_ASSERT_EQUAL(88802350, find_aoc03_result(2));
}

int main(void) {
  UNITY_BEGIN();
  RUN_TEST(test_find_numbers_extracts_numbers_from_first_instruction);
  RUN_TEST(test_find_numbers_returns_offset_after_first_instruction);
  RUN_TEST(test_find_numbers_returns_nothing_for_empty_string);
  RUN_TEST(test_find_numbers_returns_nothing_for_string_without_instructions);
  RUN_TEST(test_aoc03_returns_correct_results);
  return UNITY_END();
}
