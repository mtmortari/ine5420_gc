#include <gtk/gtk.h>
#include <list>
#include <vector>
#include "Point2D.h"
#include "DrawableObject.h"
#include "ObjectType.cpp"
#include "View.h"

static cairo_surface_t *surface = NULL;

//list of objects that are gonna be drawn
std::list<DrawableObject> display_file;


GtkWidget *window;
GtkWidget *drawing_area;

//point
GtkEntry *point_x_input;
GtkEntry *point_y_input;
GtkEntry *point_name_input;

//line
GtkEntry *line_name_input;
GtkEntry *line_start_x_input;
GtkEntry *line_start_y_input;;
GtkEntry *line_end_x_input;
GtkEntry *line_end_y_input;


View main_window;
View viewport;


//method declaratrions
static void clear_surface();
static gboolean create_surface (GtkWidget *widget, GdkEventConfigure *event, gpointer data);
static gboolean redraw (GtkWidget *widget, cairo_t *cr, gpointer data);
extern "C" G_MODULE_EXPORT void add_new_object_dialog();
extern "C" G_MODULE_EXPORT void button_add_line_clicked();
extern "C" G_MODULE_EXPORT void add_point_button_signal();


void drawNewObject(DrawableObject obj);
void addPoint(double x, double y, std::string name);
void addLine(double x1, double y1, double x2, double y2, std::string name);
void draw_y_axis();
void draw_x_axis();
void draw_axis();



//method implementations

