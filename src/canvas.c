#include <math.h>
#include <stdio.h>
#include "canvas.h"

void set_pixel_f(canvas_t *canvas, float x, float y, float intensity)
{
    int x0, y0, x1, y1;
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