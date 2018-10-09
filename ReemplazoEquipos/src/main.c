#include <gtk/gtk.h>
#include <time.h>
#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <stdbool.h>
#include "reemplazo.h"

GtkWidget *window;

GtkEntry *costoEntry;
GtkEntry *plazoEntry;
GtkEntry *vidaUtilEntry;
GtkEntry *inflacionEntry;

GtkContainer *mantenimientoContainer;
GtkContainer *ventaContainer;
GtkContainer *gananciasContainer;

GtkEntry** entriesMantenimientos = NULL;
GtkEntry** entriesVentas = NULL;
GtkEntry** entriesGanancias = NULL;


bool validarPlazo(const char* stringValue, int value) {
  if (value < 0) return false;
  if (value > 80) return false;
  if (value == 0 && strcmp(stringValue, "0") != 0) return false;
  return true;
}

int entryToInt (GtkEntry* entry) {
  const char* strValue = gtk_entry_get_text(entry);
  int value = atoi(strValue);
  if (validarPlazo(strValue, value)) {
    return value;
  }
  gtk_entry_set_text(entry, "");
  return -1;
}

double validarDouble(const char* stringValue, double value, char* end) {
  if (value < 0) return false;
  if (value == 0 && end == stringValue) return false;
  return true;
}

double entryToDouble (GtkEntry* entry) {
  const char* strValue = gtk_entry_get_text(entry);
  char* end;
  double value = strtod(strValue, &end);
  if (validarDouble(strValue, value, end)) {
    return value;
  }
  gtk_entry_set_text(entry, "");
  return -1;
}

double validarDouble01(const char* stringValue, double value, char* end) {
  if (value < 0) return false;
  if (value > 1) return false;
  if (value == 0 && stringValue == end) return false;
  return true;
}

double entryToDouble01 (GtkEntry* entry) {
  const char* strValue = gtk_entry_get_text(entry);
  char* end;
  double value = strtod(strValue, &end);;
  if (validarDouble01(strValue, value, end)) {
    return value;
  }
  gtk_entry_set_text(entry, "");
  return -1;
}

void initializeGtkElements(GtkBuilder *builder) {
  window = GTK_WIDGET(gtk_builder_get_object(builder, "Main_Window"));

  costoEntry = GTK_ENTRY(gtk_builder_get_object(builder, "CostoInicialEntry"));
  plazoEntry = GTK_ENTRY(gtk_builder_get_object(builder, "PlazoEntry"));
  vidaUtilEntry = GTK_ENTRY(gtk_builder_get_object(builder, "VidaUtilEntry"));
  inflacionEntry = GTK_ENTRY(gtk_builder_get_object(builder, "InflacionEntry"));

  mantenimientoContainer = GTK_CONTAINER(gtk_builder_get_object(builder, "MantenimientoContainer"));
  ventaContainer = GTK_CONTAINER(gtk_builder_get_object(builder, "VentaContainer"));
  gananciasContainer = GTK_CONTAINER(gtk_builder_get_object(builder, "GananciasContainer"));

}

int main(int argc, char *argv[]) {
  GtkBuilder      *builder = 0; 

  gtk_init(&argc, &argv);

  builder = gtk_builder_new();
  gtk_builder_add_from_file (builder, "glade/main_window.glade", NULL);

  initializeGtkElements(builder);

  gtk_builder_connect_signals(builder, NULL);

  g_object_unref(builder);

  gtk_widget_show(window);
  gtk_main();

  return 0;
}

void limpiarGtkContainer(GtkContainer* box, GtkEntry** entryList) {
  GList *children, *iter;
  children = gtk_container_get_children(box);
  for (iter = children; iter != NULL; iter = g_list_next(iter)) {
    gtk_container_remove(box, GTK_WIDGET(iter->data));
  }
  if (entryList != NULL)
    free(entryList);
}

void actualizarListaVentas(int plazo) {
  if (entriesVentas != NULL) {
    limpiarGtkContainer(ventaContainer, entriesVentas);
  }
  entriesVentas = (GtkEntry**)malloc(sizeof(GtkEntry*) * plazo);
  char buff[20];
  for (int i = 0; i < plazo; i++) {
    snprintf(buff, 20, "instante %d", i);
    GtkWidget* nuevo = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(nuevo), buff);
    gtk_container_add(ventaContainer, nuevo);
    gtk_widget_show(nuevo);
    entriesVentas[i] = GTK_ENTRY(nuevo);
  }
}

