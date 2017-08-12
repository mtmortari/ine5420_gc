#include <gtk/gtk.h>
#include <list>
#include <vector>
#include "Point2D.h"
#include "DrawableObject.h"
#include "ObjectType.cpp"


static cairo_surface_t *surface = NULL;

//list of objects that are gonna be drawn
std::list<DrawableObject> display_file;

GtkBuilder *builder;
GtkWidget *window;
GtkWidget *drawing_area;
GtkWidget *new_object_dialog;



/* removes all objects from the surface and clears the display_file*/
static void clear_surface (){
  cairo_t *cr;
  cr = cairo_create (surface);
  cairo_set_source_rgb (cr, 1, 1, 1);
  cairo_paint (cr);
  cairo_destroy (cr);

  // clears the list
  display_file.clear();
}

/* Function that will be called when the button new object is called, to it can open a dialog to add new objects to
  the display file
*/
G_MODULE_EXPORT void add_new_object_dialog()
{
  gtk_window_present( GTK_WINDOW( new_object_dialog ) );
} 



int main (int   argc, char *argv[])
{
  gtk_init (&argc, &argv);

  /* Construct a GtkBuilder instance and load our UI description */
  builder = gtk_builder_new ();
  gtk_builder_add_from_file (builder, "window.glade", NULL);

  /* Connect signal handlers to the constructed widgets. */
  window = GTK_WIDGET( gtk_builder_get_object( builder, "window" ) );
  //drawing_area = GTK_WIDGET( gtk_builder_get_object( builder, "drawing_area" ) );
  new_object_dialog = GTK_WIDGET( gtk_builder_get_object( builder, "new_object_dialog" ) );


  g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);
  g_signal_connect (window, "configure-event", G_CALLBACK (add_new_object_dialog), NULL);
  //g_signal_connect (drawing_area, "draw", G_CALLBACK (redraw), NULL);

  gtk_main ();

  //test
	Point2D point;
	point.setX(5.0);
	point.setY(7.0);

  ObjectType type1;
  type1 = ObjectType::Point;

  DrawableObject obj1;
  obj1.setName("teste");
  obj1.setType(type1);

  return 0;
}