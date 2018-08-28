#include <cairo.h>
#include <math.h>
#include <stdio.h>

int width = 1500;
int height = 1000;
float radius = 50;

struct Node {
  float x;
  float y;
};

struct Node nodeArray[4];

void drawNode(struct Node node, char name[5], cairo_t *cr) {
  cairo_set_line_width (cr, 6.0);
  cairo_arc (cr, node.x, node.y, radius, 0.0, 2*M_PI);
  cairo_move_to(cr, node.x, node.y);
  cairo_show_text(cr, name);
  cairo_stroke (cr);
}

void setNodes(int numberOfNodes, cairo_t *cr) {
  cairo_select_font_face(cr, "Purisa",
      CAIRO_FONT_SLANT_NORMAL,
      CAIRO_FONT_WEIGHT_BOLD);
  cairo_set_font_size(cr, 40);

  float delta = width/numberOfNodes/1.4;
  for(int i = 0; i<numberOfNodes; i++) {
    nodeArray[i].x = (delta/2)+(i*delta);
    nodeArray[i].y = height/2;
    char c[5] = {i+1+'0'};
    drawNode(nodeArray[i], c, cr);
  }
}

void edge(struct Node node1, struct Node node2, float weight, cairo_t *cr) {
  cairo_set_line_width (cr, 2.0);
  cairo_select_font_face(cr, "Purisa",
      CAIRO_FONT_SLANT_NORMAL,
      CAIRO_FONT_WEIGHT_BOLD);
  cairo_set_font_size(cr, 25);
  char c[50];
  sprintf(c, "%g", weight);

  if(node1.x < node2.x) {
    cairo_arc (cr, (node1.x+node2.x)/2, height/2, (node2.x-node1.x)/2, M_PI, 0);
    cairo_move_to(cr, ((node1.x+node2.x)/2)-radius, height/2-(node2.x-node1.x)/2+30);
    cairo_show_text(cr, c);
  } else {
    cairo_arc (cr, (node1.x+node2.x)/2, height/2, (node1.x-node2.x)/2, 0, M_PI);
    cairo_move_to(cr, ((node1.x+node2.x)/2)-radius, height/2+(node1.x-node2.x)/2-30);
    cairo_show_text(cr, c);
  }
  cairo_stroke (cr);
}

void draw (float matrix[4][4], int matrixSize, cairo_t *cr)
{
  cairo_surface_t *surface =
    cairo_image_surface_create (CAIRO_FORMAT_ARGB32, width, height);
  //cairo_t *cr =
  //  cairo_create (surface);

  cairo_set_source_rgb (cr, 255, 255, 255);
  cairo_paint (cr);

  cairo_set_source_rgb (cr, 0, 0, 0);

  int n = sizeof(nodeArray) / sizeof(nodeArray[0]);

  setNodes(n, cr);
  for(int i = 0; i<n; i++) {
    for (int j = 0; j<n; j++) {
      if(matrix[i][j] > 0 && matrix[i][j] != INFINITY) {
        edge(nodeArray[i], nodeArray[j], matrix[i][j], cr);
      } else if(matrix[i][j] < 0) {
        printf("Error, valor negativo en la matriz");
      }
    }
  }
  for(int i = 0; i<n; i++) {
    cairo_translate(cr, nodeArray[i].x, nodeArray[i].y);
    cairo_set_source_rgb(cr, 0.3, 0.4, 0.6);
    cairo_fill(cr);
  }


  cairo_surface_write_to_png (surface, "graph.png");
  cairo_surface_destroy (surface);
}

//int main() {
//  float matrix[4][4] = {{0, 5, 1, 2},
//                        {5, 0, 3, INFINITY},
//                        {1, 3, 0, 4},
//                        {2, INFINITY, 4, 0}};
//  draw(matrix, 4);
//
//  return 0;
//}
