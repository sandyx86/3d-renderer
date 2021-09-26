int h = 0;
int v = 0;
void run()
{
    drawLine(0, 0, 10, 8, 0x00ff00);
    //drawHLine(0, 0 + v++, 640, 0xff0000);
    //drawVLine(0 + h++, 0, 480, 0x00ff00);
    if(h >= 640){h = 0;}
    if(v >= 480){v = 0;}
    pushFrame(fb.vm, fb.scnb);
}
