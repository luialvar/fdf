/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printing.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luialvar <luialvar@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 12:00:11 by luialvar          #+#    #+#             */
/*   Updated: 2024/11/10 17:42:45 by luialvar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

//algoritmo de Bresenham
void draw_line(mlx_image_t *img, int x0, int y0, int x1, int y1, uint32_t color)
{
	// Calcula las diferencias absolutas en X e Y
	int dx = abs(x1 - x0);
	int dy = abs(y1 - y0);

	// Determina la dirección de los pasos en X e Y
	int sx = -1;
	int sy = -1;
	if (x0 < x1)
		sx = 1;
	if (y0 < y1)
		sy = 1;

	// Error inicial
	int err = dx - dy;

	// Dibuja la línea píxel a píxel
	while (x0 != x1 || y0 != y1)
	{
		// Verifica que el punto está dentro de los límites de la imagen
		if (x0 >= 0 && x0 < img->width && y0 >= 0 && y0 < img->height)
			mlx_put_pixel(img, x0, y0, color);

		// Calcula el doble del error para ajustar X o Y
		int e2 = 2 * err;

		// Ajusta el error y el valor de X
		if (e2 > -dy)
		{
			err -= dy;
			x0 += sx;
		}

		// Ajusta el error y el valor de Y
		if (e2 < dx)
		{
			err += dx;
			y0 += sy;
		}
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
	clear_image(img);

	double angle_x = 0.523599; // 30 grados en radianes
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
