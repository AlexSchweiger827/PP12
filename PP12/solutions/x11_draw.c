#include <X11/Xlib.h>
#include <stdlib.h>
#include <stdio.h>

int main(void) {
    Display *dpy = XOpenDisplay(NULL);
    if (!dpy) {
        fprintf(stderr, "Cannot open display\n");
        return EXIT_FAILURE;
    }
    int screen = DefaultScreen(dpy);
    Window win = XCreateSimpleWindow(
        dpy, RootWindow(dpy, screen),
        10, 10, 400, 300, 1,
        BlackPixel(dpy, screen),
        WhitePixel(dpy, screen)
    );
    XSelectInput(dpy, win, ExposureMask | KeyPressMask);
    XMapWindow(dpy, win);

    GC gc = XCreateGC(dpy, win, 0, NULL);
    XSetForeground(dpy, gc, BlackPixel(dpy, screen));

    for(;;) {
        XEvent e;
        XNextEvent(dpy, &e);
        if (e.type == Expose) {
            // Draw a rectangle
            XDrawRectangle(dpy, win, gc, 50, 50, 200, 100);
        }
        if (e.type == KeyPress)
            break;
    }

    XFreeGC(dpy, gc);
    XDestroyWindow(dpy, win);
    XCloseDisplay(dpy);
    return EXIT_SUCCESS;
}
