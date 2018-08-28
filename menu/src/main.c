#include <gtk/gtk.h>
int main(int argc, char *argv[])
{
    GtkBuilder      *builder = 0; 
    GtkWidget       *window = 0;

    gtk_init(&argc, &argv);

    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "glade/_main.glade", NULL);

    window = GTK_WIDGET(gtk_builder_get_object(builder, "window_menu"));
    gtk_builder_connect_signals(builder, NULL);

    g_object_unref(builder);

    gtk_widget_show(window);                
    gtk_main();

    return 0;
}

void terminate() {
    gtk_main_quit();
    return;
}

void tbd() {
    chdir("../placeholder/");
    system("./placeholder &");
    return;
}

void shortestRoute() {
    chdir("../floyd/");
    system("./rutasOptimas &");
    return;
}

// called when window is closed
void on_window_main_destroy()
{
    gtk_main_quit();
}
