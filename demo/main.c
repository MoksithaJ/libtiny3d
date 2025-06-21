#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../include/canvas.h"

#define WIDTH 1080
#define HEIGHT 1080
#define PI 3.14159265

int main()
{

    // char cwd[1024];
    // getcwd(cwd, sizeof(cwd));
    // printf("Current directory: %s\n", cwd);

    canvas_t canvas;
    canvas_init(&canvas, WIDTH, HEIGHT);
    canvas_clear(&canvas);

    int centerX = canvas.width / 2;
    int centerY = canvas.height / 2;
    float radius = (canvas.height < canvas.width ? canvas.height : canvas.width) / 2.0f - 2;

    for (int angle_deg = 0; angle_deg < 360; angle_deg += 15)
    {
        float angle_rad = angle_deg * (PI / 180.0f); // Convert to radians

        float endX = centerX + radius * cosf(angle_rad);
        float endY = centerY + radius * sinf(angle_rad);

        draw_line_f(&canvas, centerX, centerY, endX, endY, 1.0f);
    }
    // draw_line_f(&canvas, 10, 10, 70, 30, 1.0f);
    // canvas_print_ascii(&canvas);
    canvas_save_pgm(&canvas, "output.pgm");
    canvas_free(&canvas);

    return 0;
}
