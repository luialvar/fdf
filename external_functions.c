#include "fdf.h"

int	calculate_sign(int *i, const char *nptr)
{
	int	sign;

	sign = 1;
	while (nptr[*i] == '+' || nptr[*i] == '-')
	{
		if (nptr[*i] == '-')
			sign *= -1;
		(*i)++;
	}
	return (sign);
}

//atoi con modificaciones para tratar desbordamientos de int
int	ft_atoi(const char *nptr, char *checker)
{
	int		i;
	int		sign;
	long	num;

	i = 0;
	sign = calculate_sign(&i, nptr);
	num = 0;
	if (nptr[0] == '\0' || nptr[i] == '\0')
	{
		*checker = 'b';
		return (0);
	}
	while (nptr[i])
	{
		if (nptr[i] >= '0' && nptr[i] <= '9')
			num = (nptr[i] - 48) + (num * 10);
		if ((num > INT_MAX && sign == 1) || (num > ((long)INT_MAX + 1)
				&& sign == -1) || (nptr[i] < '0' && nptr[i] != '\n') || nptr[i] > '9')

		{
			*checker = 'b';
			return (0);
		}
		i++;
	}
	return ((int)(num * sign));
}