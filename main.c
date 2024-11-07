/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luialvar <luialvar@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 08:49:13 by luialvar          #+#    #+#             */
/*   Updated: 2024/11/06 16:34:27 by luialvar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

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
	/*lines = read_file_lines(argv[1], &line_count);
	number_count = malloc(sizeof(int)*line_count);
        if (!lines)
                return 1;
        matrix = make_matrix(lines, line_count, number_count);
        if (!matrix)
                return 1;
	draw(img, matrix, line_count, number_count);*/
	mlx_loop(mlx);
	mlx_terminate(mlx);
	return (0);
}
