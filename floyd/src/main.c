#include <gtk/gtk.h>
#include "floyd.h"
#include "time.h"
#include <stdio.h>
#include <graphviz/gvc.h>

int gridSize = 10;
void fillMatrix(GtkWidget* grid);

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
  //float **myGraph = createTestGraph(6);

  //struct FloydData *data = initFloydData(6, myGraph);
  //optimize(data);

  //int* optimalRoute = findOptimalRoute(0, 4, data);
  //int orSize = optimalRoute[0];
  //optimalRoute++;
  //printf("size:%d\n", orSize);

  //printOptimal(0, data);
  //printOptimal(1, data);
  //printOptimal(2, data);
  //printOptimal(3, data);
  //printOptimal(4, data);
  //printOptimal(5, data);
  //printOptimal(6, data);
  //printRoutes(data);

  //for (int i = 0; i < orSize; i++) {
  //  printf("%d, ", optimalRoute[i]);
  //}

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
  GtkWidget       *matrix = 0;

  builder = gtk_builder_new();
  gtk_builder_add_from_file (builder, "glade/Win1.glade", NULL);

  window = GTK_WIDGET(gtk_builder_get_object(builder, "FloydGUI"));
  gtk_builder_connect_signals(builder, NULL);

  matrix = GTK_WIDGET(gtk_builder_get_object(builder, "InputGrid"));
  gtk_builder_connect_signals(builder, NULL);
  fillMatrix(matrix);

  g_object_unref(builder);

  gtk_widget_show(window);                
}

void cancel () {
  gtk_main_quit();
}

void accept () {
  showMatrixWindow();
}

void fillMatrix (GtkWidget *grid) {
  GtkWidget *button;
  int i,j;

  for (i = 0; i < gridSize; i++) {
    gtk_grid_insert_column((GtkGrid*)grid, (gint)i);
    for (j = 0; j < gridSize; j++) {
      gtk_grid_insert_row((GtkGrid*)grid, (gint)j);
    }
  }

  for (i = 0; i < gridSize; i++) {
    for (j = 0; j < gridSize; j++) {
      button = gtk_button_new_with_label("label");
      gtk_grid_attach(GTK_GRID (grid), button, i, j, 1, 1);
      gtk_widget_show(button);
    }
  }


}
