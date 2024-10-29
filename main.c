#include "MLX42.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

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
	// esto puede causar desproporciones
}

void	draw(mlx_image_t *img)
{
	int	i;
	int	e;

	i = 0;
	while (i < 80)
	{
		e = 0;
		while (e < 400)
		{
			mlx_put_pixel(img, i, e, 0xFF0000FF);
			e++;
		}
		i++;
	}
}

int	main(int argc, char **argv)
{
	mlx_t			*mlx;
	mlx_image_t		*img;
	int				monitor_width;
	int				monitor_height;
	int				infile;
	char			**lines;
	int				i;
	char			*current_line;


	infile = open(argv[1], O_RDONLY, 0777);
	while (get_next_line(infile)
	{
		current_line 
	}

	// Inicializar la ventana con MLX42 (temporal, luego redimensionamos si es necesario)
	mlx = mlx_init(1, 1, "fdf", true);
	if (!mlx)
		return (1);

	// Ahora que mlx está inicializado, obtener el tamaño máximo del monitor
	mlx_get_monitor_size(0, &monitor_width, &monitor_height); // Monitor principal

	// Redimensionar la ventana al tamaño máximo del monitor
	mlx_set_window_size(mlx, monitor_width * 0.7, monitor_height * 0.6);

	// Crear la imagen con el tamaño de la ventana
	img = mlx_new_image(mlx, monitor_width * 0.7, monitor_height * 0.6);
	if (!img)
		return (1);

	draw(img);
	mlx_image_to_window(mlx, img, 0, 0); // optimiza llamadas al hardware
	// para imprimir píxeles, a la vez ayuda al renderizado para no imprimir uno a uno

	mlx_key_hook(mlx, handle_keypress, mlx); // llama a handle_keypress
	// cuando detecta una tecla, el tercer parámetro es la ventana que se controla,
	// mientras que el primero es la ventana en la que se detecta el evento

	mlx_resize_hook(mlx, handle_resize, img); // en este caso el primer parámetro
	// es donde se recibe el evento y también donde se realizan los cambios,
	// los parámetros se le pasan solo a handle_resize

	mlx_loop(mlx); // coge los manejadores de eventos declarados con hook,
	// y es el que los mantiene atentos

	mlx_terminate(mlx); // se encarga de liberar toda la memoria no de cerrar el programa
	return (0);
}

