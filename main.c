/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luialvar <luialvar@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 08:49:13 by luialvar          #+#    #+#             */
/*   Updated: 2024/12/08 15:54:42 by luialvar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

mlx_t	*initialize_window(mlx_image_t **img)
{
	int		monitor_width;
	int		monitor_height;
	mlx_t	*mlx;

	mlx = mlx_init(1, 1, "fdf", true);
	if (!mlx)
		return (NULL);
	mlx_get_monitor_size(0, &monitor_width, &monitor_height);
	mlx_set_window_size(mlx, monitor_width, monitor_height);
	*img = mlx_new_image(mlx, monitor_width, monitor_height);
	if (!img)
		return (NULL);
	return (mlx);
}

void	handle_keypress(mlx_key_data_t keydata, void *param)
{
	mlx_t	*mlx;

	mlx = (mlx_t *)param;
	if (keydata.key == MLX_KEY_ESCAPE && keydata.action == MLX_PRESS)
		mlx_close_window(mlx);
}

int	is_fdf(char *name)
{
	int	n;

	n = ft_strlen(name);
	if (name[n - 4] == '.' && name[n - 3] == 'f' && name[n - 2] == 'd'
		&& name[n - 1] == 'f')
		return (1);
	return (0);
}

int	validate_arguments(int argc, char **argv)
{
	if (argc != 2 || !is_fdf(argv[1]))
	{
		perror("No good format file");
		return (1);
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_data_main	a;

	if (validate_arguments(argc, argv))
		return (1);
	a.mlx = initialize_window(&a.img);
	if (!a.mlx)
		return (1);
	a.lines = read_file_lines(argv[1], &a.line_count);
	if (!a.lines)
		return (1);
	a.number_count = malloc(sizeof(int) * (a.line_count));
	if (!a.number_count)
		free_array((void **)a.lines);
	a.matrix = make_matrix(a.lines, a.line_count, a.number_count);
	if (!a.matrix || !a.number_count)
		return (1);
	draw(a.img, a.matrix, a.line_count, a.number_count);
	mlx_image_to_window(a.mlx, a.img, 0, 0);
	mlx_key_hook(a.mlx, handle_keypress, a.mlx);
	mlx_loop(a.mlx);
	free(a.number_count);
	free_array((void **)a.matrix);
	mlx_terminate(a.mlx);
	return (0);
}

/*
void	clear_image(mlx_image_t *img)
{
	int y = 0;
	while (y < (int)img->height)
	{
		int x = 0;
		while (x < (int)img->width)
		{
			mlx_put_pixel(img, x, y, 0x00000000); // Color transparente
			x++;
		}
		y++;
	}
}
*/

/*
 void	handle_resize(int32_t width, int32_t height, void *param)
{
	mlx_image_t	*img;
	
	img = (mlx_image_t *)param;
	if (!mlx_resize_image(img, width, height))
		return ;
	// hasta aquí solo se redimensiona la imagen,
	// esto causa desproporciones
}
*/

/*
int	main(int argc, char **argv)
{
	mlx_t			*mlx;
	mlx_image_t		*img;
	char                    **lines;
        int                     line_count;
        int                             **matrix;
        int                     *number_count;
		int		monitor_width;
        int		monitor_height;

	mlx = mlx_init(1, 1, "fdf", true);
	if (!mlx)
		return (1);
	mlx_get_monitor_size(0, &monitor_width, &monitor_height);
	mlx_set_window_size(mlx, monitor_width, monitor_height);
        img = mlx_new_image(mlx, monitor_width, monitor_height);
	if (!img)
		return (1);
	lines = read_file_lines(argv[1], &line_count);
        number_count = malloc(sizeof(int)*line_count);
        if (!lines)
                return 1;
        matrix = make_matrix(lines, line_count, number_count);
        if (!matrix)
                return 1;
	draw(img, matrix, line_count, number_count);
	mlx_image_to_window(mlx, img, 0, 0);
	mlx_loop(mlx);
	mlx_terminate(mlx);
	return (0);
}
*/
/*
int	main(int argc, char **argv)
{
	mlx_t			*mlx;
	mlx_image_t		*img;
	char			**lines;
	int			line_count;
	int				**matrix;
	int			*number_count;

	mlx = initialize_window(&img);
	if (!mlx)
		return 1;
	mlx_key_hook(mlx, handle_keypress, mlx);
	mlx_resize_hook(mlx, handle_resize, img);
	lines = read_file_lines(argv[1], &line_count);
	number_count = malloc(sizeof(int)*line_count);
        if (!lines)
                return 1;
        matrix = make_matrix(lines, line_count, number_count);
        if (!matrix)
                return 1;
	draw(img, matrix, line_count, number_count);
	mlx_image_to_window(mlx, img, 0, 0);
	mlx_loop(mlx);
	mlx_terminate(mlx);
	return (0);
}

int	main(int argc, char **argv)
{
	mlx_t			*mlx;
	mlx_image_t		*img;
	char                    **lines;
        int                     line_count;
        int                             **matrix;
        int                     *number_count;

	mlx = mlx_init(800, 600, "Ventana Isométrica con Zoom", true);
	if (!mlx)
		return (1);

	img = mlx_new_image(mlx, 800, 600);
	if (!img)
		return (1);
	lines = read_file_lines(argv[1], &line_count);
        number_count = malloc(sizeof(int)*line_count);
        if (!lines)
                return 1;
        matrix = make_matrix(lines, line_count, number_count);
        if (!matrix)
                return 1;
	draw(img, matrix, line_count, number_count);
	mlx_image_to_window(mlx, img, 0, 0);
	mlx_loop(mlx);
	mlx_terminate(mlx);
	return (0);
}*/
