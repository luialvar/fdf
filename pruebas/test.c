#include "MLX42.h"
#include <math.h>
#include <stdlib.h>

float zoom_factor = 1.0f; // Factor de zoom inicial

void	draw_line(mlx_image_t *img, int x0, int y0, int x1, int y1, uint32_t color)
{
	int dx = abs(x1 - x0);
	int dy = abs(y1 - y0);
	int sx = (x0 < x1) ? 1 : -1;
	int sy = (y0 < y1) ? 1 : -1;
	int err = dx - dy;

	while (x0 != x1 || y0 != y1)
	{
		if (x0 >= 0 && x0 < img->width && y0 >= 0 && y0 < img->height)
			mlx_put_pixel(img, x0, y0, color);
		int e2 = 2 * err;
		if (e2 > -dy) { err -= dy; x0 += sx; }
		if (e2 < dx) { err += dx; y0 += sy; }
	}
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

void draw_isometric_square(mlx_image_t *img)
{
	// Limpiar la imagen antes de redibujar
	clear_image(img);

	// Puntos del cubo en 3D (base cuadrada y una altura en z)
	int points[8][3] = {
		{0, 0, 0},        // punto A (base)
		{80, 0, 0},       // punto B (base)
		{80, 80, 0},      // punto C (base)
		{0, 80, 0},       // punto D (base)
		{0, 0, 80},       // punto A' (arriba)
		{80, 0, 80},      // punto B' (arriba)
		{80, 80, 80},     // punto C' (arriba)
		{0, 80, 80}       // punto D' (arriba)
	};

	// Líneas para dibujar la base y las aristas
	int edges[12][2] = {
		{0, 1}, {1, 2}, {2, 3}, {3, 0}, // base cuadrada
		{4, 5}, {5, 6}, {6, 7}, {7, 4}, // parte superior
		{0, 4}, {1, 5}, {2, 6}, {3, 7}  // aristas verticales
	};

	for (int i = 0; i < 12; i++)
	{
		int p1 = edges[i][0];
		int p2 = edges[i][1];

		// Aplicar el factor de zoom a cada coordenada
		int scaled_x1 = points[p1][0] * zoom_factor;
		int scaled_y1 = points[p1][1] * zoom_factor;
		int scaled_z1 = points[p1][2] * zoom_factor;
		int scaled_x2 = points[p2][0] * zoom_factor;
		int scaled_y2 = points[p2][1] * zoom_factor;
		int scaled_z2 = points[p2][2] * zoom_factor;

		// Transformación isométrica
		int x1 = (scaled_x1 - scaled_y1) * cos(0.523599);
		int y1 = (scaled_x1 + scaled_y1) * sin(0.523599) - scaled_z1;
		int x2 = (scaled_x2 - scaled_y2) * cos(0.523599);
		int y2 = (scaled_x2 + scaled_y2) * sin(0.523599) - scaled_z2;

		// Dibujar la línea
		draw_line(img, 400 + x1, 300 + y1, 400 + x2, 300 + y2, 0xFF0000FF);
	}
}

/*
void	draw_isometric_square(mlx_image_t *img)
{
	// Limpiar la imagen antes de redibujar
	clear_image(img);

	// Puntos originales del cuadrado en 3D
	int points[4][3] = {
		{0, 0, 0},
		{80, 0, 0},
		{80, 80, 0},
		{0, 80, 0}
	};

	for (int i = 0; i < 4; i++)
	{
		int j = (i + 1) % 4;

		// Aplicar el factor de zoom a cada coordenada
		int scaled_x1 = points[i][0] * zoom_factor;
		int scaled_y1 = points[i][1] * zoom_factor;
		int scaled_x2 = points[j][0] * zoom_factor;
		int scaled_y2 = points[j][1] * zoom_factor;

		// Transformación isométrica
		int x1 = (scaled_x1 - scaled_y1) * cos(0.523599);
		int y1 = (scaled_x1 + scaled_y1) * sin(0.523599);
		int x2 = (scaled_x2 - scaled_y2) * cos(0.523599);
		int y2 = (scaled_x2 + scaled_y2) * sin(0.523599);

		// Dibujar la línea del cuadrado en la nueva escala
		draw_line(img, 400 + x1, 300 + y1, 400 + x2, 300 + y2, 0xFF0000FF);
	}
}
*/


void	handle_scroll(double xdelta, double ydelta, void *param)
{
	mlx_image_t *img = (mlx_image_t *)param;

	// Ajustar el factor de zoom en función del desplazamiento
	if (ydelta > 0) // Zoom in
		zoom_factor *= 1.1f;
	else if (ydelta < 0) // Zoom out
		zoom_factor *= 0.9f;
	
	/*// Limitar el factor de zoom para que no sea demasiado pequeño o grande
	if (zoom_factor < 0.1f) zoom_factor = 0.1f;
	if (zoom_factor > 10.0f) zoom_factor = 10.0f;

	// Redibujar el cuadrado con el nuevo factor de zoom*/
	draw_isometric_square(img);

}

int	main(void)
{
	mlx_t			*mlx;
	mlx_image_t		*img;

	mlx = mlx_init(800, 600, "Ventana Isométrica con Zoom", true);
	if (!mlx)
		return (1);

	img = mlx_new_image(mlx, 800, 600);
	if (!img)
		return (1);

	draw_isometric_square(img);
	mlx_image_to_window(mlx, img, 0, 0);

	// Configurar el scroll para controlar el zoom
	mlx_scroll_hook(mlx, handle_scroll, img);

	mlx_loop(mlx);
	mlx_terminate(mlx);
	return (0);
}
