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
