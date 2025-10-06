#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

unsigned long sum_mul(char *string) {
  regex_t rx;
  int value = regcomp(&rx, "mul\\(([0-9]{1,3}),([0-9]{1,3})\\)", REG_EXTENDED);
  if (value != 0) {
    printf("Compilation error.");
  }

  unsigned int result = 0;

  int offset = 0;
  while (offset > -1) {
    string = string + offset;

    unsigned int numbers[] = {0, 0};
    offset = find_numbers(numbers, string, &rx);
    if (offset > -1) {
      result += numbers[0] * numbers[1];
    }
  }

  regfree(&rx);
  return result;
}

int main() {
  FILE *fptr;
  size_t size = 10000;
  char *string = malloc(size);

  fptr = fopen("input/03", "r");
  unsigned int result = 0;
  while (fgets(string, size, fptr)) {
    result += sum_mul(string);
  }
  printf("Result: %d\n", result);

  fclose(fptr);
  free(string);
}
