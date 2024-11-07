/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luialvar <luialvar@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 08:49:13 by luialvar          #+#    #+#             */
/*   Updated: 2024/11/07 17:34:54 by luialvar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

mlx_t	*initialize_window(mlx_image_t **img)
{
	int monitor_width;
	int monitor_height;

	mlx_t *mlx = mlx_init(1, 1, "fdf", true);
	mlx_get_monitor_size(0, &monitor_width, &monitor_height);
	mlx_set_window_size(mlx, monitor_width, monitor_height);
	*img = mlx_new_image(mlx, monitor_width, monitor_height);
	mlx_image_to_window(mlx, *img, 0, 0);
	return (mlx);
}

void	clear_image(mlx_image_t *img)
{
	int y = 0;
	while (y < img->height)
	{
		int x = 0;
		while (x < img->width)
		{
			mlx_put_pixel(img, x, y, 0x00000000); // Color transparente
			x++;
		}
		y++;
	}
}

void	handle_keypress(mlx_key_data_t keydata, void *param)
{
	mlx_t	*mlx;

	mlx = (mlx_t *)param;
	if (keydata.key == MLX_KEY_ESCAPE && keydata.action == MLX_PRESS)
		mlx_close_window(mlx);
}

void	handle_resize(int32_t width, int32_t height, void *param)
{
	mlx_image_t	*img;
	
	img = (mlx_image_t *)param;
	if (!mlx_resize_image(img, width, height))
		return ;
	// hasta aquí solo se redimensiona la imagen,
	// esto causa desproporciones
}

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
