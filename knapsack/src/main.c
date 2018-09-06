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
void generateMath();

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

void knapsackOneZero(){
  printf("1/0");
  GtkBuilder      *builder = 0; 
  GtkWidget       *window = 0;

  itemList = (itemLinkedList*)malloc(sizeof(itemLinkedList));
  valueList = (valueLinkedList*)malloc(sizeof(valueLinkedList));


  builder = gtk_builder_new();
  gtk_builder_add_from_file (builder, "glade/main_window.glade", NULL);

  window = GTK_WIDGET(gtk_builder_get_object(builder, "Input_Window"));

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

}

void knapsackBounded(){
  printf("bounded");
}

void knapsackUnbounded(){
  printf("Unbounded");
}

void showResultWindow(knapsackResult* result) {
  GtkBuilder      *builder = 0; 
  GtkWidget       *window = 0;
  GtkGrid         *grid = 0;

  builder = gtk_builder_new();
  gtk_builder_add_from_file (builder, "glade/result_window.glade", NULL);

  window = GTK_WIDGET(gtk_builder_get_object(builder, "result_window"));
  grid = (GtkGrid*)GTK_WIDGET(gtk_builder_get_object(builder, "result_table"));

  for (int i = 1; i <= result->n; i++) {
    gtk_grid_insert_column(grid, i-1);
    for (int j = 0; j <= result->knapsackCapacity; j++) {
      gtk_grid_insert_row(grid, j);
    }
  }

  for (int i = 1; i <= result->n; i++) {
    for (int j = 0; j <= result->knapsackCapacity; j++) {
      printf("%d ",result->tabla[i][j]);
      char buff[50];
      if (result->color[i][j])
        snprintf(buff, 50, "<span foreground=\"red\">%i</span>", result->tabla[i][j]);
      else
        snprintf(buff, 50, "<span foreground=\"green\">%i</span>", result->tabla[i][j]);
      GtkWidget *label = gtk_label_new(NULL);
      gtk_label_set_markup(GTK_LABEL(label), buff);
      gtk_grid_attach(grid, label, i-1, j, 1, 1);
      gtk_widget_show(label);
    }
    printf("\n ");
  }

  for (int i = 1; i <= result->n; i++) {
    for (int j = 0; j <= result->knapsackCapacity; j++) {
    }
  }

  gtk_builder_connect_signals(builder, NULL);

  g_object_unref(builder);

  gtk_widget_show(window);
  gtk_main();

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

  newValue->name = (char*)malloc(sizeof(char)*strlen(stringItemName));
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
  generateMath();

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
    printf("{%s, %ld, %ld, %ld}\n", curr->name, curr->quantity, curr->cost, curr->value);
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

void loadFromFile() {
  // TODO
}

void closeGtkApp() {
  gtk_main_quit();
}

void closeGtkWindow(GtkWindow* window) {
  gtk_window_close(window);
}

void generateMath() {
  int length = 45 + valueList->count * 20;
  char math[length];
  //x<sub>1</sub>
  //<span font_desc="Serif Italic 18">Z = </span>
  strcpy(math, "<span font_desc=\"Serif Italic 18\">Z = ");
  for (int i = 0; i < valueList->count; i ++) {
    valueLine* currLine = valueList->first;
    char currOpperand[20];
    snprintf(currOpperand, 20, "x<sub>%li</sub>", currLine->value);
    strcat(math, currOpperand);
  }
  printf("%s", math);
}