void actualizarListaGanancias(int plazo) {
  if (entriesGanancias != NULL) {
    limpiarGtkContainer(gananciasContainer, entriesGanancias);
  }
  entriesGanancias = (GtkEntry**)malloc(sizeof(GtkEntry*) * plazo);
  char buff[20];
  for (int i = 0; i < plazo; i++) {
    snprintf(buff, 20, "instante %d", i);
    GtkWidget* nuevo = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(nuevo), buff);
    gtk_container_add(gananciasContainer, nuevo);
    gtk_widget_show(nuevo);
    entriesGanancias[i] = GTK_ENTRY(nuevo);
  }
}

void actualizarListaMantenimientos(int plazo) {
  if (entriesMantenimientos != NULL) {
    limpiarGtkContainer(mantenimientoContainer, entriesMantenimientos);
  }
  entriesMantenimientos = (GtkEntry**)malloc(sizeof(GtkEntry*) * plazo);
  char buff[20];
  for (int i = 0; i < plazo; i++) {
    snprintf(buff, 20, "instante %d", i);
    GtkWidget* nuevo = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(nuevo), buff);
    gtk_container_add(mantenimientoContainer, nuevo);
    gtk_widget_show(nuevo);
    entriesMantenimientos[i] = GTK_ENTRY(nuevo);
  }
}

double* leerGtkContainer(GtkEntry** entryList, int size) {
  double* result = (double*)malloc(sizeof(double) * size);
  bool valid = true;
  for (int i = 0; i < size; i++){
    result[i] = entryToDouble(entryList[i]);
    if (result[i] == -1) valid = false;
  }
  if (!valid) return NULL;
  return result;
}

void calcular () {
  bool valid = true;
  modeloProblema *modelo = (modeloProblema*)malloc(sizeof(modeloProblema));
  modelo->costoInicial = entryToDouble(costoEntry);
  if (modelo->costoInicial == -1) valid = false;
  modelo->plazo = entryToInt(plazoEntry);
  if (modelo->plazo == -1) valid = false;
  modelo->vidaUtil = entryToInt(vidaUtilEntry);
  if (modelo->vidaUtil == -1) valid = false;
  modelo->inflacion = entryToDouble01(inflacionEntry);
  if (modelo->inflacion == -1) valid = false;
  modelo->venta = leerGtkContainer(entriesVentas, modelo->vidaUtil);
  if (modelo->venta == NULL) valid = false;
  modelo->mantenimiento = leerGtkContainer(entriesMantenimientos, modelo->vidaUtil);
  if (modelo->mantenimiento == NULL) valid = false;
  modelo->ganancia = leerGtkContainer(entriesGanancias, modelo->vidaUtil);
  if (modelo->ganancia == NULL) valid = false;

  if (!valid) {
    return;
  }

  reemplazosOptimos(modelo);
}

void actualizarVidaUtil () {
  int vidaUtil = entryToInt(vidaUtilEntry);
  if (vidaUtil > 0) {
    actualizarListaMantenimientos(vidaUtil);
    actualizarListaGanancias(vidaUtil);
    actualizarListaVentas(vidaUtil);
  }
}

void closeGtkApp() {
  gtk_main_quit();
}

void closeGtkWindow(GtkWindow* window) {
  gtk_window_close(window);
}

