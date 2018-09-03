#include <gtk/gtk.h>
#include <time.h>
#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <stdbool.h>
#include "knapsack.h"

GtkEntry    *knapsackCapacity;

GtkEntry    *itemName;
GtkEntry    *itemQuantity;
GtkEntry    *itemCost;
GtkEntry    *itemValue;

GtkBox      *gtkBoxItemList;

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

int main(int argc, char *argv[]) {
  GtkBuilder      *builder = 0; 
  GtkWidget       *window = 0;

  itemList = (itemLinkedList*)malloc(sizeof(itemLinkedList));
  valueList = (valueLinkedList*)malloc(sizeof(valueLinkedList));

  gtk_init(&argc, &argv);

  builder = gtk_builder_new();
  gtk_builder_add_from_file (builder, "glade/main_window.glade", NULL);

  window = GTK_WIDGET(gtk_builder_get_object(builder, "Main_Window"));

  knapsackCapacity = (GtkEntry*)GTK_WIDGET(gtk_builder_get_object(builder, "Entry_Knapsack_Capacity"));

  itemName = (GtkEntry*)GTK_WIDGET(gtk_builder_get_object(builder, "Entry_Item_Name"));
  itemQuantity = (GtkEntry*)GTK_WIDGET(gtk_builder_get_object(builder, "Entry_Item_Quantity"));
  itemCost = (GtkEntry*)GTK_WIDGET(gtk_builder_get_object(builder, "Entry_Item_Cost"));
  itemValue = (GtkEntry*)GTK_WIDGET(gtk_builder_get_object(builder, "Entry_Item_Value"));
  
  gtkBoxItemList = (GtkBox*)GTK_WIDGET(gtk_builder_get_object(builder, "Box_Item_List"));

  gtk_builder_connect_signals(builder, NULL);

  g_object_unref(builder);

  gtk_widget_show(window);
  gtk_main();

  return 0;
}

// recibe el valor del input a validar y en nombre del input para imprimir errores.
bool isNumericalInputValid(const gchar* inputValue, char* inputName) {
  int value;
  if (strcmp(inputValue, "") == 0) { // If string is null
    fprintf(stderr, "La entrada \"%s\" no puede ser vacia.\n", inputName);
    return false;
  }
  value = strtol(inputValue, NULL, 10);
  if (value == 0 && strcmp(inputValue, "0")) {
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

void addKnapsackItem () {
  GtkBox* newEntry = (GtkBox*)gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  valueLine *newValue = (valueLine*)malloc(sizeof(valueLine));
  gtk_box_set_homogeneous(newEntry, 1);

  const gchar* stringItemName = gtk_entry_get_text(itemName);
  const gchar* stringItemCost = gtk_entry_get_text(itemCost);
  const gchar* stringItemValue = gtk_entry_get_text(itemValue);
  const gchar* stringItemQuantity = gtk_entry_get_text(itemQuantity);

  if (
      !isStringInputValid   (stringItemName,      "item name") ||
      !isNumericalInputValid(stringItemCost,      "item cost") ||
      !isNumericalInputValid(stringItemValue,     "item value") ||
      !isNumericalInputValid(stringItemQuantity,  "item quantity")
     ) return;

  labelFromString(stringItemName,     newEntry);
  labelFromString(stringItemQuantity, newEntry);
  labelFromString(stringItemCost,     newEntry);
  labelFromString(stringItemValue,    newEntry);

  strcpy(newValue->name, stringItemName);
  newValue->cost = strtol(stringItemCost, NULL, 10);
  newValue->value = strtol(stringItemValue, NULL, 10);
  newValue->quantity = strtol(stringItemQuantity, NULL, 10);
  addElementToValues(newValue, valueList);

  gtk_entry_set_text(itemName, "");
  gtk_entry_set_text(itemCost, "");
  gtk_entry_set_text(itemValue, "");
  gtk_entry_set_text(itemQuantity, "");

  gtk_container_add((GtkContainer*)gtkBoxItemList, (GtkWidget*)newEntry);
  gtk_widget_show((GtkWidget*)newEntry);
}

void calculate () {
  valueLine *curr;
  curr = valueList->first;
  while (curr != NULL) {
    printf("{%s, %ld, %ld, %ld}\n", curr->name, curr->quantity, curr->cost, curr->value);
    curr = curr->next;
  }
}

void loadFromFile() {
  // TODO
}

void closeGtkApp() {
  gtk_main_quit();
}
