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
	// esto causa desproporciones
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

int** convertir_a_int(char** arr_str, int filas) {
    int** resultado = (int**)malloc(filas * sizeof(int*));
    int i = 0;

    while (i < filas) {
        // Usa ft_split para dividir el string en números separados
        char** tokens = ft_split(arr_str[i], ' ');

        // Cuenta cuántos tokens hay
        int cuenta = 0;
        while (tokens[cuenta] != NULL) {
            cuenta++;
        }

        // Reserva memoria para almacenar los enteros en resultado[i]
        resultado[i] = (int*)malloc(cuenta * sizeof(int));

        // Convierte cada token a un entero usando ft_atoi y verifica el resultado
        int j = 0;
        char checker = 'a';  // Inicia checker como válido
        while (j < cuenta) {
            resultado[i][j] = ft_atoi(tokens[j], &checker);
            if (checker == 'b') {
                printf("Error: Número inválido o desbordamiento detectado en la fila %d, columna %d\n", i, j);
                resultado[i][j] = 0;  // Puedes manejar el error según lo que prefieras
            }
            free(tokens[j]);  // Libera cada token después de la conversión
            j++;
        }
        free(tokens);          // Libera el array de tokens
        free(arr_str[i]);      // Libera cada string de arr_str una vez procesado
        i++;
    }

    free(arr_str);  // Libera el array de strings en sí
    return resultado;
}



int	count_lines(int infile)
{
	int		count = 0;
	char	buffer;
	int		has_content = 0;

	// Leer el archivo carácter por carácter
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
	int			**matrix;


	infile = open(argv[1], O_RDONLY, 0777);
	if (infile < 0)
	{
		perror("Error opening file");
		return 1;
	}
	line_count = count_lines(infile);
	clos(infile);
	infile = open(argv[1], O_RDONLY, 0777); // se hace esto para redirigir el puntero al inicio
	// Reservar memoria para el puntero de punteros con el tamaño exacto
	lines = malloc(sizeof(char *) * (line_count + 1));
	if (!lines)
	{
		perror("Error allocating memory");
		close(infile);
		return 1;
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
	matrix = make_matrix(lines);

	mlx = mlx_init(1, 1, "fdf", true);
	if (!mlx)
		return (1);

	// Ahora que mlx está inicializado, obtener el tamaño máximo del monitor
	mlx_get_monitor_size(0, &monitor_width, &monitor_height); // Monitor principal

	// Redimensionar la ventana al tamaño máximo del monitor
	mlx_set_window_size(mlx, monitor_width, monitor_height);

	// Crear la imagen con el tamaño de la ventana
	img = mlx_new_image(mlx, monitor_width, monitor_height);
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

