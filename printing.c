/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printing.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luialvar <luialvar@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 12:00:11 by luialvar          #+#    #+#             */
/*   Updated: 2024/12/08 15:53:46 by luialvar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

// algoritmo de Bresenham
void	draw_line(mlx_image_t *img, int x0, int y0, int x1, int y1)
{
	int	dx;
	int	dy;
	int	sx;
	int	sy;
	int	err;
	int	e2;
	uint32_t color;

	color = 0xFF0000FF;
	dx = abs(x1 - x0);
	dy = abs(y1 - y0);
	sx = -1;
	sy = -1;
	if (x0 < x1)
		sx = 1;
	if (y0 < y1)
		sy = 1;
	err = dx - dy;
	while (x0 != x1 || y0 != y1)
	{
		if (x0 >= 0 && x0 < (int)img->width && y0 >= 0 && y0 < (int)img->height)
			mlx_put_pixel(img, x0, y0, color);
		e2 = 2 * err;
		if (e2 > -dy)
		{
			err -= dy;
			x0 += sx;
		}
		if (e2 < dx)
		{
			err += dx;
			y0 += sy;
		}
	}
}

double	determine_scale(mlx_image_t *img, int **matrix, int line_count, int *number_count)
{
	t_projection_data	data;

	data.angle = 0.523599;
	data.max_min_x[0] = 0;
	data.max_min_x[1] = 0;
	data.max_min_y[0] = 0;
	data.max_min_y[1] = 0;
	data.i = 0;
	while (data.i < line_count)
	{
		data.j = 0;
		while (data.j < number_count[data.i])
		{
			data.projection[0] = (data.j - data.i) * cos(data.angle); // x_iso
			data.projection[1] = (data.j + data.i) * sin(data.angle) - matrix[data.i][data.j]; // y_iso
			data.max_min_x[0] = fmax(data.max_min_x[0], data.projection[0]);
			data.max_min_x[1] = fmin(data.max_min_x[1], data.projection[0]);
			data.max_min_y[0] = fmax(data.max_min_y[0], data.projection[1]);
			data.max_min_y[1] = fmin(data.max_min_y[1], data.projection[1]);
			data.j++;
		}
		data.i++;
	}
	data.scale_x = img->width * 0.5 / (data.max_min_x[0] - data.max_min_x[1]);
	data.scale_y = img->height * 0.5 / (data.max_min_y[0] - data.max_min_y[1]);
	return (fmin(data.scale_x, data.scale_y));
}

void	calculate_limits(t_draw_data *data ,double *max_min_x, double *max_min_y) //bien
{
	t_limits	limit;

	limit.i = 0;
	while (limit.i < data->line_count)
	{
		limit.j = 0;
		while (limit.j < data->number_count[limit.i])
		{
			limit.x3D = limit.j * data->scale;
			limit.y3D = limit.i * data->scale; 
			limit.z3D = data->matrix[limit.i][limit.j] * data->scale;
			limit.projection[0] = (limit.x3D - limit.y3D) * cos(data->angle);
			limit.projection[1] = (limit.x3D + limit.y3D) * sin(data->angle) - limit.z3D;
			max_min_x[0] = fmax(max_min_x[0], limit.projection[0]);
			max_min_x[1] = fmin(max_min_x[1], limit.projection[0]);
			max_min_y[0] = fmax(max_min_y[0], limit.projection[1]);
			max_min_y[1] = fmin(max_min_y[1], limit.projection[1]);
			limit.j++;
		}
		limit.i++;
	}
}

void	draw_horizontal_lines(mlx_image_t *img, t_draw_data *data, t_offsets offsets, int i)
{
	double	projection[2];
	double	projection_next[2];
	int		j;
	int		y3D;

	y3D = i * data->scale;
	j = 0;
	while (j < data->number_count[i])
	{
		int x3D = j * data->scale;
		int z3D = data->matrix[i][j] * data->scale;
		projection[0] = (x3D - y3D) * cos(data->angle);
		projection[1] = (x3D + y3D) * sin(data->angle) - z3D;
		if (j + 1 < data->number_count[i])
		{
			projection_next[0] = ((j + 1) * data->scale - y3D) * cos(data->angle);
			projection_next[1] = ((j + 1) * data->scale + y3D) * sin(data->angle)
								 - (data->matrix[i][j + 1] * data->scale);
			draw_line(img, offsets.x + projection[0], offsets.y + projection[1],
					  offsets.x + projection_next[0], offsets.y + projection_next[1]);
		}
		j++;
	}
}

void	draw_vertical_lines(mlx_image_t *img, t_draw_data *data, t_offsets offsets, int i)
{
	double	projection[2];
	double	projection_next[2];
	int		j;
	int		y3D;

	y3D = i * data->scale;
	j = 0;
	while (j < data->number_count[i])
	{
		int x3D = j * data->scale;
		int z3D = data->matrix[i][j] * data->scale;
		projection[0] = (x3D - y3D) * cos(data->angle);
		projection[1] = (x3D + y3D) * sin(data->angle) - z3D;
		if (i + 1 < data->line_count && j < data->number_count[i + 1])
		{
			projection_next[0] = (x3D - (i + 1) * data->scale) * cos(data->angle);
			projection_next[1] = (x3D + (i + 1) * data->scale) * sin(data->angle)
								 - (data->matrix[i + 1][j] * data->scale);
			draw_line(img, offsets.x + projection[0], offsets.y + projection[1],
					  offsets.x + projection_next[0], offsets.y + projection_next[1]);
		}
		j++;
	}
}

void	draw_adjusted_lines(mlx_image_t *img, t_draw_data *data, int offset_x, int offset_y)
{
	t_offsets offsets = {offset_x, offset_y};
	int	i;

	i = 0;
	while (i < data->line_count)
	{
		draw_horizontal_lines(img, data, offsets, i);
		draw_vertical_lines(img, data, offsets, i);
		i++;
	}
}

void	draw(mlx_image_t *img, int **matrix, int line_count, int *number_count) //bien
{
	t_draw_data	data;
	double	max_min_x[2];
	double	max_min_y[2];
	int	offset_x;
	int	offset_y;

	data.matrix = matrix;
    data.line_count = line_count;
    data.number_count = number_count;
    data.angle = 0.523599;
    data.scale = determine_scale(img, matrix, line_count, number_count);
	max_min_x[0] = 0;
	max_min_x[1] = 0;
	max_min_y[0] = 0;
	max_min_y[1] = 0;
	// Calcular límites para centrado
	calculate_limits(&data, max_min_x, max_min_y);

	offset_x = (img->width - (max_min_x[0] - max_min_x[1])) / 2 - max_min_x[1];
	offset_y = (img->height - (max_min_y[0] - max_min_y[1])) / 2 - max_min_y[1];

	// Dibujar líneas ajustadas al centro
	draw_adjusted_lines(img, &data, offset_x, offset_y);
}

/*
void	draw_adjusted_lines(mlx_image_t *img, t_draw_data *data, int offset_x, int offset_y)
{
	double	projection[2];
	double	projection_next[2];
	int		i; 
	int		j;

	i = 0;
	while (i < data->line_count)
	{
		j = 0;
		while (j < data->number_count[i])
		{
			int x3D = j * data->scale, y3D = i * data->scale, z3D = data->matrix[i][j] * data->scale;
			projection[0] = (x3D - y3D) * cos(data->angle); // x_iso
			projection[1] = (x3D + y3D) * sin(data->angle) - z3D; // y_iso
			if (j + 1 < data->number_count[i])
			{
				projection_next[0] = ((j + 1) * data->scale - y3D) * cos(data->angle); // x_iso
				projection_next[1] = ((j + 1) * data->scale + y3D) * sin(data->angle) - (data->matrix[i][j + 1] * data->scale); // y_iso
				draw_line(img, offset_x + projection[0], offset_y + projection[1],
						   offset_x + projection_next[0], offset_y + projection_next[1]);
			}
			if (i + 1 < data->line_count && j < data->number_count[i + 1])
			{
				projection_next[0] = (x3D - (i + 1) * data->scale) * cos(data->angle); // x_iso
				projection_next[1] = (x3D + (i + 1) * data->scale) * sin(data->angle) - (data->matrix[i + 1][j] * data->scale); // y_iso
				draw_line(img, offset_x + projection[0], offset_y + projection[1],
						   offset_x + projection_next[0], offset_y + projection_next[1]);
			}
			j++;
		}
		i++;
	}
}
*/

