# PP12: GUI Programming with X11 and GTK+

## Goal

In this exercise you will:

1. Work directly with the X11 (Xlib) API to create a window and draw primitive graphics.
2. Install GTK+ and build a simple GTK3 application, then extend it with an additional feature.

**Important:** Start a stopwatch when you begin and work uninterruptedly for **90 minutes**. When time is up, stop immediately and record where you paused.

---

## Workflow

1. **Fork** this repository on GitHub.
2. **Clone** your fork locally.
3. Create a `solutions/` directory at the project root:

   ```bash
   mkdir solutions
   ```
4. Add each task’s source file(s) under `solutions/`.
5. **Commit** and **push** your solutions.
6. **Submit** your GitHub repo link for review.

---

## Prerequisites

* X11 development libraries (`libx11-dev`).
* GTK+ 3 development libraries (`libgtk-3-dev`).
* GNU C compiler (`gcc`).

---

## Tasks

### Task 1: Bare X11 Window & Drawing

**Objective:** Use Xlib directly to open a window and draw a rectangle.

1. Create `solutions/x11_draw.c` with the following skeleton:

   ```c
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
   ```
2. Compile and run:

   ```bash
   gcc -o solutions/x11_draw solutions/x11_draw.c -lX11
   ./solutions/x11_draw
   ```

#### Reflection Questions

1. **What steps are required to open an X11 window and receive events?**

1.) Install Libx11-dev. The command is:

   ```
    sudo apt-get install libx11-dev

   ```
2.) Include the Xlib header: 

   ```c
    #include <X11/Xlib.h>
   ```

3.) Open an connection to the X server
   ```c
   int main(void) {
    Display *dpy = XOpenDisplay(NULL);
    if (!dpy) {
        fprintf(stderr, "Cannot open display\n");
        return EXIT_FAILURE;
    ```  
4.) specify the attributes of the window. 

   ```c
    int screen = DefaultScreen(dpy);
    Window win = XCreateSimpleWindow(
        dpy, RootWindow(dpy, screen),
        10, 10, 400, 300, 1,
        BlackPixel(dpy, screen),
        WhitePixel(dpy, screen)
   ```
5.) Specify the types of events that the x server need to handle. In this case it is a simple window.

```c
    XSelectInput(dpy, win, ExposureMask | KeyPressMask);
 ```
    ExposureMask: When part of the window becomes visible again, the window will be redrawn
    KeyPressMask: If a key of the keyboard is pressed while the window is in the forground, the program reacts to the keypress.

6.) Create a loop of events. In this case the variable e holds the information about the event and XNextEvent waits for a free event from       the X server to open a Display and send the data to the e variable. When the display and the event is exposed the server should draw        the event.

    If a key of the keyboard is pressed while the window is in the forground, the program reacts to the keypress. In this case the              execution closes.

      ```c
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
      ```
7.) Clean up the Program

    ```c
    XFreeGC(dpy, gc);
    XDestroyWindow(dpy, win);
    XCloseDisplay(dpy);
    return EXIT_SUCCESS;
    ```

8.) Compile and run 

    gcc -o solutions/x11_draw solutions/x11_draw.c -lX11
    ./solutions/x11_draw
    
2. **How does the `Expose` event trigger your drawing code?**

---

### Task 2: GTK+ 3 Application & Extension

**Objective:** Install GTK+ 3, build a basic GTK application, then add a new interactive feature.

1. Install GTK+ 3:

   ```bash
   sudo apt update && sudo apt install libgtk-3-dev
   ```
2. Create `solutions/gtk_app.c` with this skeleton:

   ```c
   #include <gtk/gtk.h>

   static void on_button_clicked(GtkButton *button, gpointer user_data) {
       GtkLabel *label = GTK_LABEL(user_data);
       gtk_label_set_text(label, "Button clicked!");
   }

   int main(int argc, char **argv) {
       gtk_init(&argc, &argv);

       GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
       gtk_window_set_title(GTK_WINDOW(window), "GTK+ Demo");
       gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);
       g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

       GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
       gtk_container_add(GTK_CONTAINER(window), vbox);

       GtkWidget *label = gtk_label_new("Hello, GTK+!");
       gtk_box_pack_start(GTK_BOX(vbox), label, TRUE, TRUE, 0);

       GtkWidget *button = gtk_button_new_with_label("Click me");
       g_signal_connect(button, "clicked",
                        G_CALLBACK(on_button_clicked), label);
       gtk_box_pack_start(GTK_BOX(vbox), button, TRUE, TRUE, 0);

       gtk_widget_show_all(window);
       gtk_main();
       return 0;
   }
   ```
3. Compile and run:

   ```bash
   gcc -o solutions/gtk_app solutions/gtk_app.c $(pkg-config --cflags --libs gtk+-3.0)
   ./solutions/gtk_app
   ```
4. **Extension Feature:** Modify the program to add a **text entry** (`GtkEntry`) above the button, and on button click update the label to show the entry’s current text instead of the fixed string.

   * Hint: use `gtk_entry_get_text()`.

#### Reflection Questions

1. **How does GTK’s signal-and-callback mechanism differ from X11’s event loop?**
2. **Why use `pkg-config` when compiling GTK applications?**

---

**Remember:** Stop after **90 minutes** and record where you stopped.
