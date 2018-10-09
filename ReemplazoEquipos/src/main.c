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

double validarDouble(const char* stringValue, double value) {
  if (value < 0) return false;
  if (value == 0 && strcmp(stringValue, "0") != 0) return false;
  return true;
}

double entryToDouble (GtkEntry* entry) {
  const char* strValue = gtk_entry_get_text(entry);
  double value = strtod(strValue, NULL);
  if (validarDouble(strValue, value)) {
    return value;
  }
  gtk_entry_set_text(entry, "");
  return -1;
}

double validarDouble01(const char* stringValue, double value) {
  if (value < 0) return false;
  if (value > 1) return false;
  if (value == 0 && strcmp(stringValue, "0") != 0) return false;
  return true;
}

double entryToDouble01 (GtkEntry* entry) {
  const char* strValue = gtk_entry_get_text(entry);
  double value = strtod(strValue, NULL);
  if (validarDouble01(strValue, value)) {
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
  for (int i = 0; i < size; i++){
    result[i] = entryToDouble(entryList[i]);
  }
  return result;
}

void calcular () {
  modeloProblema *modelo = (modeloProblema*)malloc(sizeof(modeloProblema));
  modelo->costoInicial = entryToDouble(costoEntry);
  modelo->plazo = entryToInt(plazoEntry);
  modelo->vidaUtil = entryToInt(vidaUtilEntry);
  modelo->inflacion = entryToDouble01(inflacionEntry);
  modelo->venta = leerGtkContainer(entriesVentas, modelo->vidaUtil);
  modelo->mantenimiento = leerGtkContainer(entriesMantenimientos, modelo->vidaUtil);
  modelo->ganancia = leerGtkContainer(entriesGanancias, modelo->vidaUtil);

  //double resultado = calcularCosto(2, 4, modelo);
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