/*
void draw(mlx_image_t *img, int **matrix, int line_count, int *number_count) {
    double angle_x = 0.523599, angle_y = 0.523599;
    double scale = determine_scale(img, matrix, line_count, number_count);
    double max_min_x[2] = {0, 0}, max_min_y[2] = {0, 0};
    double projection[2], projection_next[2];
    int i = 0;

    // Calcular valores mínimos y máximos proyectados para ajustar el centrado
    while (i < line_count) {
        int j = 0;
        while (j < number_count[i]) {
            int x3D = j * scale, y3D = i * scale, z3D = matrix[i][j] * scale;
            calculate_projection(x3D, y3D, z3D, angle_x, angle_y, projection);
            max_min_x[0] = fmax(max_min_x[0], projection[0]);
            max_min_x[1] = fmin(max_min_x[1], projection[0]);
            max_min_y[0] = fmax(max_min_y[0], projection[1]);
            max_min_y[1] = fmin(max_min_y[1], projection[1]);
            j++;
        }
        i++;
    }

    int offset_x = (img->width - (max_min_x[0] - max_min_x[1])) / 2 - max_min_x[1];
    int offset_y = (img->height - (max_min_y[0] - max_min_y[1])) / 2 - max_min_y[1];
    i = 0;

    // Dibujar líneas ajustadas al centro
    while (i < line_count) {
        int j = 0;
        while (j < number_count[i]) {
            int x3D = j * scale, y3D = i * scale, z3D = matrix[i][j] * scale;
            calculate_projection(x3D, y3D, z3D, angle_x, angle_y, projection);

            // Dibujar línea horizontal
            if (j + 1 < number_count[i]) {
                calculate_projection((j + 1) * scale, y3D, matrix[i][j + 1] * scale, angle_x, angle_y, projection_next);
                draw_lines(img, offset_x + projection[0], offset_y + projection[1],
                           offset_x + projection_next[0], offset_y + projection_next[1], 0xFF0000FF);
            }

            // Dibujar línea vertical
            if (i + 1 < line_count && j < number_count[i + 1]) {
                calculate_projection(x3D, (i + 1) * scale, matrix[i + 1][j] * scale, angle_x, angle_y, projection_next);
                draw_lines(img, offset_x + projection[0], offset_y + projection[1],
                           offset_x + projection_next[0], offset_y + projection_next[1], 0xFF0000FF);
            }
            j++;
        }
        i++;
    }
}

double determine_scale(mlx_image_t *img, int **matrix, int line_count, int* number_count)
{
	double angle_x = 0.523599; // 30 grados en radianes
	double angle_y = 0.523599; // 30 grados en radianes
	double max_min_x[2] = {0, 0};
	double max_min_y[2] = {sin(angle_y) - matrix[0][0], sin(angle_y) - matrix[0][0]};
	int i = 0;

	while (i < line_count)
	{
		int stop = number_count[i];
		int j = 0;

		while (j < stop)
		{
			int x3D = j;
			int y3D = i;
			int z3D = matrix[i][j];

			// Proyección isométrica
			int x_iso = (x3D - y3D) * cos(angle_x);
			int y_iso = (x3D + y3D) * sin(angle_y) - z3D;

			if (x_iso > max_min_x[0])
			{
				max_min_x[0] = x_iso;
			}
			if (x_iso < max_min_x[1])
			{
				max_min_x[1] = x_iso;
			}
			if (y_iso > max_min_y[0])
			{
				max_min_y[0] = y_iso;
			}
			if (y_iso < max_min_y[1])
			{
				max_min_y[1] = y_iso;
			}
			j++;
		}
		i++;
	}

	double scale_x = img->width * 0.5 / (max_min_x[0] - max_min_x[1]);
	double scale_y = img->height * 0.5 / (max_min_y[0] - max_min_y[1]);

	if (scale_x < scale_y)
	{
		return scale_x;
	}
	else
	{
		return scale_y;
	}
}

void draw(mlx_image_t *img, int **matrix, int line_count, int* number_count)
{
	// Limpiar la imagen antes de redibujar
	//clear_image(img);

	double angle_x = 0.523599; // 30 grados en radianesgrado quimica uma
	double angle_y = 0.523599; // 30 grados en radianes
	double scale = determine_scale(img, matrix, line_count, number_count);

	double min_x = 0;
	double max_x = 0;
	double min_y = 0;
	double max_y = 0;
	int i = 0;

	while (i < line_count)
	{
		int stop = number_count[i];
		int j = 0;

		while (j < stop)
		{
			int x3D = j * scale;
			int y3D = i * scale;
			int z3D = matrix[i][j] * scale;

			int x_iso = (int)((x3D - y3D) * cos(angle_x));
			int y_iso = (int)((x3D + y3D) * sin(angle_y) - z3D);

			if (x_iso > max_x)
			{
				max_x = x_iso;
			}
			if (x_iso < min_x)
			{
				min_x = x_iso;
			}
			if (y_iso > max_y)
			{
				max_y = y_iso;
			}
			if (y_iso < min_y)
			{
				min_y = y_iso;
			}

			j++;
		}

		i++;
	}

	int offset_x = (img->width - (max_x - min_x)) / 2 - min_x;
	int offset_y = (img->height - (max_y - min_y)) / 2 - min_y;
	i = 0;

	while (i < line_count)
	{
		int stop = number_count[i];
		int j = 0;

		while (j < stop)
		{
			int x3D = j * scale;
			int y3D = i * scale;
			int z3D = matrix[i][j] * scale;

			int x_iso = (int)((x3D - y3D) * cos(angle_x));
			int y_iso = (int)((x3D + y3D) * sin(angle_y) - z3D);

			int x_current = offset_x + x_iso;
			int y_current = offset_y + y_iso;

			// Dibujar la línea horizontal si hay un punto siguiente en la misma fila
			if (j + 1 < stop)
			{
				int x3D_next = (j + 1) * scale;
				int y3D_next = i * scale;
				int z3D_next = matrix[i][j + 1] * scale;

				int x_iso_next = (int)((x3D_next - y3D_next) * cos(angle_x));
				int y_iso_next = (int)((x3D_next + y3D_next) * sin(angle_y) - z3D_next);

				int x_next = offset_x + x_iso_next;
				int y_next = offset_y + y_iso_next;

				draw_line(img, x_current, y_current, x_next, y_next, 0xFF0000FF);
			}

			// Dibujar la línea vertical si hay un punto en la siguiente fila
			if (i + 1 < line_count && j < number_count[i + 1])
			{
				int x3D_next = j * scale;
				int y3D_next = (i + 1) * scale;
				int z3D_next = matrix[i + 1][j] * scale;

				int x_iso_next = (int)((x3D_next - y3D_next) * cos(angle_x));
				int y_iso_next = (int)((x3D_next + y3D_next) * sin(angle_y) - z3D_next);

				int x_next = offset_x + x_iso_next;
				int y_next = offset_y + y_iso_next;

				draw_line(img, x_current, y_current, x_next, y_next, 0xFF0000FF);
			}

			j++;
		}

		i++;
	}
}
*/