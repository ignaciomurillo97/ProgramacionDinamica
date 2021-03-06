#include <gtk/gtk.h>
#include <time.h>
#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include "knapsack.h"

GtkEntry    *knapsackCapacity;

GtkEntry    *itemName;
GtkEntry    *itemQuantity;
GtkEntry    *itemCost;
GtkEntry    *itemValue;

GtkBox      *gtkBoxItemList;

GtkWidget *window;
GtkWidget *inputWindow;

// bounded
bool bounded;
char* quantityDefault;
GtkLabel *quantityLabel;

typedef struct itemLine {
  GtkLabel* name;
  GtkLabel* quantity;
  GtkLabel* cost;
  GtkLabel* value;
  // list
  struct itemLine* next;
  int index;
} itemLine;

typedef struct valueLine {
  gchar* name;
  long quantity;
  long cost;
  long value;
  // list
  struct valueLine* next;
  long index;
} valueLine;

typedef struct valueLinkedList {
  valueLine* first;
  valueLine* last;
  int count;
} valueLinkedList;

valueLinkedList* valueList;

typedef struct itemLinkedList {
  itemLine* first;
  itemLine* last;
  int count;
} itemLinkedList;

itemLinkedList* itemList;
void setFormulaRepresentation(GtkLabel *mathRepLabel, knapsackResult *result);
void saveToFile (char* filename);
void loadFromFile(char* filename);

int main(int argc, char *argv[]) {
  GtkBuilder      *builder = 0; 

  itemList = (itemLinkedList*)malloc(sizeof(itemLinkedList));
  valueList = (valueLinkedList*)malloc(sizeof(valueLinkedList));

  gtk_init(&argc, &argv);

  builder = gtk_builder_new();
  gtk_builder_add_from_file (builder, "glade/main_window.glade", NULL);

  inputWindow = GTK_WIDGET(gtk_builder_get_object(builder, "Input_Window"));
  window = GTK_WIDGET(gtk_builder_get_object(builder, "Main_Window"));

  knapsackCapacity = (GtkEntry*)GTK_WIDGET(gtk_builder_get_object(builder, "Entry_Knapsack_Capacity"));


  itemName = (GtkEntry*)GTK_WIDGET(gtk_builder_get_object(builder, "Entry_Item_Name"));
  itemQuantity = (GtkEntry*)GTK_WIDGET(gtk_builder_get_object(builder, "Entry_Item_Quantity"));
  itemCost = (GtkEntry*)GTK_WIDGET(gtk_builder_get_object(builder, "Entry_Item_Cost"));
  itemValue = (GtkEntry*)GTK_WIDGET(gtk_builder_get_object(builder, "Entry_Item_Value"));
  
  gtkBoxItemList = (GtkBox*)GTK_WIDGET(gtk_builder_get_object(builder, "Box_Item_List"));

  quantityLabel = GTK_LABEL(gtk_builder_get_object(builder, "Lable_Header_Quantity"));

  gtk_builder_connect_signals(builder, NULL);

  g_object_unref(builder);

  gtk_widget_show(window);
  gtk_main();

  return 0;
}

void clear() {
  itemLine *currItem = itemList->first;
  while (currItem != NULL) {
    gtk_widget_destroy(GTK_WIDGET(currItem->name));
    gtk_widget_destroy(GTK_WIDGET(currItem->quantity));
    gtk_widget_destroy(GTK_WIDGET(currItem->cost));
    gtk_widget_destroy(GTK_WIDGET(currItem->value));
    itemLine *tmp = currItem;
    currItem = currItem->next;
    free(tmp);
  }
  itemList->count = 0;
  itemList->first = NULL;
  itemList->last = NULL;

  valueLine *currValue = valueList->first;
  while (currValue != NULL) {
    valueLine *tmp = currValue;
    currValue = currValue->next;
    free(tmp);
  }
  valueList->count = 0;
  valueList->first = NULL;
  valueList->last = NULL;
}

void knapsackOneZero(){
  printf("1/0");
  quantityDefault = "1";
  bounded = false;
  gtk_entry_set_text(itemQuantity, quantityDefault);
  gtk_widget_show(inputWindow);
  gtk_widget_hide(window);
  gtk_widget_hide(GTK_WIDGET(itemQuantity));
  gtk_widget_hide(GTK_WIDGET(quantityLabel));
}

