# include <stdio.h>

int fact(int n) {
	int i = 2;
	while (i * i <= n) {
		while (n % i == 0) {
			n = n / i;
			printf("%d\n", i);
		}
		i++;
	}
	if (n > 1) {
		printf("%d\n", n);
	}
	return 0;
}

int main() {
    int n;
	scanf("%d", &n);
	if (n < 0) {
		n = -n;
		}
    fact(n);
    return 0;
}
    
