#include <fcntl.h>  // Para open()
#include <unistd.h> // Para close()
#include <stdio.h>  // Para printf()
#include <stdlib.h> // Para malloc(), free()

// Prototipo de la función get_next_line
char    *get_next_line(int fd);

int main(int argc, char **argv)
{
    int     *fds;
    int     i;
    char    *line;
    int     files_remaining;

    // Verificar si se ha pasado al menos un archivo como argumento
    if (argc < 2)
    {
        printf("Uso: %s <archivo1> <archivo2> ... <archivoN>\n", argv[0]);
        return (1);
    }

    // Reservar memoria para los descriptores de archivo
    fds = (int *)malloc(sizeof(int) * (argc - 1));
    if (!fds)
        return (1);

    // Abrir todos los archivos pasados como argumentos
    for (i = 0; i < argc - 1; i++)
    {
        fds[i] = open(argv[i + 1], O_RDONLY);
        if (fds[i] == -1)
        {
            printf("Error al abrir el archivo %s\n", argv[i + 1]);
            // Liberar los archivos abiertos hasta ahora
            while (i-- > 0)
                close(fds[i]);
            free(fds);
            return (1);
        }
    }

    // Leer y mostrar cada línea de todos los archivos simultáneamente
    files_remaining = argc - 1;
    while (files_remaining > 0)
    {
        files_remaining = 0;
        for (i = 0; i < argc - 1; i++)
        {
            line = get_next_line(fds[i]);
            if (line)
            {
                printf("Archivo %d: %s", i + 1, line);  // Imprimir la línea con el índice del archivo
                free(line);  // Liberar la memoria asignada por get_next_line
                files_remaining++;
            }
        }
    }

    // Cerrar todos los archivos
    for (i = 0; i < argc - 1; i++)
    {
        close(fds[i]);
    }
    
    // Liberar la memoria reservada para los descriptores de archivo
    free(fds);

    return (0);
}
