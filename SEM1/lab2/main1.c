#include <math.h>
#include <stdio.h>
#include <stdlib.h>


long double Tser(long double x, int n)
{

	long double Series = 1;
	long double y = x * x;

	long double numrtr = (-2) * y;
	int denumrtr = 2;
	Series += (double) (numrtr / denumrtr);

	for (int i = 3; i <= n; ++i)
	{
		denumrtr += 2;
		Series += (double) ((-4) * y * Series) / ((denumrtr + 1) * (denumrtr + 2));
	}
	return Series;
}

int input_longdouble(long double *value)
{
	int check = 0;
	while (check != 1)
	{
		check = scanf("%Lf", value);
		if (check == EOF)
			return 1;
		if (check == 1)
			break;
		printf("Как же так? Попробуйте снова\n");
		scanf("%*[^\n]");
	}
	return 0;
}

int main()
{
	long double x;
	long double n1 = -1;

	printf("%s", "Введите число х функции cos^2(x): ");
	if (input_longdouble(&x))
		return 1;

	printf("%s", "Введие точность n, которая больше нуля: ");
	n1 = -1;
	while (n1 <= 0)
	{
		printf("%s", "Как же так? Попробуйте снова\n");
		if (input_longdouble(&n1))
			return 1;
	}

	int n = 1 / n1;

	printf("%s", "Функция согласно нашим расчетам: ");
	printf("%.10Lf\n", Tser(x, n));

	printf("%s", "Функция cos^2(x) согласно расчетам с библиотекой math.h: ");
	printf("%.10lf", cos(x) * cos(x));
	printf("%lu\n", sizeof(double));
	return 0;
}
