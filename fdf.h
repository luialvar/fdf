/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luialvar <luialvar@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/08 20:59:20 by luialvar          #+#    #+#             */
/*   Updated: 2024/12/08 21:07:57 by luialvar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
# define FDF_H

# include "src/MLX42/MLX42.h"
# include <math.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <limits.h>
# include "get_next_line/get_next_line.h"

typedef struct s_data
{
    mlx_t        *mlx;
    mlx_image_t  *img;
    char         **lines;
    int          line_count;
    int          **matrix;
    int          *number_count;
} t_data_main;

typedef struct s_draw_data {
    int **matrix;
    int line_count;
    int *number_count;
    double scale;
    double angle;
} t_draw_data;

typedef struct s_projection_data {
    double angle;
    double max_min_x[2];
    double max_min_y[2];
    double projection[2];
    int i;
    int j;
    double scale_x;
    double scale_y;
} t_projection_data;

typedef struct s_limits {
    double projection[2];
    int i;
    int x3D;
    int y3D;
    int z3D;
    int j;
} t_limits;

typedef struct s_offsets {
    int x;
    int y;
} t_offsets;

int			ft_atoi(const char *nptr, char *checker);
char		**ft_split(char const *s, char c);
void		draw(mlx_image_t *img, int **matrix, int line_count, int *number_count);
int**		make_matrix(char** str_arr, int rows, int* count_numberes_line);
int			count_lines(int infile);
char		**read_file_lines(const char *filename, int *line_count);
mlx_t		*initialize_window(mlx_image_t **img);
void		handle_resize(int32_t width, int32_t height, void *param);
void		handle_keypress(mlx_key_data_t keydata, void *param);
void		clear_image(mlx_image_t *img);
int			is_fdf(char* name);
void		clean_trailing_spaces(char *str);
void		free_array(void **directories);

int populate_matrix(int **result, char **str_arr, int rows, int *count);
int process_row(int **result, char *row, int *count_numbers_line, int i);
int allocate_and_fill_row(int **result, char **tokens, int count, int i);

#endif
