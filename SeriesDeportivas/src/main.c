#include <gtk/gtk.h>
#include <time.h>
#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <limits.h>
#include <stdbool.h>

int main(int argc, char *argv[]) {
  GtkBuilder      *builder = 0; 

  gtk_init(&argc, &argv);

  builder = gtk_builder_new();
  gtk_builder_add_from_file (builder, "glade/main_window.glade", NULL);

  window = GTK_WIDGET(gtk_builder_get_object(builder, "Main_Window"));

  gtk_builder_connect_signals(builder, NULL);

  g_object_unref(builder);

  gtk_widget_show(window);
  gtk_main();

  return 0;
}
