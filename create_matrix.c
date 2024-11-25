/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_matrix.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luialvar <luialvar@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 12:20:38 by luialvar          #+#    #+#             */
/*   Updated: 2024/11/09 12:28:16 by luialvar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

int**	make_matrix(char** str_arr, int rows, int* count_numbers_line) 
{
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
		count_numbers_line[i] = count;
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
		//free(str_arr[i]);
		i++;
	}
	//free(str_arr);
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
	while (i < (*line_count))
	{
		lines[i] = current_line;
		i++;
		current_line = get_next_line(infile);
	}
	lines[i] = NULL;
	close(infile);
	return (lines);
}
/*
int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <archivo>\n", argv[0]);
        return 1;
    }

    char **lines;
    int line_count;
    int *count_numbers_line;
    int **matrix;

    // Leer el archivo y obtener el contenido en `lines`
    lines = read_file_lines(argv[1], &line_count);
    if (!lines) {
        fprintf(stderr, "Error al leer el archivo\n");
        return 1;
    }

    // Asignar memoria para el conteo de números en cada línea
    count_numbers_line = malloc(line_count * sizeof(int));
    // Crear la matriz a partir de las líneas leídas
    matrix = make_matrix(lines, line_count, count_numbers_line);
   if (!matrix) {
        fprintf(stderr, "Error al crear la matriz\n");
        free(count_numbers_line);
        for (int i = 0; i < line_count; i++) {
            free(lines[i]);
        }
        free(lines);
        return 1;
    }

    // Imprimir la matriz para verificar el contenido
    printf("Contenido de la matriz:\n");
    for (int i = 0; i < line_count; i++) {
        printf("Fila %d: ", i);
        for (int j = 0; j < count_numbers_line[i]; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
    return 0;
}*/
