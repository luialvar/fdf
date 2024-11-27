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
				&& sign == -1) || (nptr[i] < '0' && nptr[i] != '\n' && nptr[i] != '\t' 
			  && nptr[i] != ' ')|| nptr[i] > '9')
		{
			*checker = 'b';
			return (0);
		}
		i++;
	}
	return ((int)(num * sign));
}

int	is_fdf(char* name)
{
	int	n;

	n = ft_strlen(name);
	if (name[n - 4] == '.' && name[n - 3] == 'f' && name[n - 2] == 'd' 
		&& name[n - 1] == 'f')
		return (1);
	return (0); 
}

void clean_trailing_spaces(char *str)
{
    int len = ft_strlen(str);
    while (len > 0 && (str[len - 1] == ' ' || str[len - 1] == '\n' || str[len - 1] == '\t'))
    {
        str[len - 1] = '\0';  // Reemplaza espacios, saltos de línea o tabulaciones
        len--;
    }
}