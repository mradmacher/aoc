#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 10000
#define RULE_LEN 7
#define RULES_SIZE 100

typedef int rules_t[RULES_SIZE][RULES_SIZE];
typedef char rules_input_t[BUFFER_SIZE][RULE_LEN];

void mark_all_allowed(rules_t rules) {
  for (int i = 0; i < RULES_SIZE; i++) {
    for (int j = 0; j < RULES_SIZE; j++) {
      rules[i][j] = 1;
    }
  }
}

void split(int pages[2], char *string) {
  char buf[3];
  int i = 0;

  while (string[i] != '|') {
    buf[i] = string[i];
    i++;
  }
  buf[i] = '\0';
  pages[0] = atoi(buf);
  string = string + i + 1;

  i = 0;
  while(string[i]) {
    buf[i] = string[i];
    i++;
  }
  buf[i] = '\0';
  pages[1] = atoi(buf);
}

size_t split_update(int update[BUFFER_SIZE], char *string) {
  char buf[3];
  size_t n = 0;
  size_t i = 0;

  if (string[0] == '\0') {
    return 0;
  }

  while (1) {
    if (string[i] == ',' || string[i] == '\0') {
      buf[i] = '\0';
      update[n] = atoi(buf);
      n++;
      if (string[i] == '\0') {
        return n;
      } else {
        string = string + i + 1;
        i = 0;
      }
    } else {
      buf[i] = string[i];
      i++;
    }
  }
}

void apply_rules(rules_t rules, rules_input_t input) {
  size_t i = 0;
  mark_all_allowed(rules);
  while(input[i][0] != '\0') {
    int pages[2];
    split(pages, input[i]);
    rules[pages[1]][pages[0]] = 0;
    i++;
  }
}

// Returns positive number if the order is correct,
// otherwise negative number from corrected collection.
int correct_middle_page(rules_t rules, char *string) {
  int sign = 1;
  int update[BUFFER_SIZE];
  size_t n = split_update(update, string);

  for (int i = 0; i < n - 1; i++) {
    for (int j = i + 1; j < n; j++) {
      if (rules[update[i]][update[j]] == 0) {
        sign = -1;
        int buf = update[i];
        update[i] = update[j];
        update[j] = buf;
        i--;
        break;
      }
    }
  }

  return sign * update[n/2];
}

void test_apply_rules() {
  rules_input_t example = {
    "1|20",
    "21|32",
    "99|9",
  };
  rules_t rules;
  apply_rules(rules, example);
  for (int i = 0; i < RULES_SIZE; i++) {
    for (int j = 0; j < RULES_SIZE; j++) {
      if ((j == 1 && i == 20) || (j == 21 && i == 32) || (j == 99 && i == 9)) {
        assert(rules[i][j] == 0);
      } else {
        assert(rules[i][j] == 1);
      }
    }
  }

}

void test_split() {
  int pages[2];
  split(pages, "47|53");

  assert(pages[0] == 47);
  assert(pages[1] == 53);
}

void test_split_update() {
  int numbers[BUFFER_SIZE];
  int size = 0;

  size = split_update(numbers, "47,53,21");

  assert(size == 3);
  assert(numbers[0] == 47);
  assert(numbers[1] == 53);
  assert(numbers[2] == 21);

  size = split_update(numbers, "1");

  assert(size == 1);
  assert(numbers[0] == 1);

  size = split_update(numbers, "");

  assert(size == 0);
}

void test_correct_middle_page() {
  rules_input_t example_rules = {
    "47|53",
    "97|13",
    "97|61",
    "97|47",
    "75|29",
    "61|13",
    "75|53",
    "29|13",
    "97|29",
    "53|29",
    "61|53",
    "97|53",
    "61|29",
    "47|13",
    "75|47",
    "97|75",
    "47|61",
    "75|61",
    "47|29",
    "75|13",
    "53|13",
  };

  char example_updates[6][50] = {
    "75,47,61,53,29",
    "97,61,53,29,13",
    "75,29,13",
    "75,97,47,61,53",
    "61,13,29",
    "97,13,75,29,47",
  };

  rules_t rules;
  apply_rules(rules, example_rules);

  assert(correct_middle_page(rules, example_updates[0]) == 61);
  assert(correct_middle_page(rules, example_updates[1]) == 53);
  assert(correct_middle_page(rules, example_updates[2]) == 29);
  assert(correct_middle_page(rules, example_updates[3]) == -47);
  assert(correct_middle_page(rules, example_updates[4]) == -29);
  assert(correct_middle_page(rules, example_updates[5]) == -47);
}


void run_tests() {
  test_split();
  test_split_update();
  test_apply_rules();
  test_correct_middle_page();
}

void print_result(char *input) {
  int correct_result = 0;
  int corrected_result = 0;

  rules_t rules;
  rules_input_t rules_input;
  FILE *fptr;
  char line[1000];
  fptr = fopen(input, "r");
  assert(fptr != NULL && "File does not exist");

  int i = 0;
  while (fgets(line, 1000, fptr)) {
    if (line[0] == '\n') {
      break;
    }
    strcpy(rules_input[i], line);
    i++;
  }
  apply_rules(rules, rules_input);

  while (fgets(line, 1000, fptr)) {
    int result = correct_middle_page(rules, line);
    if (result > 0) {
      correct_result += result;
    } else {
      corrected_result += -result;
    }
  }

  fclose(fptr);

  printf("%d\n", correct_result);
  printf("%d\n", corrected_result);
}

void main(int argc, char **argv) {
  run_tests();

  if (argc == 2) print_result(argv[1]);
}
