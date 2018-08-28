#include <gtk/gtk.h>
#include "floyd.h"
#include "time.h"
#include <stdio.h>
#include <math.h>

int gridSize = 10;
GtkWidget *textBoxCuantity = 0;

int currMatrix = 0;
struct FloydData* data;
float** graph;
GtkEntry ***inputGrid;

void fillMatrix(GtkWidget* grid);

float **createGraph(int size) {
  float **graph = (float**)malloc(sizeof(float*) * size);
  for (int i = 0; i < size; i++) {
    graph[i] = (float*)malloc(sizeof(float) * size);
    for (int j = 0; j < size; j ++) {
      if (i == j) graph[i][j] = 0;
      else graph[i][j] = INFINITY;
    }
  }
  return graph;
}

int main(int argc, char *argv[]) {
  //struct FloydData *data = initFloydData(6, graph);
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

  textBoxCuantity = GTK_WIDGET(gtk_builder_get_object(builder, "Entry_Cuantity"));

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

  const gchar* text = gtk_entry_get_text(GTK_ENTRY(textBoxCuantity));
  gridSize = atoi(text);

  matrix = GTK_WIDGET(gtk_builder_get_object(builder, "InputGrid"));
  graph = createGraph(gridSize);
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


void changed_event(GtkWidget *grid) {
  return;
}

void fillMatrix (GtkWidget *grid) {
  GtkWidget *entry;
  int i,j;

  inputGrid = (GtkEntry***)malloc(sizeof(GtkWidget**) * gridSize);
  for (i = 0; i < gridSize; i++) {
    inputGrid[i] = (GtkEntry**)malloc(sizeof(GtkEntry*) * gridSize);
    gtk_grid_insert_column((GtkGrid*)grid, (gint)i);
    for (j = 0; j < gridSize; j++) {
      gtk_grid_insert_row((GtkGrid*)grid, (gint)j);
    }
  }

  for (i = 0; i < gridSize; i++) {
    for (j = 0; j < gridSize; j++) {
      int len = snprintf(NULL, 0, "%f", graph[i][j]);
      char *result = (char *)malloc(len + 1);
      snprintf(result, len + 1, "%f", graph[i][j]);
      entry = gtk_entry_new();
      gtk_entry_set_text(GTK_ENTRY(entry), result);
      g_signal_connect (entry, "changed", (GCallback) changed_event, grid);
      gtk_entry_set_width_chars(GTK_ENTRY(entry), 3);
      gtk_grid_attach(GTK_GRID (grid), entry, i, j, 1, 1);
      gtk_widget_show(entry);
      inputGrid[i][j]=(GtkEntry*)entry;
      if (i == j) {
        gtk_widget_set_sensitive (entry, 0);
      }
    }
  }
}

void next(GtkWidget *grid) {
  if (currMatrix < gridSize){
    GtkEntry *entry;
    int i,j;
    if (currMatrix == 0) {
      for (i = 0; i < gridSize; i++) {
        for (j = 0; j < gridSize; j++) {
          entry = inputGrid[i][j];
          const gchar* labelText = gtk_entry_get_text(GTK_ENTRY((GtkEntry*)entry));
          graph[i][j] = strtof(labelText, NULL);
        }
      }
      data = initFloydData(gridSize, graph);
      printOptimal(0, data);
      optimize(data);
    }

    for (int i = 0; i < gridSize + 1; i++) {
      printOptimal(i, data);
    }
    currMatrix++;

    for (i = 0; i < gridSize; i++) {
      for (j = 0; j < gridSize; j++) {
        int len = snprintf(NULL, 0, "%f", data->optimizedDistances[currMatrix][i][j]);
        char *result = (char *)malloc(len + 1);
        snprintf(result, len + 1, "%f", data->optimizedDistances[currMatrix][i][j]);
        entry = inputGrid[i][j];
        gtk_entry_set_text(GTK_ENTRY(entry), result);
      }
    }
  }
}
