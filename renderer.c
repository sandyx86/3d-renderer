#include <stdio.h> //for testing stuff with printf


//make sure frame buffer is only initialized once, and then maybe realloced

/*
    \\Projection Matrix\\

*/

static void ptrChange(unsigned int **p, unsigned int address)
{
    *p += address;
}

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

static void drawPixelFast(int ix, int iy, unsigned int color)
{
    unsigned int *pixel = (unsigned int*)fb.buffer;
    for(int i = 0; i < fb.width * iy + ix; i++)
    {
        *pixel++;
    }
    *pixel = color;
}

static void drawPixelFaster(int ix, int iy, unsigned int color)
{
    unsigned int *pixel = (unsigned int*)fb.buffer;
    ptrChange(&pixel, fb.width * iy + ix);
    
    *pixel = color;
}

static void drawHLine(int x0, int y0, int x1, unsigned int color) //(x, y, x')
{
    int dx = x1 - x0;
    for (int v = 0; v < dx; v++)
    {
        drawPixelFaster(x0++, y0, color); //assuming x will always increment since its a horizontal line
    }
}

static void drawVLine(int x0, int y0, int y1, unsigned int color) //(x, y, y')
{
    int dy = y1 - y0;
    for (int v = 0; v < dy; v++)
    {
        drawPixelFaster(x0, y0++, color); //assuming y will always increment since its a horizontal line
    }
}


//y = mx + b
static void bLine(int x0, int y0, int x1, int y1, unsigned int color)
{
    int dx = x1 - x0;
    int dy = y1 - y0;
    
}