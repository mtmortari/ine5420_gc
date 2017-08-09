#include <gtk/gtk.h>
#include <list>
#include "Point2D.h"
#include "DrawableObject.h"
#include "ObjectType.cpp"



std::list<DrawableObject> object_list;

GtkBuilder *builder;
GObject *window;
GObject *drawing_area;

int main (int   argc, char *argv[])
{
  gtk_init (&argc, &argv);

  /* Construct a GtkBuilder instance and load our UI description */
  builder = gtk_builder_new ();
  gtk_builder_add_from_file (builder, "window.glade", NULL);

  /* Connect signal handlers to the constructed widgets. */
  window = gtk_builder_get_object (builder, "window");
  g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);

  drawing_area = gtk_builder_get_object(builder, "drawing_area");

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