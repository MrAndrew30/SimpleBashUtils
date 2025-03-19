#include "s21_grep.h"

int scanArguments(int argc, char *argv[], struct ArgumentsOfGrep *arguments,
                  char patterns[8192]);
void getstr(FILE *fp, char *str);
void printFile(int argc, struct ArgumentsOfGrep *arguments, FILE *fp,
               char line[4096], char *file, char patterns[8192]);
int goodString(struct ArgumentsOfGrep *arguments, char line[4096],
               char patterns[8192]);
void sFlag(int argc, struct ArgumentsOfGrep *arguments, char *file);

int main(int argc, char *argv[]) {
  struct ArgumentsOfGrep arguments = {0};
  arguments.startIndexFilenames = 1;
  char patterns[8192];
  patterns[0] = '\0';
  scanArguments(argc, argv, &arguments, patterns);
  char line[4096] = "1";
  int check = 1;
  if (arguments.startIndexFilenames >= argc) {
    fprintf(stderr, "Error");
  }
  for (int i = arguments.startIndexFilenames; i < argc && check == 1; i++) {
    char *file = argv[i];
    FILE *fp = fopen(file, "r");
    if (fp == NULL && arguments.s == 0) {
      fprintf(stderr, "Error with  file\n");
    } else if (fp != NULL) {
      printFile(argc, &arguments, fp, line, file, patterns);
    }
    if (fp != NULL) {
      fclose(fp);
    }
  }
  return 0;
}

void printFile(int argc, struct ArgumentsOfGrep *arguments, FILE *fp,
               char line[4096], char *file, char patterns[8192]) {
  int numberOfStr = 1;
  int flag = 1;
  int countOfMatches = 0;
  while (strlen(line) != 0 || flag == 1) {
    flag = 0;
    getstr(fp, line);
    if (strlen(line) != 0 && line[strlen(line) - 1] != '\n') {
      strcat(line, "\n");
    }
    if (goodString(arguments, line, patterns) && strlen(line) != 0) {
      countOfMatches += 1;
      if (arguments->c != 1 && arguments->l != 1) {
        sFlag(argc, arguments, file);
      }
      if (arguments->n == 1 && arguments->c != 1 && arguments->l != 1) {
        printf("%d:", numberOfStr);
      }
      if (arguments->c != 1 && arguments->l != 1) {
        printf("%s", line);
      }
    }
    numberOfStr += 1;
  }
  if (arguments->l == 1 && countOfMatches > 0) {
    printf("%s\n", file);
  }
  if (arguments->c == 1 && arguments->l != 1) {
    sFlag(argc, arguments, file);
    printf("%d\n", countOfMatches);
  }
}

void sFlag(int argc, struct ArgumentsOfGrep *arguments, char *file) {
  if (arguments->h != 1 && argc - arguments->startIndexFilenames > 1) {
    printf("%s:", file);
  }
}

int goodString(struct ArgumentsOfGrep *arguments, char line[4096],
               char patterns[8192]) {
  int res = 0;
  char *tmp = (char *)calloc(4096, sizeof(char));
  strcpy(tmp, line);
  if (strlen(tmp) > 0 && tmp[strlen(tmp) - 1] == '\n') {
    tmp[strlen(tmp) - 1] = '\0';
  }
  regex_t regex;
  if (regcomp(&regex, patterns, arguments->i ? REG_ICASE : 0) == 0) {
    if (regexec(&regex, tmp, 0, NULL, 0) == 0) {
      res = 1;
    }
    regfree(&regex);
  }
  if (arguments->v == 1) {
    res = (res + 1) % 2;
  }
  free(tmp);
  return res;
}

int scanArguments(int argc, char *argv[], struct ArgumentsOfGrep *arguments,
                  char patterns[8192]) {
  int res = 1;
  int index = 1;
  for (int chr; (chr = getopt(argc, argv, "e:ivclnhsf:o")) != -1 && res == 1;) {
    if (chr == 'e') {
      arguments->e = 1;
      if (strlen(patterns) != 0) {
        strcat(patterns, "\\|");
        strcat(patterns, optarg);
      } else {
        strcpy(patterns, optarg);
      }
      arguments->startIndexFilenames += 1;
    } else if (chr == 'i')
      arguments->i = 1;
    else if (chr == 'v')
      arguments->v = 1;
    else if (chr == 'c')
      arguments->c = 1;
    else if (chr == 'l')
      arguments->l = 1;
    else if (chr == 'n')
      arguments->n = 1;
    else if (chr == 'h')
      arguments->h = 1;
    else if (chr == 's')
      arguments->s = 1;
    else if (chr == 'f') {
      arguments->f = 1;
      arguments->startIndexFilenames += 1;
    } else if (chr == 'o')
      arguments->o = 1;
    else
      res = 0;
    if ((strlen(argv[index]) == 2 && argv[index][0] == '-') ||
        (strlen(argv[index]) > 2 && chr == argv[index][1])) {
      arguments->startIndexFilenames += 1;
      index += 1;
    }
    if (chr == 'e' || chr == 'f') {
      index += 1;
    }
  }
  if (arguments->e == 0 && res == 1) {
    strcpy(patterns, argv[arguments->startIndexFilenames]);
    arguments->startIndexFilenames += 1;
  }
  return res;
}

void getstr(FILE *fp, char *str) {
  char chr;
  int len = 0;
  int flag = 1;
  while (flag == 1 && (chr = getc(fp)) != EOF) {
    str[len] = chr;
    len += 1;
    if (chr == '\n') {
      flag = 0;
    }
  }
  str[len] = '\0';
}