// Archivos:
void loadFromFile(char* filename) {
  FILE * fp;
  char * line = NULL;
  size_t len = 0;
  ssize_t read;

  fp = fopen(filename, "r");
  if (fp == NULL)
    exit(EXIT_FAILURE);

  // read capacity
  if ((read = getline(&line, &len, fp)) != -1) {
    line = strtok(line,"\n");
    gtk_entry_set_text(costoEntry, line);
  }
  if ((read = getline(&line, &len, fp)) != -1) {
    line = strtok(line,"\n");
    gtk_entry_set_text(plazoEntry, line);
  }
  if ((read = getline(&line, &len, fp)) != -1) {
    line = strtok(line,"\n");
    gtk_entry_set_text(vidaUtilEntry, line);
  }
  if ((read = getline(&line, &len, fp)) != -1) {
    line = strtok(line,"\n");
    gtk_entry_set_text(inflacionEntry, line);
  }

  int i = 0;
  while ((read = getline(&line, &len, fp)) != -1) {
    char* venta = strtok(line, ",");
    if (venta == NULL) return;
    char* mantenimiento = strtok(NULL, ",");
    if (mantenimiento == NULL) return;
    char* ganancia = strtok(NULL, "\n");
    if (ganancia == NULL) return;
    
    gtk_entry_set_text(entriesVentas[i], venta);
    gtk_entry_set_text(entriesMantenimientos[i], mantenimiento);
    gtk_entry_set_text(entriesGanancias[i], ganancia);
    i++;
  }

  fclose(fp);
  if (line != NULL)
    free(line);
}

void openLoadDialog(){
  GtkWidget *dialog;
  GtkFileChooser *chooser;
  GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_SAVE;
  gint res;

  dialog = gtk_file_chooser_dialog_new ("Open File",
      GTK_WINDOW(window),
      action,
      "Cancel",
      GTK_RESPONSE_CANCEL,
      "Open",
      GTK_RESPONSE_ACCEPT,
      NULL);
  chooser = GTK_FILE_CHOOSER (dialog);


  res = gtk_dialog_run (GTK_DIALOG (dialog));
  if (res == GTK_RESPONSE_ACCEPT)
  {
    char *filename;

    filename = gtk_file_chooser_get_filename (chooser);
    loadFromFile(filename);
    g_free (filename);
  }

  gtk_widget_destroy (dialog);
}

void saveToFile (char* filename) {
  FILE *fp;
  fp = fopen(filename, "w+");

  bool valid = true;
  modeloProblema *modelo = (modeloProblema*)malloc(sizeof(modeloProblema));
  modelo->costoInicial = entryToDouble(costoEntry);
  if (modelo->costoInicial == -1) valid = false;
  modelo->plazo = entryToInt(plazoEntry);
  if (modelo->plazo == -1) valid = false;
  modelo->vidaUtil = entryToInt(vidaUtilEntry);
  if (modelo->vidaUtil == -1) valid = false;
  modelo->inflacion = entryToDouble01(inflacionEntry);
  if (modelo->inflacion == -1) valid = false;
  modelo->venta = leerGtkContainer(entriesVentas, modelo->vidaUtil);
  if (modelo->venta == NULL) valid = false;
  modelo->mantenimiento = leerGtkContainer(entriesMantenimientos, modelo->vidaUtil);
  if (modelo->mantenimiento == NULL) valid = false;
  modelo->ganancia = leerGtkContainer(entriesGanancias, modelo->vidaUtil);
  if (modelo->ganancia == NULL) valid = false;

  if (!valid) {
    return;
  }

  fprintf(fp, "%f\n", modelo->costoInicial);
  fprintf(fp, "%d\n", modelo->plazo);
  fprintf(fp, "%d\n", modelo->vidaUtil);
  fprintf(fp, "%f\n", modelo->inflacion);

  for (int i = 0; i < modelo->vidaUtil; i++) {
    fprintf(fp, "%f,%f,%f\n", modelo->venta[i], modelo->mantenimiento[i], modelo->ganancia[i]);
  }

  fclose(fp);
}

void openSaveDialog(GtkWindow* window){
  GtkWidget *dialog;
  GtkFileChooser *chooser;
  GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_SAVE;
  gint res;

  dialog = gtk_file_chooser_dialog_new ("Save File",
      window,
      action,
      "Cancel",
      GTK_RESPONSE_CANCEL,
      "Save",
      GTK_RESPONSE_ACCEPT,
      NULL);
  chooser = GTK_FILE_CHOOSER (dialog);

  gtk_file_chooser_set_do_overwrite_confirmation (chooser, TRUE);
  gtk_file_chooser_set_current_name (chooser,("Untitled document"));

  res = gtk_dialog_run (GTK_DIALOG (dialog));
  if (res == GTK_RESPONSE_ACCEPT)
  {
    char *filename;

    filename = gtk_file_chooser_get_filename (chooser);
    saveToFile(filename);
    g_free (filename);
  }

  gtk_widget_destroy (dialog);
}

