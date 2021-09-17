#include <stdio.h> //for testing stuff with printf

//need a way to not go out of bounds of memory
//possibly need an extra framebuffer to make animated things work


static void renderNothing()
{
    unsigned int *pixel = (unsigned int*)fb.scnb; //a pixel pointer to draw the pixels
    for(int y = 0; y < fb.height; y++)
    {
        for(int x = 0; x < fb.width; x++)
        {
            *pixel++ = 0;
        }
    }
}

//pushes the frame in the buffer to video memory, then clears the buffer. may there be a more efficient way to do this?
static void pushFrame(unsigned int* d, unsigned int* buffer)
{
    unsigned int *pixel = (unsigned int*)fb.vm;
    unsigned int *buffer_pixel = (unsigned int*)fb.scnb;
    
    for(int b = 0; b < fb.width * fb.height; b++)
    {
        *pixel++ = *buffer_pixel++;
    }
    renderNothing();
}

//may be an issue if called multiple times
static void ptrChange(unsigned int **p, unsigned int address)
{
    *p += address;
}

/*
    scans the entire screen and checks whether the current coordinates
    are the intended values (extremely slow)
*/
static void drawPixel_FullScan(int ix, int iy, unsigned int color)
{
    unsigned int *pixel = (unsigned int*)fb.scnb;
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

/*
    scans the screen until it reaches the intended pixel
*/
static void drawPixel_PartialScan(int ix, int iy, unsigned int color)
{
    unsigned int *pixel = (unsigned int*)fb.scnb;
    for(int i = 0; i < fb.width * iy + ix; i++)
    {
        *pixel++;
    }
    *pixel = color;
}

/*
    to map a 2d grid of pixels to 1d for direct memory manipulation - pixel = h_width(y) + x
    ptrChange() adds the number of the intended pixel to the initial address pointed to
    by *pixel
*/
static void drawPixel(int ix, int iy, unsigned int color)
{
    unsigned int *pixel = (unsigned int*)fb.scnb;
    ptrChange(&pixel, fb.width * iy + ix);
    //ptrChange(&pixel, fb.width * iy + ix);
    
    *pixel = color;
}

static void drawHLine(int x0, int y0, int x1, unsigned int color) //(x, y, x')
{
    int dx = x1 - x0;
    for (int v = 0; v < dx; v++)
    {
        drawPixel(x0++, y0, color); //assuming x will always increment since its a horizontal line
    }
}

static void drawVLine(int x0, int y0, int y1, unsigned int color) //(x, y, y')
{
    int dy = y1 - y0;
    for (int v = 0; v < dy; v++)
    {
        drawPixel(x0, y0++, color); //assuming y will always increment since its a horizontal line
    }
}


//y = mx + b
static void bLine(int x0, int y0, int x1, int y1, unsigned int color)
{
    int dx = x1 - x0;
    int dy = y1 - y0;
    
}