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

int	**make_matrix(char **str_arr, int rows, int *count_numbers_line)
{
	int	**result;

	result = (int **)malloc((rows + 1) * sizeof(int *));
	if (!result)
	{
		free_array((void **)str_arr);
		free(count_numbers_line);
		return (NULL);
	}
	result[rows] = NULL;
	if (!populate_matrix(result, str_arr, rows, count_numbers_line))
	{
		free_array((void **)result);
		free_array((void **)str_arr);
		free(count_numbers_line);
		return (NULL);
	}
	free_array((void **)str_arr);
	return (result);
}

int	populate_matrix(int **result, char **str_arr, int rows, int *count)
{
	int	i;

	i = 0;
	while (i < rows)
	{
		if (!process_row(result, str_arr[i], count, i))
			return (0);
		i++;
	}
	return (1);
}

int	process_row(int **result, char *row, int *count_numbers_line, int i)
{
	char	**tokens;
	int		count;

	count = 0;
	tokens = ft_split(row, ' ');
	if (!tokens)
	{
		perror("Error allocating memory");
		return (0);
	}
	while (tokens[count] != NULL)
		count++;
	count_numbers_line[i] = count;
	if (!allocate_and_fill_row(result, tokens, count, i))
	{
		free_array((void **)tokens);
		return (0);
	}
	free_array((void **)tokens);
	return (1);
}

int	allocate_and_fill_row(int **result, char **tokens, int count, int i)
{
	int		j;
	char	checker;

	checker = 'a';
	result[i] = (int *)malloc(count * sizeof(int));
	if (!result[i])
	{
		perror("Error allocating memory");
		return (0);
	}
	j = 0;
	while (j < count)
	{
		result[i][j] = ft_atoi(tokens[j], &checker);
		if (checker == 'b')
		{
			printf("Error: Invalid number or overflow detected at row %d, column %d\n", i, j); //change
			return (0);
		}
		j++;
	}
	return (1);
}

int	count_lines(int infile)
{
	int		count;
	char	buffer;
	int		has_content;

	count = 0;
	has_content = 0;
	while (read(infile, &buffer, 1) > 0)
	{
		has_content = 1;
		if (buffer == '\n')
			count++;
	}
	if (has_content && buffer != '\n')
		count++;
	return (count);
}

char	**allocate_lines_array(int line_count, int infile)
{
	char	**lines;
	char	*current_line;
	int		i;

	lines = malloc(sizeof(char *) * (line_count + 1));
	if (!lines)
	{
		perror("Error allocating memory");
		return (NULL);
	}
	i = 0;
	current_line = get_next_line(infile);
	while (i < line_count)
	{
		clean_trailing_spaces(current_line);
		lines[i] = current_line;
		i++;
		current_line = get_next_line(infile);
	}
	lines[i] = NULL;
	return (lines);
}

char	**read_file_lines(const char *filename, int *line_count)
{
	int		infile;
	char	**lines;

	infile = open(filename, O_RDONLY);
	if (infile < 0)
	{
		perror("Error opening file");
		return (NULL);
	}
	*line_count = count_lines(infile);
	close(infile);
	infile = open(filename, O_RDONLY);
	if (infile < 0)
	{
		perror("Error reopening file");
		return (NULL);
	}
	lines = allocate_lines_array(*line_count, infile);
	close(infile);
	return (lines);
}

/*
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
		return (NULL);
	}
	*line_count = count_lines(infile);
	close(infile);
	infile = open(filename, O_RDONLY);
	if (infile < 0)
	{
		perror("Error reopening file");
		return (NULL);
	}
	lines = malloc(sizeof(char *) * ((*line_count) + 1));
	if (!lines)
	{
		perror("Error allocating memory");
		close(infile);
		return (NULL);
	}
	i = 0;
	current_line = get_next_line(infile);
	while (i < (*line_count))
	{
		clean_trailing_spaces(current_line);
		lines[i] = current_line;
		i++;
		current_line = get_next_line(infile);
	}
	lines[i] = NULL;
	close(infile);
	return (lines);
}
*/

/*
int	**make_matrix(char **str_arr, int rows, int *count_numbers_line)
{
	int			**result;
	int			i;
	int			count;
	char		**tokens;
	int			j;
	char		checker;

	result = (int **)malloc(rows * sizeof(int *));
	checker = 'a';
	if (!result)
	{
		perror("Error allocating memory");
		return (NULL);
	}
	i = 0;
	while (i < rows)
	{
		tokens = ft_split(str_arr[i], ' ');
		if (!tokens)
		{
			perror("Error allocating memory");
			free_array((void **)result);
			return (NULL);
		}
		count = 0;
		while (tokens[count] != NULL)
			count++;
		count_numbers_line[i] = count;
		result[i] = (int *)malloc(count * sizeof(int));
		if (!(result[i]))
		{
			perror("Error allocating memory");
			free_array((void **)result);
			free_array((void **)tokens);
			return (NULL);
		}
		j = 0;
		while (j < count)
		{
			result[i][j] = ft_atoi(tokens[j], &checker);
			if (checker == 'b')
			{
				printf("Error: Invalid number or overflow detected
				free_array((void **)result);
				free_array((void **)tokens);
				free(result[i]);
				return (NULL);
			}
			free(tokens[j]);
			j++;
		}
		free(tokens);
		free(str_arr[i]);
		i++;
	}
	free(str_arr);
	return (result);
}
*/

/*
int	**make_matrix(char **str_arr, int rows, int *count_numbers_line)
{
	t_data_matrix b;

	b.result = (int **)malloc(rows * sizeof(int *));
	b.checker = 'a';
	if (!b.result)
	{
		perror("Error allocating memory");
		return (NULL);
	}
	b.i = 0;
	while (b.i < rows)
	{
		b.tokens = ft_split(str_arr[b.i], ' ');
		if (!b.tokens)
		{
			perror("Error allocating memory");
			free_array((void **)b.result);
			return (NULL);
		}
		b.count = 0;
		while (b.tokens[b.count] != NULL)
			b.count++;
		count_numbers_line[b.i] = b.count;
		b.result[b.i] = (int *)malloc(b.count * sizeof(int));
		if (!(b.result[b.i]))
		{
			perror("Error allocating memory");
			free_array((void **)b.result);
			free_array((void **)b.tokens);
			return (NULL);
		}
		b.j = 0;
		while (b.j < b.count)
		{
			b.result[b.i][b.j] = ft_atoi(b.tokens[b.j], &b.checker);
			if (b.checker == 'b')
			{
				printf("Error: Invalid number or overflow detected 
				free_array((void **)b.result);
				free_array((void **)b.tokens);
				free(b.result[b.i]);
				return (NULL);
			}
			free(b.tokens[b.j]);
			b.j++;
		}
		free(b.tokens);
		free(str_arr[b.i]);
		b.i++;
	}
	free(str_arr);
	return (b.result);
}*/

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
