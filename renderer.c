#include <stdio.h> //for testing stuff with printf



//need a way to not go out of bounds of memory

//const int cx = fb.width / 2; //center points so that pixels can be drawn relative to a center
//const int cy = fb.height / 2;



static void renderNothing()
{
    unsigned int *pixel = (unsigned int*)fb.scnb; //a pixel pointer to draw the pixels
    for(int b = 0; b < fb.height * fb.width; b++)
    {
        *pixel++ = 0;
    }
}

static void renderBackground(unsigned int iy, unsigned int ix, unsigned int color, unsigned char size)
{
    unsigned int *pixel = (unsigned int*)fb.scnb;
    for(int y = 0; y < fb.height; y++)
    {
        for(int x = 0; x < fb.width; x++)
        {
            *pixel++ = (((y*y/size)+iy) + ((x*x/size)+ix)) * color;
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
static void drawPixel_PartialScan(signed int ix, signed int iy, unsigned int color)
{
    unsigned int *pixel = (unsigned int*)fb.scnb;
    for(int i = 0; i < (fb.width * iy) + ix; i++)
    {
        *pixel++;
    }
    *pixel = color;
}


//  to map a 2d grid of pixels to 1d for direct memory manipulation - pixel = h_width(y) + x
/*
    ptrChange() adds the number of the intended pixel to the initial address pointed to
    by *pixel

    ix and iy needs clamped?
*/
static void drawPixel(unsigned int ix, unsigned int iy, unsigned int color)
{
    unsigned int *pixel = (unsigned int*)fb.scnb;
    ptrChange(&pixel, (fb.width) * iy + ix);
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
//y = (dy - dx) * x //b is constant so its unnecessary

// (0, 0)
// (3, 7)


/*
    if dx is greater than dy = low slope line
    if dx is not greater than dy = high slope line  
    bitshifting instead of multiplying by 2? good?
*/
static void drawLine(signed int x0, signed int y0, signed int x1, signed int y1, unsigned int color)
{
    signed int dx = x1 - x0;
    signed int dy = y1 - y0;
    signed int p = (dy - dx)<<1;
    //signed int y = y0;
    signed int x = x0;
    signed int y = y0;

    
    if((dx > 0 && dy > 0) || (dx < 0 && dy < 0))
    {
        //printf("Positively Sloped Line\n");
        if(dy < dx)
        {
            for(x; x <= x1 && y < fb.width; x++)
            {
                if(p >= 0)
                {
                    drawPixel(x, y++, color);
                    p += (dy - dx)<<1;
                }
                else
                {
                    drawPixel(x, y, color);
                    p += dy<<1;
                }
            }
        }
        else
        {
            for(y; y <= y1 && y < fb.width; y++)
            {
                if(p >= 0)
                {
                    drawPixel(x++, y, color);
                    p += (dx - dy)<<1;
                }
                else
                {
                    drawPixel(x, y, color);
                    p +=dx<<1;
                }
            }
        }
    }
    else
    {
        //printf("Negatively Sloped Line\n");
        if(!(dy > dx))
        {
            for(x; x <= x1; x++)
            {
                if(p <= 0)
                {
                    drawPixel(x, y--, color);
                    p += (dy - dx)<<1;
                }
                else
                {
                    drawPixel(x, y, color);
                    p += dy<<1;
                }
            }
        }
        else
        {
            for(y; y <= y1; y)
            {
                if(p <= 0)
                {
                    drawPixel(x--, y, color);
                    p += (dx - dy)<<1;
                }
                else
                {
                    drawPixel(x, y, color);
                    p += dx<<1;
                }
            }
        }
    }
}