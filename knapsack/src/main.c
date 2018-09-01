#include <gtk/gtk.h>
#include <time.h>
#include <stdio.h>
#include <math.h>

GtkEntry    *knapsackCapacity;

GtkEntry    *itemName;
GtkEntry    *itemQuantity;
GtkEntry    *itemCost;
GtkEntry    *itemValue;

GtkBox      *gtkBoxItemList;

struct itemLine {
  GtkLabel* name;
  GtkLabel* quantity;
  GtkLabel* cost;
  GtkLabel* value;
} GtkEntryLine;

int main(int argc, char *argv[]) {
  GtkBuilder      *builder = 0; 
  GtkWidget       *window = 0;

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

GtkLabel* lineLableFromImput (GtkEntry* input, GtkBox* line) {
  const gchar* value  = gtk_entry_get_text(input);
  GtkLabel* label     = (GtkLabel*)gtk_label_new(value);
  gtk_widget_set_hexpand((GtkWidget*)label, 1);
  gtk_container_add((GtkContainer*)line, (GtkWidget*)label);
  gtk_widget_show((GtkWidget*)label);
  gtk_entry_set_text(input, "");
  return label;
}

/*
 * TODO:
 * meter lineLableFromImput en un struct para eliminarlos despues
 */

void addKnapsackItem () {
  GtkBox* newEntry = (GtkBox*)gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  gtk_box_set_homogeneous(newEntry, 1);

  lineLableFromImput(itemName, newEntry);
  lineLableFromImput(itemCost, newEntry);
  lineLableFromImput(itemValue, newEntry);
  lineLableFromImput(itemQuantity, newEntry);

  gtk_container_add((GtkContainer*)gtkBoxItemList, (GtkWidget*)newEntry);
  gtk_widget_show((GtkWidget*)newEntry);
}

void calculate () {
  //TODO
}

void loadFromFile() {
  // TODO
}

void closeGtkApp() {
  gtk_main_quit();
}