void knapsackBounded(){
  quantityDefault = "";
  bounded = true;
  gtk_entry_set_text(itemQuantity, quantityDefault);
  gtk_widget_show(inputWindow);
  gtk_widget_hide(window);
  printf("bounded");
}

void knapsackUnbounded(){
  quantityDefault = "inf";
  bounded = false;
  gtk_entry_set_text(itemQuantity, quantityDefault);
  gtk_widget_show(inputWindow);
  gtk_widget_hide(window);
  gtk_widget_hide(GTK_WIDGET(itemQuantity));
  gtk_widget_hide(GTK_WIDGET(quantityLabel));
  printf("Unbounded");
}

void showResultWindow(knapsackResult* result) {
  GtkBuilder      *builder = 0; 
  GtkWidget       *window = 0;
  GtkGrid         *grid = 0;
  GtkBox          *resultBox = 0;
  GtkLabel        *mathRep = 0;

  builder = gtk_builder_new();
  gtk_builder_add_from_file (builder, "glade/result_window.glade", NULL);

  window = GTK_WIDGET(gtk_builder_get_object(builder, "result_window"));
  grid = GTK_GRID(gtk_builder_get_object(builder, "result_table"));
  mathRep = GTK_LABEL(gtk_builder_get_object(builder, "math_representation_label"));
  resultBox = GTK_BOX(gtk_builder_get_object(builder, "quantities_table"));
  setFormulaRepresentation(mathRep, result);

  for (int i = 1; i <= result->n; i++) {
    gtk_grid_insert_column(grid, i-1);
    for (int j = 0; j <= result->knapsackCapacity; j++) {
      gtk_grid_insert_row(grid, j);
    }
  }

  for (int i = 1; i <= result->n; i++) {
    for (int j = 0; j <= result->knapsackCapacity; j++) {
      char buff[50];
      if (result->color[i][j])
        snprintf(buff, 50, "<span foreground=\"green\">%i(%i)</span>", result->tabla[i][j], result->color[i][j]);
      else
        snprintf(buff, 50, "<span foreground=\"red\">%i(%i)</span>", result->tabla[i][j], result->color[i][j]);
      GtkWidget *label = gtk_label_new(NULL);
      gtk_label_set_markup(GTK_LABEL(label), buff);
      gtk_grid_attach(grid, label, i-1, j, 1, 1);
      gtk_widget_show(label);
    }
  }

  gtk_grid_insert_row(grid, 0);
  valueLine *curr = valueList->first;
  for (int i = 0; i < valueList->count; i++) {
      GtkWidget *label = gtk_label_new(curr->name);
      gtk_grid_attach(grid, label, i, 0, 1, 1);
      gtk_widget_show(label);
      curr = curr->next;
  }

  for (int i = 0; i < result->n; i++) {
    char res[60];
    snprintf(res, 60, "Se llevan %i %ss, con peso %i y valor %i", result->s[i], result->items[i].name, result->items[i].weight, result->items[i].value);
    GtkLabel* currLabel = GTK_LABEL(gtk_label_new(res));
    gtk_widget_show(GTK_WIDGET(currLabel));
    gtk_container_add(GTK_CONTAINER(resultBox), GTK_WIDGET(currLabel));
  }

  gtk_builder_connect_signals(builder, NULL);

  g_object_unref(builder);

  gtk_widget_show(window);

  return;

}

// recibe el valor del input a validar y en nombre del input para imprimir errores.
bool isNumericalInputValid(const gchar* inputValue, char* inputName) {
  int value;
  if (strcmp(inputValue, "") == 0) { // If string is null
    fprintf(stderr, "La entrada \"%s\" no puede ser vacia.\n", inputName);
    return false;
  }
  value = strtol(inputValue, NULL, 10);
  if (value == 0 && strcmp(inputValue, "0") != 0 && strcmp(inputValue, "inf") != 0) { // si no es un numero
    fprintf(stderr, "La entrada \"%s\" debe ser un número.\n", inputName);
    return false;
  }
  if (value < 0) {
    fprintf(stderr, "La entrada \"%s\" no puede ser un valor negativo.\n", inputName);
  }
  return true;
}

