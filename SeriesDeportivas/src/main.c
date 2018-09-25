#include <gtk/gtk.h>
#include <time.h>
#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <limits.h>
#include <stdbool.h>

GtkEntry* gameCountEntry;
GtkEntry* homeWinProbEntry;
GtkEntry* visitWinProbEntry;
GtkBox*   localityCluster;
GtkToggleButton* localityButtons;

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

void closeGtkApp() {
  gtk_main_quit();
}

void closeGtkWindow(GtkWindow* window) {
  gtk_window_close(window);
}

bool validateEntryToInt(int intValue, const char* stringValue) {
  if (intValue == 0 && strcmp(stringValue, "0") != 0){
    return false;
  }
  return true;
}

void showResult() {
  GtkBuilder  *builder  = 0; 
  GtkWidget   *window   = 0;
  GtkTreeView *tree     = 0;

  builder = gtk_builder_new();
  gtk_builder_add_from_file (builder, "glade/Result.glade", NULL);

  window = GTK_WIDGET(gtk_builder_get_object(builder, "ResultWindow"));

  gtk_builder_connect_signals(builder, NULL);

  g_object_unref(builder);

  gtk_widget_show(window);
  gtk_main();

}

void calculate() {
  const char* gameCountString = gtk_entry_get_text(gameCountEntry);
  const char* pcString = gtk_entry_get_text(homeWinProbEntry);
  const char* prString = gtk_entry_get_text(visitWinProbEntry);

  int gameCount = atoi(gameCountString);
  int pc = atoi(pcString);
  int pr = atoi(prString);

  if (!validateEntryToInt(gameCount, gameCountString)) return;
  if (!validateEntryToInt(pc, pcString)) return;
  if (!validateEntryToInt(pr, prString)) return;

  showResult();
}

void clearLocalityCluster() {
  GList *children, *iter;

  children = gtk_container_get_children(GTK_CONTAINER(localityCluster));
  for(iter = children; iter != NULL; iter = g_list_next(iter))
    gtk_widget_destroy(GTK_WIDGET(iter->data));
  g_list_free(children);
}

void changeNGames () {
  const char* gameCountString = gtk_entry_get_text(gameCountEntry);
  int gameCount = atoi(gameCountString);
  if (!validateEntryToInt(gameCount, gameCountString)) return;

  clearLocalityCluster();
  for (int i = 0; i < gameCount; i++) {
    char buff[20];
    snprintf(buff, 20, "%d", (i+1));
    GtkWidget* toggleButton = gtk_toggle_button_new_with_mnemonic (buff);
    gtk_container_add(GTK_CONTAINER(localityCluster), toggleButton);
    gtk_widget_show(toggleButton);
  }
}
