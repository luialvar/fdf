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

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t	i;

	if (!src && !dest)
		return (NULL);
	i = 0;
	while (i < n)
	{
		((unsigned char *)dest)[i] = ((unsigned char *)src)[i];
		i++;
	}
	return (dest);
}

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i] != '\0')
	{
		i++;
	}
	return (i);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*retorno;
	size_t	size;

	if (!s1 || !s2)
		return (0);
	size = ft_strlen(s1) + ft_strlen(s2);
	retorno = (char *)malloc(size + 1);
	if (!retorno)
		return (0);
	ft_memcpy(retorno, s1, ft_strlen(s1));
	ft_memcpy(retorno + ft_strlen(s1), s2, ft_strlen(s2));
	retorno[size] = '\0';
	return (retorno);
}

char	*ft_strchr(const char *s, int c)
{
	size_t	i;
	size_t	len;

	len = ft_strlen(s);
	i = 0;
	while (i <= len)
	{
		if (s[i] == (char)c)
		{
			return ((char *)s + i);
		}
		i++;
	}
	return (NULL);
}
