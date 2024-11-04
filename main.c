/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luialvar <luialvar@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 08:49:13 by luialvar          #+#    #+#             */
/*   Updated: 2024/10/30 11:06:58 by luialvar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

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

void draw(mlx_image_t *img, int **matrix)
{
	// Limpiar la imagen antes de redibujar
	clear_image(img);

	double angle_x = 0.523599; // 30 grados en radianes
	double angle_y = 0.523599; // 30 grados en radianes
	int offset_x = 400;        // Offset en X para centrar la imagen
	int offset_y = 300;        // Offset en Y para centrar la imagen
	int scale = 20;            // Factor de escala unificado

	for (int i = 0; matrix[i] != NULL; i++)
	{
		for (int j = 0; matrix[i][j] != '\0'; j++)
		{
			// Coordenadas originales del punto en 3D
			int x3D = j * scale;
			int y3D = i * scale;
			int z3D = matrix[i][j] * scale;

			// Transformación isométrica
			int x_iso = (x3D - y3D) * cos(angle_x);
			int y_iso = (x3D + y3D) * sin(angle_y) - z3D;

			// Dibujar el punto en la proyección isométrica
			mlx_put_pixel(img, offset_x + x_iso, offset_y + y_iso, 0xFF0000FF);
		}
	}
}

int**	make_matrix(char** str_arr, int rows) {
	int**	result;
	int		i;
	int 		count;

	result = (int**)malloc(rows * sizeof(int*));
	i = 0;
	while (i < rows) 
	{
		char**	tokens = ft_split(str_arr[i], ' ');
		count = 0;
		while (tokens[count] != NULL) 
			count++;
		result[i] = (int*)malloc(count * sizeof(int));
		int		j = 0;
		char	checker = 'a';  // Inicia checker como válido
		while (j < count) {
			result[i][j] = ft_atoi(tokens[j], &checker);
			if (checker == 'b') {
				printf("Error: Invalid number or overflow detected at row %d, column %d\n", i, j);
				return (NULL);
			}
			free(tokens[j]); // Libera cada token después de la conversión
			j++;
		}
		free(tokens);
		free(str_arr[i]);
		i++;
	}
	free(str_arr);
	return result;
}

int	count_lines(int infile)
{
	int		count = 0;
	char	buffer;
	int		has_content = 0;

	// leer el archivo caracter por caracter
	while (read(infile, &buffer, 1) > 0)
	{
		has_content = 1; // Indica que hay contenido en el archivo
		if (buffer == '\n')
			count++;
	}
	// Si hay contenido pero no terminó en un salto de línea, contar una línea extra
	if (has_content && buffer != '\n')
		count++;
	return count;
}

char	**read_file_lines(const char *filename, int *line_count)
{
	int		infile;
	char	**lines;
	char	*current_line;
	int		i;

	infile = open(filename, O_RDONLY);
	if (infile < 0)
	{
		perror("Error opening file");
		return NULL;
	}
	*line_count = count_lines(infile);
	close(infile);
	infile = open(filename, O_RDONLY);
	if (infile < 0)
	{
		perror("Error reopening file");
		return NULL;
	}
	lines = malloc(sizeof(char *) * ((*line_count) + 1));
	if (!lines)
	{
		perror("Error allocating memory");
		close(infile);
		return NULL;
	}
	i = 0;
	current_line = get_next_line(infile);
	while (current_line != NULL)
	{
		lines[i] = current_line;
		i++;
		current_line = get_next_line(infile);
	}
	lines[i] = NULL;
	close(infile);
	return (lines);
}

mlx_t *initialize_window(mlx_image_t **img)
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

int	main(int argc, char **argv)
{
	mlx_t			*mlx;
	mlx_image_t		*img;
	char			**lines;
	int			line_count;
	int				**matrix;

	mlx = initialize_window(&img);
	if (!mlx)
		return 1;
	mlx_key_hook(mlx, handle_keypress, mlx);
	mlx_resize_hook(mlx, handle_resize, img);
	lines = read_file_lines(argv[1], &line_count);
        if (!lines)
                return 1;
        matrix = make_matrix(lines, line_count);
        if (!matrix)
                return 1;
	draw(img, matrix);
	mlx_loop(mlx);
	mlx_terminate(mlx);
	return (0);
}
