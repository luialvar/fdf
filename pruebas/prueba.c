#include "MLX42.h"
#include <math.h>
#include <stdlib.h>

void	draw_line(mlx_image_t *img, int x0, int y0, int x1, int y1, uint32_t color)
{
	int dx = abs(x1 - x0);
	int dy = abs(y1 - y0);
	int sx = (x0 < x1) ? 1 : -1;
	int sy = (y0 < y1) ? 1 : -1;
	int err = dx - dy;

	while (x0 != x1 || y0 != y1)
	{
		mlx_put_pixel(img, x0, y0, color); // Dibuja el píxel en la posición actual
		int e2 = 2 * err;
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

void	draw_isometric_square(mlx_image_t *img)
{
	int points[4][3] = {
		{0, 0, 0},
		{80, 0, 0},
		{80, 80, 0},
		{0, 80, 0}
	};

	for (int i = 0; i < 4; i++)
	{
		int j = (i + 1) % 4;
		int x1 = (points[i][0] - points[i][1]) * cos(0.523599);
		int y1 = (points[i][0] + points[i][1]) * sin(0.523599) - points[i][2];
		int x2 = (points[j][0] - points[j][1]) * cos(0.523599);
		int y2 = (points[j][0] + points[j][1]) * sin(0.523599) - points[j][2];
		draw_line(img, 200 + x1, 200 + y1, 200 + x2, 200 + y2, 0xFF0000FF);
	}handle_keypress(mlx_key_data_t keydata, void *param)
}

int	main(void)
{
	mlx_t			*mlx;
	mlx_image_t		*img;

	mlx = mlx_init(800, 600, "Ventana Isométrica", true);
	if (!mlx)
		return (1);

	img = mlx_new_image(mlx, 800, 600);
	if (!img)
		return (1);

	draw_isometric_square(img);
	mlx_image_to_window(mlx, img, 0, 0);

	mlx_loop(mlx);
	mlx_terminate(mlx);
	return (0);
}

