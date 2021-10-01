/*
    make the points somehow independent of the framebuffer
    so when it draws outside the buffer it dont crash
*/


int h = 0;
int v = 0;

void run()
{
    drawLine(0, 100, 100, 50, 0xff0000);
    //drawLine(0, 0, 100, 100, 0x00ff00);
    
    pushFrame(fb.vm, fb.scnb);
}
