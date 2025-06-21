#ifndef CANVAS_H
#define CANVAS_H

typedef struct
{
    int width;
    int height;
    float **brightnessOfPixel; // implement this brightnessOfPixel[x][y] >  dont know x y exctly
} canvas_t;                    // define a struct and give it a type name (canvas_t)

void draw_line_f(canvas_t *canvas, float x0, float y0, float x1, float y1, float thickness);
void set_pixel_f(canvas_t *canvas, float x, float y, float intensity);
void canvas_init(canvas_t *canvas, int width, int height);
void canvas_save_pgm(canvas_t *canvas, const char *filename);
void canvas_free(canvas_t *canvas);
void canvas_clear(canvas_t *canvas);
void canvas_print_ascii(canvas_t *canvas);

#endif