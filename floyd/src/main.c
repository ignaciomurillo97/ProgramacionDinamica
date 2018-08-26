#include <gtk/gtk.h>
#include "floyd.h"
#include "time.h"
#include <stdio.h>

float **createTestGraph(int size) {
  srand(time(NULL));
  float **graph = (float**)malloc(sizeof(float*) * size);
  for (int i = 0; i < size; i++) {
    graph[i] = (float*)malloc(sizeof(float) * size);
    for (int j = 0; j < size; j ++) {
      graph[i][j] = rand() % 9 + 1;
    }
  }
  return graph;
}

int main(int argc, char *argv[]) {
  float **myGraph = createTestGraph(3);

  struct FloydData *data = initFloydData(3, myGraph);
  optimize(data);

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

void showMatrixWindow () {
  GtkBuilder      *builder = 0; 
  GtkWidget       *window = 0;

  builder = gtk_builder_new();
  gtk_builder_add_from_file (builder, "glade/_main.glade", NULL);

  window = GTK_WIDGET(gtk_builder_get_object(builder, "window_menu"));
  gtk_builder_connect_signals(builder, NULL);

  g_object_unref(builder);

  gtk_widget_show(window);                
}

void terminate() {
  gtk_main_quit();
  return;
}

void cancel () {
  printf("cancel");
}

void accept () {
  showMatrixWindow();
}
