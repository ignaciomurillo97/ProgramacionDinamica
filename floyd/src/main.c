#include <gtk/gtk.h>
#include "floyd.h"
#include "time.h"
#include <stdio.h>
#include <math.h>
#include "graphDraw.h"

int gridSize = 10;
GtkWidget *textBoxCuantity = 0;
GtkWidget *textBoxRoute = 0;

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
  GtkBuilder      *builder = 0; 
  GtkWidget       *window = 0;

  gtk_init(&argc, &argv);

  builder = gtk_builder_new();
  gtk_builder_add_from_file (builder, "glade/_main.glade", NULL);

  textBoxCuantity = GTK_WIDGET(gtk_builder_get_object(builder, "Entry_Cuantity"));
  textBoxRoute = GTK_WIDGET(gtk_builder_get_object(builder, "matrizRutas"));

  window = GTK_WIDGET(gtk_builder_get_object(builder, "window_menu"));
  gtk_builder_connect_signals(builder, NULL);

  g_object_unref(builder);

  gtk_widget_show(window);
  gtk_main();

  return 0;
}

static void do_drawing(cairo_t *cr, gpointer user_data) {
  float meinMatrix[4][4] = {{0, 5, 1, 2},
                        {5, 0, 3, INFINITY},
                        {1, 3, 0, 4},
                        {2, INFINITY, 4, 0}};
  draw(meinMatrix, 4, cr);

  return;
}

static gboolean on_darea_draw(GtkWidget *widget, cairo_t *cr, gpointer user_data){
  do_drawing(cr, user_data);

  return FALSE;
}

void showMatrixWindow () {
  GtkBuilder      *builder = 0;
  GtkWidget       *window = 0;
  GtkWidget       *matrix = 0;
  GtkDrawingArea  *darea = 0;

  builder = gtk_builder_new();
  gtk_builder_add_from_file (builder, "glade/Win1.glade", NULL);

  window = GTK_WIDGET(gtk_builder_get_object(builder, "FloydGUI"));
  gtk_builder_connect_signals(builder, NULL);

  const gchar* text = gtk_entry_get_text(GTK_ENTRY(textBoxCuantity));
  gridSize = atoi(text);

  darea = (GtkDrawingArea*)GTK_WIDGET(gtk_builder_get_object(builder, "darea"));
  g_signal_connect(G_OBJECT(darea), "draw", G_CALLBACK(on_darea_draw), NULL);

  matrix = GTK_WIDGET(gtk_builder_get_object(builder, "InputGrid"));
  graph = createGraph(gridSize);
  fillMatrix(matrix);

  g_object_unref(builder);

  gtk_widget_show(window);
}

void showRoutes () {
  GtkBuilder      *builder = 0;
  GtkWidget       *window = 0;

  builder = gtk_builder_new();
  gtk_builder_add_from_file (builder, "glade/Win1.glade", NULL);

  textBoxRoute = GTK_WIDGET(gtk_builder_get_object(builder, "RouteLabel"));

  window = GTK_WIDGET(gtk_builder_get_object(builder, "FloydGUI"));
  gtk_builder_connect_signals(builder, NULL);

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
    else {
      showRoutes();
    }
  }
}

void viewRoutes() {
  int* optimalRoute = findOptimalRoute(0, 2, data);
  int len = optimalRoute[0];
  optimalRoute++;
  char str[len];
  int i=0;
  int index = 0;
  for (i=0; i<optimalRoute; i++)
    index += snprintf(&str[index], len-index, "%d -> ", optimalRoute[i]);
  gtk_label_set_text(textBoxRoute, str);
}
