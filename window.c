#include <windows.h>
#include <stdbool.h>
#include <stdio.h>
#include "utils.c"

global_variable bool isRunning = true;

struct Framebuffer
{
    void *buffer;
    int width;
    int height;
    BITMAPINFO bmi;
};

global_variable struct Framebuffer fb;

#include "renderer.c"
#include "program.c"


LRESULT CALLBACK window_callback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LRESULT result = 0;

    switch(uMsg)
    {
        case WM_CLOSE:
            isRunning = false;
            DestroyWindow(hwnd);
        break;

        case WM_DESTROY:
             isRunning = false;
             DestroyWindow(hwnd);
             PostQuitMessage(0);
        break;

        case WM_SIZE:
        {
            RECT rect;
            GetClientRect(hwnd, &rect);
            fb.width = rect.right - rect.left;
            fb.height = rect.bottom - rect.top;

            printf("%d %d %d %d\n", rect.right, rect.left, rect.top, rect.bottom);

            int bufferSize = fb.width * fb.height * sizeof(unsigned int);

            //if(fb.buffer) VirtualFree(fb.buffer, 0, MEM_RELEASE);
            //fb.buffer = VirtualAlloc(0, bufferSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

            if(fb.buffer)
            {
                free(fb.buffer);
                fb.buffer = (int*)malloc(bufferSize);
            }
            else fb.buffer = (int*)malloc(bufferSize);

            fb.bmi.bmiHeader.biSize = sizeof(fb.bmi.bmiHeader);
            fb.bmi.bmiHeader.biWidth = fb.width;
            fb.bmi.bmiHeader.biHeight = fb.height;
            fb.bmi.bmiHeader.biPlanes = 1;
            fb.bmi.bmiHeader.biBitCount = 32;
            fb.bmi.bmiHeader.biCompression = BI_RGB;
        }
        break;

        default:
            result = DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return result;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCMD)
{
    WNDCLASS wc = {};
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpszClassName = "ViewWindow";
    wc.lpfnWndProc = window_callback;

    RegisterClass(&wc);

    HWND window = CreateWindow(wc.lpszClassName, "View Window", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 640 + 16, 480 + 39, 0, 0, hInstance, 0);
    HDC hdc = GetDC(window);


    while(isRunning)
    {
        MSG message;
        while(PeekMessage(&message, window, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&message);
            DispatchMessage(&message);
        }

        //simulate
        run();
        
        //render
        StretchDIBits(hdc, 0, 0, fb.width, fb.height, 0, 0, fb.width, fb.height,fb.buffer, &fb.bmi, DIB_RGB_COLORS, SRCCOPY);
    }
    return 0;
}