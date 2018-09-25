#include <gtk/gtk.h>
#include <time.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>
#include <stdbool.h>

GtkEntry* gameCountEntry;
GtkEntry* homeWinProbEntry;
GtkEntry* visitWinProbEntry;
GtkBox*   localityCluster;
GtkGrid*  table;

GtkToggleButton** localityButtons;
int localityButtonsLength;

int main(int argc, char *argv[]) {
  GtkBuilder  *builder  = 0; 
  GtkWidget   *window   = 0;

  gtk_init(&argc, &argv);

  builder = gtk_builder_new();
  gtk_builder_add_from_file (builder, "glade/main_window.glade", NULL);

  // Initialize GTK Objects
  window            = GTK_WIDGET(gtk_builder_get_object(builder, "Main_Window"));
  gameCountEntry    = GTK_ENTRY(gtk_builder_get_object(builder, "EntryNGame"));
  homeWinProbEntry  = GTK_ENTRY(gtk_builder_get_object(builder, "EntryPC"));
  visitWinProbEntry = GTK_ENTRY(gtk_builder_get_object(builder, "EntryPR"));
  localityCluster   = GTK_BOX(gtk_builder_get_object(builder, "LocalityCluster"));

  gtk_builder_connect_signals(builder, NULL);

  g_object_unref(builder);

  gtk_widget_show(window);
  gtk_main();

  return 0;
}

float** calculateSeries (int n, float p_h, float p_r, float *serie) {
  int i, j;
  float *mm, **m;
  //float serie[7] = {0.57,0.57,0.49,0.49,0.49,0.57,0.57}; //Ese 7 es n y cada campo es o p_h o p_r

  int wins = (int)ceil((double)n/2);
  int m_size = (wins+1) * (wins+1);
  mm = calloc(m_size, sizeof (float));
  m = malloc((wins+1) * sizeof (float *));
  m[0] = mm;

  for (i = 0; i <= wins; i++) {
    m[i] = &mm[i * (wins+1)];
    for (j = 0; j <= wins; j++) {
      if (i == 0) m[i][j] = 1;
    }
  }

  for (i = 1; i <= wins; i++) {
    for (j = 1; j <= wins; j++) {
      int actual_game = (wins - i) + (wins - j) + 1;
      float p = serie[actual_game-1];
      float q = 1-p;
      m[i][j] = m[i-1][j]*p + m[i][j-1]*q;
      printf("%f ",m[i][j]);
    }
    printf("\n");
  }

  return m;
}


void closeGtkApp() {
  gtk_main_quit();
}

void closeGtkWindow(GtkWindow* window) {
  gtk_window_close(window);
}

bool validateMaxGameCount(int maxGameCount) {
  if (maxGameCount % 2 == 0 || maxGameCount < 1){
    return false;
  }
  return true;
}

bool validateEntryToInt(int intValue, const char* stringValue) {
  if (intValue == 0 && strcmp(stringValue, "0") != 0){
    return false;
  }
  return true;
}

bool validateEntryToFloat(float floatValue, const char* stringValue) {
  if ((floatValue == 0 && strcmp(stringValue, "0") != 0) || floatValue > 1 || floatValue < 0){
    return false;
  }
  return true;
}

void showResult(float** result, int wins) {
  GtkBuilder  *builder  = 0; 
  GtkWidget   *window   = 0;
  GtkWidget   *tableCont = 0;

  table = GTK_GRID(gtk_grid_new());

  builder = gtk_builder_new();
  gtk_builder_add_from_file (builder, "glade/Result.glade", NULL);

  window = GTK_WIDGET(gtk_builder_get_object(builder, "ResultWindow"));
  tableCont = GTK_WIDGET(gtk_builder_get_object(builder, "Viewport"));
  gtk_container_add(GTK_CONTAINER(tableCont), GTK_WIDGET(table));

  for (int i = 0; i < wins; i++) {
    gtk_grid_insert_column(table, i-1);
    for (int j = 0; j < wins; j++) {
      gtk_grid_insert_row(table, j);
    }
  }
  gtk_grid_set_row_homogeneous(table, true);
  gtk_grid_set_column_homogeneous(table, true);

  for (int i = 1; i <= wins; i++) {
    for (int j = 1; j <= wins; j++) {
      char buff[20];
      snprintf(buff, 20, "%f", result[i][j]);
      GtkLabel* label = GTK_LABEL(gtk_label_new(buff));
      gtk_grid_attach(table, GTK_WIDGET(label), i, j, 1, 1);
      gtk_widget_show(GTK_WIDGET(label));
    }
  }

  gtk_builder_connect_signals(builder, NULL);

  g_object_unref(builder);

  gtk_widget_show_all(window);
}

void calculate() {
  const char* gameCountString = gtk_entry_get_text(gameCountEntry);
  const char* pcString = gtk_entry_get_text(homeWinProbEntry);
  const char* prString = gtk_entry_get_text(visitWinProbEntry);

  int gameCount = atoi(gameCountString);
  float pc = strtof(pcString, NULL);
  float pr = strtof(prString, NULL);

  int wins = (int)ceil((double)gameCount/2);

  float* series = (float*)malloc(sizeof(float) * localityButtonsLength);
  for (int i = 0; i < localityButtonsLength; i++) {
    bool curr = (bool)gtk_toggle_button_get_active(localityButtons[i]);
    if (curr) {
      series[i] = pc;
    } else  {
      series[i] = pr;
    }
  }

  if (!validateEntryToInt(gameCount, gameCountString)){
    printf("gameCount invalido");
    return;
  }
  if (!validateMaxGameCount(gameCount)) {
    printf("gameCount invalido");
    return;
  }
  if (!validateEntryToFloat(pc, pcString)){
    printf("pc invalido");
    return;
  }
  if (!validateEntryToFloat(pr, prString)){
    printf("pr invalido");
    return;
  }

  float ** result = calculateSeries(localityButtonsLength, pc, pr, series);
  //calculateSeries(localityButtonsLength, pc, pr, series);

  showResult(result, wins);
}

void clearLocalityCluster() {
  GList *children, *iter;

  if (localityButtons != 0) {
    children = gtk_container_get_children(GTK_CONTAINER(localityCluster));
    for(iter = children; iter != NULL; iter = g_list_next(iter))
      gtk_container_remove(GTK_CONTAINER(localityCluster), GTK_WIDGET(iter->data));
    //gtk_widget_destroy(GTK_WIDGET(iter->data));
    free(localityButtons);
    localityButtons = 0;
    localityButtonsLength = 0;
  }
}

void changeNGames () {
  clearLocalityCluster();
  const char* gameCountString = gtk_entry_get_text(gameCountEntry);
  int gameCount = atoi(gameCountString);
  if (!validateEntryToInt(gameCount, gameCountString) || !validateMaxGameCount(gameCount)) {
    return;
  }

  localityButtons = (GtkToggleButton**)malloc(sizeof(GtkToggleButton*) * gameCount);

  for (int i = 0; i < gameCount; i++) {
    char buff[20];
    snprintf(buff, 20, "%d", (i+1));
    GtkWidget* toggleButton = gtk_toggle_button_new_with_mnemonic (buff);
    gtk_container_add(GTK_CONTAINER(localityCluster), toggleButton);
    gtk_widget_show(toggleButton);
    localityButtons[i] = GTK_TOGGLE_BUTTON(toggleButton);
    localityButtonsLength ++;
  }

}
