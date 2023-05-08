#include <math.h>
#include <stdio.h>
#include <stdlib.h>


long double Tser(long double x, long double eps, int *NUM_terms)
{

	long double Series = 1;
	long double Series_current;

	long double y = x * x;
	long double numrtr = (-2) * y;

	int denumrtr = 2;
	Series += (double) (numrtr / denumrtr);

	*NUM_terms = 2;
	Series_current = (double) (((-4) * y) / ((denumrtr + 1) * (denumrtr + 2)));

	while (((cos(x) * cos(x) - eps) > (Series - Series_current)) || ((Series - Series_current) > (cos(x) * cos(x) + eps)))
	{
		Series_current = (double) (((-4) * y) / ((denumrtr + 1) * (denumrtr + 2)));
		denumrtr += 2;

		Series += (double) (((-4) * y * Series) / ((denumrtr + 1) * (denumrtr + 2)));
		*NUM_terms += 1;
	}
	Series -= Series_current;
	return Series;
}

long double input_longdouble(long double *value)
{

	int check = 0;

	while (check != 1)
	{
		check = scanf("%Lf", value);
		if (check == EOF)
		{
			return 1;
			;
		}
		if (check == 1)
			break;
		printf("Как же так? Попробуйте снова");
		while (getchar() != '\n')
			continue;
	}
	return 0;
}

int main()
{
	long double x;
	long double eps;
	int NUM_terms = 0;

	printf("%s", "Введите число х функции cos^2(x): ");
	if (input_longdouble(&x))
		return 1;

	printf("%s", "Введите точность (epsilon), которая больше нуля: ");
	eps = -1;
	while (eps <= 0)
	{
		printf("%s", "Как же так? Попробуйте снова\n");
		if (input_longdouble(&eps))
			return 1;
	}

	printf("%s", "Функция согласно нашим расчетам: ");
	printf("%.10Lf\n", Tser(x, eps, &NUM_terms));

	printf("%s", "Количество членов, понадобишихся для достижения точности: ");
	printf("%d\n", NUM_terms);

	printf("%s", "Функция cos^2(x) согласо расчетам с библиотекой math.h: ");
	printf("%.10lf\n", cos(x) * cos(x));

	return 0;
}