/* removes all objects from the surface and clears the display_file*/
static void clear_surface (){
  cairo_t *cr;
  cr = cairo_create (surface);
  cairo_set_source_rgb (cr, 1, 1, 1);
  cairo_paint (cr);
  cairo_destroy (cr);  
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
static gboolean redraw (GtkWidget *widget, cairo_t *cr, gpointer data){
  cairo_set_source_surface (cr, surface, 0, 0);
  cairo_paint (cr);

  return FALSE;
}

void drawNewObject(DrawableObject obj)
{
  cairo_t *cr;
  cr = cairo_create (surface);

  switch (obj.geType())
  {
    case POINT:
    {
      Point2D point = obj.getPoints().back();
      cairo_move_to(cr, viewport.transformX(point.getX(), main_window), viewport.transformY(point.getY(), main_window));
      cairo_line_to(cr, viewport.transformX(point.getX(), main_window), viewport.transformY(point.getY(), main_window));
      cairo_stroke(cr);
      gtk_widget_queue_draw (window);
      break;
    }
    case LINE:
    {
      Point2D origin = obj.getPoints().front();
      Point2D end = obj.getPoints().back();
      cairo_move_to(cr, viewport.transformX(origin.getX(), main_window), viewport.transformY(origin.getY(), main_window));
      cairo_line_to(cr, viewport.transformX(end.getX(), main_window), viewport.transformY(end.getY(), main_window));
      cairo_stroke(cr);
      gtk_widget_queue_draw (window);
      break;
    }
    case POLYGON:
    {
      Point2D origin = obj.getPoints().front();
      Point2D end = obj.getPoints().back();
      cairo_move_to(cr, viewport.transformX(origin.getX(), main_window), viewport.transformX(origin.getY(), main_window));

      for (std::list<Point2D>::iterator it=obj.getPoints().begin(); it != obj.getPoints().end(); ++it)
      {          
        cairo_line_to(cr, viewport.transformX(it->getX(), main_window), viewport.transformY(it->getY(), main_window));
      }    
      cairo_line_to(cr, viewport.transformX(end.getX(), main_window), viewport.transformY(end.getY(), main_window));


      cairo_stroke(cr);
      gtk_widget_queue_draw (window);
      break;
    }
    default:
      break;

  }

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
  drawNewObject(display_file.back());
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
  drawNewObject(display_file.back());
}

void draw_y_axis(){
  cairo_t *cr;
  cr = cairo_create (surface);
  cairo_move_to(cr, viewport.transformX(0, main_window), viewport.transformY(1000, main_window));
  cairo_line_to(cr, viewport.transformX(0, main_window), viewport.transformY(-1000, main_window));
  cairo_stroke(cr);
  gtk_widget_queue_draw (window);
}

void draw_x_axis(){
  cairo_t *cr;
  cr = cairo_create (surface);
  cairo_move_to(cr, viewport.transformX(1000, main_window), viewport.transformY(0, main_window));
  cairo_line_to(cr, viewport.transformX(-1000, main_window), viewport.transformY(0, main_window));
  cairo_stroke(cr);
  gtk_widget_queue_draw (window);
}

void draw_axis(){
  draw_x_axis();
  draw_y_axis();
}

/* Function that will be called when the button new object is called, to it can open a dialog to add new objects to
  the display file
*/
extern "C" G_MODULE_EXPORT void add_new_object_dialog()
{ 
  addLine(0.0, 0.0, 200.0, 400.0, "TESTE");
  addLine(300.0, -200.0, 500.0, -150.0, "BLA");

} 


/* Button to add a new point
*/
extern "C" G_MODULE_EXPORT void add_point_button_signal()
{ 	
	const gchar *x_text = gtk_entry_get_text( point_x_input );
	const gchar *y_text = gtk_entry_get_text( point_y_input );
	double x = atof (x_text);
	double y = atof(y_text);

	addPoint(x,y, "TESTE PONTO"); 

} 


/* Button to add a new line
*/
extern "C" G_MODULE_EXPORT void button_add_line_clicked()
{ 	
	const gchar *x_start_text = gtk_entry_get_text(  line_start_x_input );
	const gchar *y_start_text = gtk_entry_get_text( line_start_y_input );
	const gchar *x_end_text = gtk_entry_get_text( line_end_x_input );
	const gchar *y_end_text = gtk_entry_get_text( line_end_y_input );
	const gchar *name = gtk_entry_get_text( line_name_input );


	double x_start = atof (x_start_text);
	double y_start = atof(y_start_text);
	double x_end = atof (x_end_text);
	double y_end = atof(y_end_text);

   g_print ("name %s\n", name);

	addLine(x_start, y_start, x_end, y_end, name); 

} 


//MAIN 
int main (int   argc, char *argv[])
{
  GtkBuilder *builder;
  gtk_init (&argc, &argv);

  /* Construct a GtkBuilder instance and load our UI description */
  builder = gtk_builder_new();
  gtk_builder_add_from_file (builder, "window.glade", NULL);

  /* Connect signal handlers to the constructed widgets. */
  window = GTK_WIDGET( gtk_builder_get_object( builder, "main_window" ) );
  drawing_area = GTK_WIDGET( gtk_builder_get_object( builder, "drawing_area" ) );

  //point init
  point_x_input = GTK_ENTRY( gtk_builder_get_object( builder, "point_x_input" ) );
  point_y_input = GTK_ENTRY( gtk_builder_get_object( builder, "point_y_input" ) );
  point_name_input = GTK_ENTRY( gtk_builder_get_object( builder, "point_name_input" ) );

  //line init
  line_start_x_input = GTK_ENTRY( gtk_builder_get_object( builder, "line_start_x_input" ) );
  line_start_y_input = GTK_ENTRY( gtk_builder_get_object( builder, "line_start_y_input" ) );
  line_end_x_input = GTK_ENTRY( gtk_builder_get_object( builder, "line_end_x_input" ) );
  line_end_y_input = GTK_ENTRY( gtk_builder_get_object( builder, "line_end_y_input" ) );
  line_name_input = GTK_ENTRY( gtk_builder_get_object( builder, "line_name_input" ) );

  g_signal_connect (drawing_area, "draw", G_CALLBACK (redraw), NULL);
  g_signal_connect (drawing_area, "configure-event", G_CALLBACK (create_surface), NULL);





  gtk_builder_connect_signals(builder, NULL);
  gtk_widget_show_all(window);
  
  
  main_window.setXMin(-1000.0);
  main_window.setYMin(-1000.0);
  main_window.setXMax(1000.0);
  main_window.setYMax(1000.0);


  viewport.setXMin(-250.0);
  viewport.setYMin(-250.0);
  viewport.setXMax(250.0);
  viewport.setYMax(250.0);

  gtk_main();  

  //draw_axis();
  return 0;
}