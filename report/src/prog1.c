#include <stdio.h>

int sum(int x, int y) {
    int s = x + y;
    return s;
}

int main() {
    int a = 0, b = 0;
    printf("a = ");
    scanf("%d", &a);
    printf("b = ");
    scanf("%d", &b);
    int s = sum(a, b);
    printf("s = a + b = %d\n", s);
    return 0;
}
