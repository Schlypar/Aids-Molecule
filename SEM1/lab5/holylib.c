#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int inputInt(int *input) {
  int check = 0;
  while (check != 1) {
    check = scanf("%d", input);
    if (check == EOF) {
      return 1;
    }
    if (check == 1) {
      break;
    }
    printf("Как же так? Попробуйте снова\n");
    scanf("%*[^\n]");
  }
  return 0;
}