// recibe el valor del input a validar y en nombre del input para imrimir errores
bool isStringInputValid(const gchar* inputValue, char* inputName) {
  if (strcmp(inputValue, "") == 0) { // If string is null
    fprintf(stderr, "La entrada \"%s\" no puede ser vacia.\n", inputName);
    return false;
  }
  return true;
}

void addElementToValues (valueLine* value, valueLinkedList* list) {
  if (list->first == NULL) {
    list->first = value;
    list->last = value;
    value->next = NULL;
  } else {
    list->last->next = value;
    list->last = value;
    value->next=NULL;
  }
  value->index = list->count;
  list->count++;
}

void addElementToItems (itemLine* value, itemLinkedList* list) {
  if (list->first == NULL) {
    list->first = value;
    list->last = value;
    value->next = NULL;
  } else {
    list->last->next = value;
    list->last = value;
    value->next=NULL;
  }
  value->index = list->count;
  list->count++;
}

GtkLabel* labelFromString (const gchar* value, GtkBox* line) {
  GtkLabel* label     = (GtkLabel*)gtk_label_new(value);
  gtk_widget_set_hexpand((GtkWidget*)label, 1);
  gtk_container_add((GtkContainer*)line, (GtkWidget*)label);
  gtk_widget_show((GtkWidget*)label);
  return label;
}

/*
 * TODO:
 * meter labelFromString en un struct para eliminarlos despues
 */

int stringToLong(const char* s) {
  if (strcmp(s, "inf") == 0) { //si es inf
    return INT_MAX;
  }
  return strtol(s, NULL, 10);
}

void knapsackLineFromStrings (const char* stringItemName, const char* stringItemCost, const char* stringItemValue, const char* stringItemQuantity) {
  GtkBox* newEntry = (GtkBox*)gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  valueLine *newValue = (valueLine*)malloc(sizeof(valueLine));
  itemLine *newItem = (itemLine*)malloc(sizeof(itemLine));
  gtk_box_set_homogeneous(newEntry, 1);

  if (
      !isStringInputValid   (stringItemName,      "item name") ||
      !isNumericalInputValid(stringItemCost,      "item cost") ||
      !isNumericalInputValid(stringItemValue,     "item value") ||
      !isNumericalInputValid(stringItemQuantity,  "item quantity")
     ) return;

  GtkLabel *cLabelName = labelFromString(stringItemName,     newEntry);
  GtkLabel *cLabelQuant = labelFromString(stringItemQuantity, newEntry);
  GtkLabel *cLabelCost = labelFromString(stringItemCost,     newEntry);
  GtkLabel *cLabelValue = labelFromString(stringItemValue,    newEntry);
  if (!bounded)gtk_widget_hide(GTK_WIDGET(cLabelQuant));

  newItem->name = cLabelName;
  newItem->quantity = cLabelQuant;
  newItem->cost = cLabelCost;
  newItem->value = cLabelValue;
  addElementToItems(newItem, itemList);

  newValue->name = (char*)malloc(sizeof(char)*strlen(stringItemName));
  strcpy(newValue->name, stringItemName);
  newValue->cost = stringToLong(stringItemCost);
  newValue->value = stringToLong(stringItemValue);
  newValue->quantity = stringToLong(stringItemQuantity);
  addElementToValues(newValue, valueList);

  gtk_entry_set_text(itemName, "");
  gtk_entry_set_text(itemCost, "");
  gtk_entry_set_text(itemValue, "");
  gtk_entry_set_text(itemQuantity, quantityDefault);

  gtk_container_add((GtkContainer*)gtkBoxItemList, (GtkWidget*)newEntry);
  gtk_widget_show((GtkWidget*)newEntry);
}

void addKnapsackItem () {
  const gchar* stringItemName = gtk_entry_get_text(itemName);
  const gchar* stringItemCost = gtk_entry_get_text(itemCost);
  const gchar* stringItemValue = gtk_entry_get_text(itemValue);
  const gchar* stringItemQuantity = gtk_entry_get_text(itemQuantity);

  knapsackLineFromStrings(stringItemName, stringItemCost, stringItemValue, stringItemQuantity);
}

