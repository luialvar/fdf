#ifndef FDF_H
# define FDF_H

#include "MLX42.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <limits.h>
#include "get_next_line/get_next_line.h"

int     ft_atoi(const char *nptr, char *checker);
char    **ft_split(char const *s, char c);
void	*ft_memcpy(void *dest, const void *src, size_t n);
char    *get_next_line(int fd);
size_t	ft_strlen(const char *str);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_strchr(const char *str, int c);


#endif
