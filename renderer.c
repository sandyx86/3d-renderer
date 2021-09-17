#include <stdio.h>


//make a global variable or something the other functions can call
//make sure frame buffer is only initialized once, and then maybe realloced

/*
    \\Projection Matrix\\

*/

static void renderNothing()
{
    unsigned int *pixel = (unsigned int*)fb.buffer; //a pixel pointer to draw the pixels
    for(int y = 0; y < fb.height; y++)
    {
        for(int x = 0; x < fb.width; x++)
        {
            *pixel++;
        }
    }
}

//unsigned int *pixel = (unsigned int*)fb.buffer;

//draws a pixel at (ix, iy)
static void drawPixel(int ix, int iy, unsigned int color)
{
    unsigned int *pixel = (unsigned int*)fb.buffer;
    for(int y = 0; y < fb.height; y++)
    {
        for(int x = 0; x < fb.width; x++)
        {
            if(y == iy && x == ix)
            {
                *pixel++ = color;
            }
            else *pixel++;
        }
        
    }
}

//DDA Algorithm
//May not need the x and y for loop since drawPixel() already has it
static void drawLine(int x0, int y0, int x1, int y1, unsigned int color)
{
    int x, y = 0;

    int dx = x1 - x0;
    int dy = y1 - y0;

    int s = max(dx, dy);

    float xi = dx / s;
    float yi = dy / s;

    for(int v = 0; v < s; v++)
    {
        if(x > 1)
        {
            drawPixel(x, y, color);
            printf("%d", x);
            x += xi;
        }
        else
        {
            drawPixel(x, y, color);
            printf("%f", x);
            y += yi;
        }
    }
}

//find the intermediate points between two points
/*
    if(x = y)
    {
        both x and y will increment
    }
    elseif(x > y)
    {
        x will always increment
    }
    elseif(x < y)
    {
        y will always increment
    }
*/
static void drawHLine(int x0, int y0, int x1, unsigned int color) //(x, y, x')
{
    int dx = x1 - x0;
    for (int v = 0; v < dx; v++)
    {
        drawPixel(x0++, y0, color); //assuming x will always increment since its a horizontal line
    }
}


//y = mx + b
static void bLine(int x0, int y0, int x1, int y1, unsigned int color)
{
    int dx = x1 - x0;
    int dy = y1 - y0;
    
}