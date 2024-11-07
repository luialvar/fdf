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
void    draw(mlx_image_t *img, int **matrix, int line_count, int* number_count);
int**   make_matrix(char** str_arr, int rows, int* count_numberes_line);
int     count_lines(int infile);
char    **read_file_lines(const char *filename, int *line_count);
mlx_t	*initialize_window(mlx_image_t **img);
void    handle_resize(int32_t width, int32_t height, void *param);
void    handle_keypress(mlx_key_data_t keydata, void *param);
void    clear_image(mlx_image_t *img);

#endif
