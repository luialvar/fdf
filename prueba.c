#include "MLX42.h"
#include <math.h>

// Función para dibujar una línea entre dos puntos
void draw_line(mlx_image_t *img, int x1, int y1, int x2, int y2, uint32_t color)
{
    int dx = abs(x2 - x1), sx = x1 < x2 ? 1 : -1;
    int dy = -abs(y2 - y1), sy = y1 < y2 ? 1 : -1;
    int err = dx + dy, e2;

    while (1) {
        mlx_put_pixel(img, x1, y1, color);  // Dibuja un pixel en las coordenadas actuales
        if (x1 == x2 && y1 == y2) break;    // Termina si llega al destino
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x1 += sx; }
        if (e2 <= dx) { err += dx; y1 += sy; }
    }
}

// Función para dibujar un cubo 3D básico
void draw_cube(mlx_image_t *img)
{
    // Coordenadas 3D del cubo
    int cube_vertices[8][3] = {
        {-50, -50, -50}, {50, -50, -50}, {50, 50, -50}, {-50, 50, -50}, // Cara frontal
        {-50, -50, 50}, {50, -50, 50}, {50, 50, 50}, {-50, 50, 50}     // Cara trasera
    };

    // Proyectamos los puntos 3D a 2D (simplificación sin perspectiva)
    int projected_vertices[8][2];
    for (int i = 0; i < 8; i++)
    {
        projected_vertices[i][0] = cube_vertices[i][0] + 400;  // Ajuste de posición X
        projected_vertices[i][1] = cube_vertices[i][1] + 300;  // Ajuste de posición Y
    }

    uint32_t color = 0xFF0000FF;  // Color rojo para las líneas

    // Dibujar líneas entre los vértices del cubo (conectamos los vértices para formar el cubo)
    // Cara frontal
    draw_line(img, projected_vertices[0][0], projected_vertices[0][1], projected_vertices[1][0], projected_vertices[1][1], color);
    draw_line(img, projected_vertices[1][0], projected_vertices[1][1], projected_vertices[2][0], projected_vertices[2][1], color);
    draw_line(img, projected_vertices[2][0], projected_vertices[2][1], projected_vertices[3][0], projected_vertices[3][1], color);
    draw_line(img, projected_vertices[3][0], projected_vertices[3][1], projected_vertices[0][0], projected_vertices[0][1], color);

    // Cara trasera
    draw_line(img, projected_vertices[4][0], projected_vertices[4][1], projected_vertices[5][0], projected_vertices[5][1], color);
    draw_line(img, projected_vertices[5][0], projected_vertices[5][1], projected_vertices[6][0], projected_vertices[6][1], color);
    draw_line(img, projected_vertices[6][0], projected_vertices[6][1], projected_vertices[7][0], projected_vertices[7][1], color);
    draw_line(img, projected_vertices[7][0], projected_vertices[7][1], projected_vertices[4][0], projected_vertices[4][1], color);

    // Conectar cara frontal con la trasera
    draw_line(img, projected_vertices[0][0], projected_vertices[0][1], projected_vertices[4][0], projected_vertices[4][1], color);
    draw_line(img, projected_vertices[1][0], projected_vertices[1][1], projected_vertices[5][0], projected_vertices[5][1], color);
    draw_line(img, projected_vertices[2][0], projected_vertices[2][1], projected_vertices[6][0], projected_vertices[6][1], color);
    draw_line(img, projected_vertices[3][0], projected_vertices[3][1], projected_vertices[7][0], projected_vertices[7][1], color);
}

int main(void)
{
    mlx_t            *mlx;
    mlx_image_t      *img;

    // Inicializar la ventana con MLX42
    mlx = mlx_init(800, 600, "Cubo 3D", true);
    if (!mlx)
        return (1);

    // Crear la imagen
    img = mlx_new_image(mlx, 800, 600);
    if (!img)
        return (1);

    // Dibujar el cubo en la imagen
    draw_cube(img);

    // Mostrar la imagen en la ventana
    mlx_image_to_window(mlx, img, 0, 0);

    // Manejar el cierre de la ventana con la tecla ESC
    mlx_key_hook(mlx, handle_keypress, mlx);

    // Ejecutar el bucle de eventos
    mlx_loop(mlx);
    mlx_terminate(mlx);

    return (0);
}