void calculate () {
  const gchar* stringKnapsackCapacity = gtk_entry_get_text(knapsackCapacity); 
  if (
      !isStringInputValid (stringKnapsackCapacity, "knapsackCapacity")
     ) return; 
  long knapsackCapacityValue = strtol(stringKnapsackCapacity, NULL, 10);

  int i = 0;
  item_t* knapsackItems = (item_t*)malloc(sizeof(item_t) * valueList->count);
  valueLine *curr;
  curr = valueList->first;

  while (curr != NULL) {
    knapsackItems[i].name   = curr->name;
    knapsackItems[i].weight = curr->cost;
    knapsackItems[i].value  = curr->value;
    knapsackItems[i].count  = curr->quantity;
    curr = curr->next;
    i++;
  }
  knapsackResult* result = knapsack(knapsackCapacityValue, knapsackItems, valueList->count); 
  showResultWindow(result);
}

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
    gtk_entry_set_text(knapsackCapacity, line);
  }

  while ((read = getline(&line, &len, fp)) != -1) {
    char* itemName = strtok(line, ",");
    if (itemName == NULL) return;
    char* itemCost = strtok(NULL, ",");
    if (itemCost == NULL) return;
    char* itemValue = strtok(NULL, ",");
    if (itemValue == NULL) return;
    char* itemQuantity = strtok(NULL, "\n");
    if (itemQuantity == NULL) return;

    knapsackLineFromStrings(itemName, itemCost, itemValue, itemQuantity);
  }

  fclose(fp);
  if (line)
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


void openSaveDialog(){
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

void saveToFile (char* filename) {
  FILE *fp;
  fp = fopen(filename, "w+");
  const gchar* stringKnapsackCapacity = gtk_entry_get_text(knapsackCapacity); 
  if (
      !isStringInputValid (stringKnapsackCapacity, "knapsackCapacity")
     ) return; 
  long knapsackCapacityValue = strtol(stringKnapsackCapacity, NULL, 10);

  valueLine* curr;
  curr = valueList->first;

  fprintf(fp, "%ld\n", knapsackCapacityValue);
  while (curr != NULL) {
    fprintf(fp, "%s, %ld, %ld, %ld\n", curr->name, curr->cost, curr->value, curr->quantity);
    curr = curr->next;
  }

  fclose(fp);
}

void closeGtkApp() {
  gtk_main_quit();
}

void closeGtkWindow(GtkWindow* window) {
  gtk_window_close(window);
}

void setFormulaRepresentation(GtkLabel *mathRepLabel, knapsackResult *result) {
  int length = 10000;
  char math[length];
  char currOpperand[30];
  strcpy(math, "<span font_desc=\"Serif Italic 14\">Z = ");
  valueLine* currLine = valueList->first;
  for (int i = 0; i < valueList->count; i ++) {
    snprintf(currOpperand, 30, "x<sub>%i</sub>*%ld", i, currLine->value);
    if (i != valueList->count -1) strcat(currOpperand, " + ");
    strcat(math, currOpperand);
    currLine = currLine->next;
  }
  strcat(math, "\nSujeto a:\n");
  currLine = valueList->first;
  for (int i = 0; i < valueList->count; i ++) {
    snprintf(currOpperand, 30, "x<sub>%i</sub>*%ld", i, currLine->cost);
    if (i != valueList->count -1) strcat(currOpperand, " + ");
    strcat(math, currOpperand);
    currLine = currLine->next;
  }
  snprintf(currOpperand, 30, "≤ %i; \n", result->knapsackCapacity);
  strcat(math, currOpperand);
  currLine = valueList->first;
  for (int i = 0; i < valueList->count; i ++) {
    snprintf(currOpperand, 30, "x<sub>%i</sub> ≤ %ld\n", i, currLine->quantity);
    strcat(math, currOpperand);
    currLine = currLine->next;
  }
  strcat(math, "</span>");

  gtk_label_set_markup(mathRepLabel, math);
}
