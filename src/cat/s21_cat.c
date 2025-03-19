#include "s21_cat.h"

int s21_filecheck(FILE *fp);
void s21_catFunction(FILE *fp, struct ArgumentsOfCat arguments,
                     struct VariablesCat *vars);
int scanArguments(int argc, char *argv[], struct ArgumentsOfCat *arguments);
void vFlag(int chr, struct ArgumentsOfCat arguments);

int main(int argc, char *argv[]) {
  struct ArgumentsOfCat arguments = {0};
  struct VariablesCat vars = {1, 1, 1, 0};
  int check = scanArguments(argc, argv, &arguments);
  if (check) {
    FILE *fp;
    for (int i = arguments.startIndexFilenames; i < argc; i++) {
      fp = fopen(argv[i], "r");
      if (s21_filecheck(fp) == 1) {
        s21_catFunction(fp, arguments, &vars);
      }
      fclose(fp);
    }
  } else
    fprintf(stderr, "Error: wrong flags");
  return 0;
}

int scanArguments(int argc, char *argv[], struct ArgumentsOfCat *arguments) {
  int i;
  int res = 1;
  for (i = 1; i < argc && argv[i][0] == '-'; i++) {
    if (strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--squeeze-blank") == 0)
      arguments->s = 1;
    else if (strcmp(argv[i], "-A") == 0) {
      arguments->v = 1;
      arguments->E = 1;
      arguments->T = 1;
    } else if (strcmp(argv[i], "-b") == 0 ||
               strcmp(argv[i], "--number-nonblank") == 0)
      arguments->b = 1;
    else if (strcmp(argv[i], "-e") == 0) {
      arguments->v = 1;
      arguments->E = 1;
    } else if (strcmp(argv[i], "-E") == 0 ||
               strcmp(argv[i], "--show-ends") == 0)
      arguments->E = 1;
    else if (strcmp(argv[i], "-n") == 0 || strcmp(argv[i], "--number") == 0) {
      arguments->n = 1;
    } else if (strcmp(argv[i], "-t") == 0) {
      arguments->v = 1;
      arguments->T = 1;
    } else if (strcmp(argv[i], "-T") == 0 ||
               strcmp(argv[i], "--show-tabs") == 0)
      arguments->T = 1;
    else if (strcmp(argv[i], "-v") == 0 ||
             strcmp(argv[i], "--show-nonprinting") == 0)
      arguments->v = 1;
    else
      res = 0;
  }
  arguments->startIndexFilenames = i;
  if (arguments->b == 1) {
    arguments->n = 0;
  }
  return res;
}

void s21_catFunction(FILE *fp, struct ArgumentsOfCat arguments,
                     struct VariablesCat *vars) {
  int chr;
  int num = vars->num, print_enter = vars->print_enter;
  int counterEnters = vars->counterEnters;
  int enter_is_printed = vars->enter_is_printed;
  while ((chr = getc(fp)) != EOF) {
    if (chr != '\n') {
      counterEnters = 0;
    }
    if (print_enter && (arguments.n == 1 || (arguments.b && chr != '\n')) &&
        !(arguments.s == 1 && counterEnters >= 2)) {
      printf("%6d\t", num);
      print_enter = 0;
      enter_is_printed = 1;
    }
    if (chr == '\n') {
      counterEnters += 1;
      if (arguments.E == 1) {
        if ((counterEnters <= 2 && arguments.s == 1) || arguments.s == 0)
          printf("$\n");
      } else if ((counterEnters <= 2 && arguments.s == 1) || arguments.s == 0)
        printf("\n");
      if (arguments.n == 1 &&
          (arguments.s == 0 || (counterEnters <= 2 && arguments.s == 1))) {
        num += 1;
        print_enter = 1;
      }
      if (arguments.b == 1 && enter_is_printed &&
          (arguments.s == 0 || (counterEnters <= 2 && arguments.s == 1))) {
        num += 1;
        print_enter = 1;
        enter_is_printed = 0;
      }

    } else if (arguments.v == 1) {
      vFlag(chr, arguments);
    } else if (arguments.T == 1 && chr == '\t')
      printf("^I");
    else
      printf("%c", chr);
  }
  vars->counterEnters = counterEnters;
  vars->print_enter = print_enter;
  vars->enter_is_printed = enter_is_printed;
  vars->num = num;
}

int s21_filecheck(FILE *fp) {
  int res = 1;
  if (fp == NULL) {
    res = 0;
    fprintf(stderr, "Error: incorrect file\n");
  }
  return res;
}

void vFlag(int chr, struct ArgumentsOfCat arguments) {
  if (chr == 127)
    printf("^?");
  else if (chr == 255)
    printf("M-^?");
  else if (chr == '\t' && arguments.T == 1)
    printf("^I");
  else if (chr == '\t' && arguments.T == 0)
    printf("\t");
  else if (chr < 32 && chr >= 0)
    printf("^%c", chr + 64);
  else if (chr > 127 && chr < 160)
    printf("M-^%c", chr - 64);
  else if (chr >= 160)
    printf("M-%c", chr - 128);
  else
    printf("%c", chr);
}