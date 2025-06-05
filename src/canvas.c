#include <stdio.h>
#include "canvas.h"

int main()
{

}

void set_pixel_f(canvas,float x,float y , float intensity)
{
    int x0,y0,x1,y1;
    //round a floating-point pixel coordinate((x, y) = (10.3, 5.7))to the nearest 4 pixel coordinates
    x0 = (int)x; 
    y0 = (int)y; 
    x1 = x0 + 1;
    y1 = y0 +1;

    //get horizontol and verticle constatnt to do the bilenear filtering
    float dx = x - x0;
    float dy = y - y0;

    //
    float TL = 

    canvas->brightnessOfPixel[y0][x0] += intensity * (1 - dx) * (1 - dy);//top left
    
}
