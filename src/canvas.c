#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "canvas.h"

void set_pixel_f(canvas_t *canvas, float x, float y, float intensity)
{
    int x0, y0, x1, y1;
    // printf("set_pixel_f: drawing at (%.2f, %.2f)\n", x, y);

    // round a floating-point pixel coordinate((x, y) = (10.3, 5.7))to the nearest 4 pixel coordinates
    x0 = (int)floor(x);
    y0 = (int)floor(y);
    x1 = x0 + 1;
    y1 = y0 + 1;

    // get horizontol and verticle constatnt to do the bilenear filtering
    float dx = x - x0;
    float dy = y - y0;

    // Compute bilinear weights
    float w_tl = (1.0f - dx) * (1.0f - dy);
    float w_tr = (dx) * (1.0f - dy);
    float w_bl = (1.0f - dx) * dy;
    float w_br = (dx) * (dy);

    // Apply brightness to each of the 4 neighboring pixels
    //  (with boundary check to prevent out-of-bounds)
    if (x0 >= 0 && x0 < canvas->width && y0 >= 0 && canvas->height > y0)
    {
        canvas->brightnessOfPixel[y0][x0] += intensity * w_tl;
    }
    if (x1 >= 0 && x1 < canvas->width && y0 >= 0 && canvas->height > y0)
    {
        canvas->brightnessOfPixel[y0][x1] += intensity * w_tr;
    }

    if (x0 >= 0 && x0 < canvas->width && y1 >= 0 && canvas->height > y1)
    {
        canvas->brightnessOfPixel[y1][x0] += intensity * w_bl;
    }

    if (x1 >= 0 && x1 < canvas->width && y1 >= 0 && canvas->height > y1)
    {
        canvas->brightnessOfPixel[y1][x1] += intensity * w_br;
    }
}

void draw_line_f(canvas_t *canvas, float x0, float y0, float x1, float y1, float thickness)
{

    // calculate dx & dy
    float dx = x1 - x0;
    float dy = y1 - y0;

    // calculate steps required for generating pixels
    int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);

    // calculate increment in x & y for each steps
    float Xinc = dx / (float)steps;
    float Yinc = dy / (float)steps;

    // Put pixel for each step
    float x = x0;
    float y = y0;

    for (int i = 0; i <= steps; i++)
    {
        // Draw a square of pixels around (x, y) to support thickness
        for (float tx = -thickness / 2; tx <= thickness / 2; tx += 0.5f)
        {
            for (float ty = -thickness / 2; ty <= thickness / 2; ty += 0.5f)
            {
                set_pixel_f(canvas, (x + tx), (y + ty), 1.0f); // Full brightness
            }
        }

        x += Xinc;
        y += Yinc;
    }
}

void canvas_save_pgm(canvas_t *canvas, const char *filename)
{
    FILE *f = fopen(filename, "w");
    if (!f)
        return;

    fprintf(f, "P2\n%d %d\n255\n", canvas->width, canvas->height);

    for (int y = 0; y < canvas->height; y++)
    {
        for (int x = 0; x < canvas->width; x++)
        {
            float b = canvas->brightnessOfPixel[y][x];
            int pixelvalue = (int)(b * 255.0f);
            if (pixelvalue > 255)
                pixelvalue = 255;
            if (pixelvalue < 0)
                pixelvalue = 0;
            fprintf(f, "%d ", pixelvalue);
        }
        fprintf(f, "\n");
    }
    fclose(f);
}

void canvas_init(canvas_t *canvas, int width, int height)
{
    //
    canvas->width = width;
    canvas->height = height;

    // allocate array of row pointers
    canvas->brightnessOfPixel = (float **)malloc(height * sizeof(float *));
    if (!canvas->brightnessOfPixel)
    {
        // allocation failed
        return;
    }

    // allocate each row
    for (int y = 0; y < height; y++)
    {
        canvas->brightnessOfPixel[y] = (float *)malloc(width * sizeof(float));
        if (!canvas->brightnessOfPixel[y])
        {
            // allocation failed free previously allocated rows
            for (int i = 0; i < y; i++)
            {
                free(canvas->brightnessOfPixel[i]);
            }
            free(canvas->brightnessOfPixel);
            canvas->brightnessOfPixel = NULL;
            return;
        }
    }
    // initialize all pixels to zer brightness (black)
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            canvas->brightnessOfPixel[y][x] = 0.0f;
        }
    }
}

void canvas_clear(canvas_t *canvas)
{

    for (int y = 0; y < canvas->height; y++)
    {
        for (int x = 0; x < canvas->width; x++)
        {
            canvas->brightnessOfPixel[y][x] = 0.0f;
        }
    }
}
void canvas_free(canvas_t *canvas)
{
    if (canvas->brightnessOfPixel)
    {
        for (int y = 0; y < canvas->height; y++)
        {
            free(canvas->brightnessOfPixel[y]);
        }
        free(canvas->brightnessOfPixel);
        canvas->brightnessOfPixel = NULL;
    }
}

void canvas_print_ascii(canvas_t *canvas)
{
    for (int y = 0; y < canvas->height; y++)
    {
        for (int x = 0; x < canvas->width; x++)
        {
            float b = canvas->brightnessOfPixel[y][x];

            // Map brightness to a character
            char ch;
            if (b > 0.9f)
                ch = '@';
            else if (b > 0.7f)
                ch = '#';
            else if (b > 0.5f)
                ch = '*';
            else if (b > 0.3f)
                ch = '+';
            else if (b > 0.1f)
                ch = '.';
            else
                ch = ' ';

            printf("%c", ch);
        }
        printf("\n");
    }
}
