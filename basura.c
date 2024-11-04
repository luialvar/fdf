#include "fdf.h"

size_t	ft_strlen(const char *str)
{
	size_t i;

	i = 0;
	while (str[i] != '\0')
		i++;
	return (i);
}

char	*ft_strchr(const char *str, int c)
{
	int i;
	int flag;

	flag = 0;
	i = 0;
	while (((char*)str)[i] != '\0' && flag == 0)
	{
		if (((char*)str)[i] == c)
		{
			return ((char*)&str[i]);
			flag = 1;
		}
		i++;
	}
	if (c == '\0')
		return ((char*)&str[i]);
	return (NULL);
}


char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*str;
	int		a;
	int		b;
	int		i;

	if (!s1 || !s2)
		return (NULL);
	a = ft_strlen(s1);
	b = ft_strlen(s2);
	i = 0;
	if (!(str = malloc((a + b + 1))) || !str)
		return (NULL);
	while (a--)
	{
		str[i] = s1[i];
		i++;
	}
	a = i;
	i = 0;
	while (b--)
		str[a++] = s2[i++];
	str[a] = '\0';
	return (str);
}
