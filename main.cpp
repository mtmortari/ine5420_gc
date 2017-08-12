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


/*Creates the surface*/
static gboolean create_surface (GtkWidget *widget, GdkEventConfigure *event, gpointer data){
  if (surface)
  {    
    cairo_surface_destroy (surface);
  }

  surface = gdk_window_create_similar_surface (gtk_widget_get_window (widget),
                                       CAIRO_CONTENT_COLOR,
                                       gtk_widget_get_allocated_width (widget),
                                       gtk_widget_get_allocated_height (widget));
  clear_surface ();
  return TRUE;
}


/* Redraw the screen from the surface */
static gboolean redraw (GtkWidget *widget, cairo_t   *cr,  gpointer   data){
  cairo_set_source_surface (cr, surface, 0, 0);
  cairo_paint (cr);

  return FALSE;
}

/* Function that will be called when the button new object is called, to it can open a dialog to add new objects to
  the display file
*/
G_MODULE_EXPORT void add_new_object_dialog()
{
  gtk_window_present( GTK_WINDOW( new_object_dialog ) );
} 

//adds a point to the surface
void addPoint(double x, double y, std::string name)
{
  Point2D point;
  point.setX(x);
  point.setY(y);  

  std::list<Point2D> pointList;  
  pointList.push_front(point);

  DrawableObject obj;
  obj.setName(name);
  obj.setType(ObjectType::POINT);
  obj.setPoints(pointList);

  display_file.push_back(obj);
}


//adds a line to the surface
void addLine(double x1, double y1, double x2, double y2, std::string name)
{
  Point2D point1;
  point1.setX(x1);
  point1.setY(y1);  

  Point2D point2;
  point2.setX(x2);
  point2.setY(y2);  

  std::list<Point2D> pointList;  
  pointList.push_back(point1);
  pointList.push_back(point2);

  DrawableObject obj;
  obj.setName(name);
  obj.setType(ObjectType::LINE);
  obj.setPoints(pointList);

  display_file.push_back(obj);
}

void drawNewObject()
{
   DrawableObject obj = display_file.back();

  cairo_t *cr;
  cr = cairo_create (surface);

  switch (obj.geType())
  {
    case POINT:
    {
      Point2D point = obj.getPoints().back();
      cairo_move_to(cr, point.getX(), point.getY());
      cairo_line_to(cr, point.getX(), point.getY());
      cairo_stroke(cr);
      gtk_widget_queue_draw (window);
      break;
    }
    case LINE:
    {
      Point2D origin = obj.getPoints().front();
      Point2D end = obj.getPoints().back();
      cairo_move_to(cr, origin.getX(), origin.getY());
      cairo_line_to(cr, end.getX(), end.getY());
      cairo_stroke(cr);
      gtk_widget_queue_draw (window);
      break;
    }
    case POLYGON:
    {
      Point2D origin = obj.getPoints().front();
      cairo_move_to(cr, origin.getX(), origin.getY());

      for (std::list<Point2D>::iterator it=obj.getPoints().begin(); it != obj.getPoints().end(); ++it)
      {          
          cairo_line_to(cr, it->getX(), it->getY());
      }    
      cairo_stroke(cr);
      gtk_widget_queue_draw (window);
      break;
    }
    default:
      break;

  }

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
  g_signal_connect (drawing_area, "draw", G_CALLBACK (redraw), NULL);

  gtk_main ();

  //test
	Point2D point;
	point.setX(5.0);
	point.setY(7.0);

  ObjectType type1;
  type1 = ObjectType::POINT;

  DrawableObject obj1;
  obj1.setName("teste");
  obj1.setType(type1);

  return 0;
}