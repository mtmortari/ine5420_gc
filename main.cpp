#include <gtk/gtk.h>
#include <list>
#include <vector>
#include "Point2D.h"
#include "DrawableObject.h"
#include "ObjectType.cpp"
#include "View.h"
#include "Matrix2D.h"


#define WINDOW_SIZE 200.0
#define VIEWPORT_SIZE 250.0
#define NAVIGATE_SCALE 50.0
#define ZOOM_FACTOR 1.2


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

//polygon
GtkEntry *polygon_name_input;
GtkEntry *polygon_x_input;
GtkEntry *polygon_y_input;
std::list<Point2D> polygon_point_list;



//listbox
GtkListBox *list_box;


View main_window;
View viewport;


//method declaratrions
static void clear_surface();
static gboolean create_surface (GtkWidget *widget, GdkEventConfigure *event, gpointer data);
static gboolean redraw (GtkWidget *widget, cairo_t *cr, gpointer data);

//signals from GUI
//add objects
extern "C" G_MODULE_EXPORT void button_add_line_clicked();
extern "C" G_MODULE_EXPORT void button_add_point_clicked();
extern "C" G_MODULE_EXPORT void button_add_point_to_polygon_clicked();
extern "C" G_MODULE_EXPORT void button_add_polygon_clicked();

//navigate and zoom
extern "C" G_MODULE_EXPORT void button_navigate_left_clicked();
extern "C" G_MODULE_EXPORT void button_navigate_right_clicked();
extern "C" G_MODULE_EXPORT void button_navigate_up_clicked();
extern "C" G_MODULE_EXPORT void button_navigate_down_clicked();

extern "C" G_MODULE_EXPORT void button_zoom_plus_clicked();
extern "C" G_MODULE_EXPORT void button_zoom_minus_clicked();

void drawNewObject(DrawableObject obj);
void clearAndRedraw();
void addPoint(double x, double y, std::string name);
void addLine(double x1, double y1, double x2, double y2, std::string name);
void addPolygon(std::list<Point2D> pointList, std::string name);
void draw_axis();
double getDoubleFromGtkEntry(GtkEntry *entry);
void clearGtkEntry(GtkEntry *entry);

void clearPointInput();
void clearLineInput();
void clearPolygonPointInput();
void clearPolygonInput();



// --------- method implementations ---------------

/* removes all objects from the surface and clears the display_file*/
static void clear_surface (){
  cairo_t *cr;
  cr = cairo_create (surface);
  cairo_set_source_rgb (cr, 1, 1, 1);
  cairo_set_line_width(cr, 1.0);
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



double getDoubleFromGtkEntry(GtkEntry *entry)
{ const gchar *text = gtk_entry_get_text(entry );  
  return atof(text);
}

void clearGtkEntry(GtkEntry *entry)
{
  gtk_entry_set_text (entry,"");
}


//draws a new object in the viewport
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
      std::list<Point2D> points = obj.getPoints();

      Point2D origin = points.front();
      cairo_move_to(cr, viewport.transformX(origin.getX(), main_window), viewport.transformX(origin.getY(), main_window));

      std::list<Point2D>::iterator it;
      for (it= points.begin(); it != points.end(); ++it)
      {                  
        cairo_line_to(cr, viewport.transformX(it->getX(), main_window), viewport.transformY(it->getY(), main_window));
      }    
      cairo_line_to(cr, viewport.transformX(origin.getX(), main_window), viewport.transformY(origin.getY(), main_window));
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
  drawNewObject(obj);
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
  drawNewObject(obj);
}

//adds a polygon to the surface
void addPolygon(std::list<Point2D> pointList, std::string name )
{
  DrawableObject obj;
  obj.setName(name);  
  obj.setType(ObjectType::POLYGON);
  obj.setPoints(pointList);

  display_file.push_back(obj);
  drawNewObject(obj);
}

void draw_axis(){ 
   addLine(0, -WINDOW_SIZE, 0, WINDOW_SIZE, "Y_AXIS"); 
   addLine(-WINDOW_SIZE,0, WINDOW_SIZE, 0, "X_AXIS"); 
}

void clearPointInput()
{
  clearGtkEntry(point_name_input);
  clearGtkEntry(point_x_input);
  clearGtkEntry(point_y_input);
}


void clearLineInput()
{
  clearGtkEntry(line_name_input);
  clearGtkEntry(line_start_x_input);
  clearGtkEntry(line_start_y_input);
  clearGtkEntry(line_end_x_input);
  clearGtkEntry(line_end_y_input);
}


void clearPolygonPointInput()
{
  clearGtkEntry(polygon_x_input);
  clearGtkEntry(polygon_y_input);
}

void clearPolygonInput()
{
  clearGtkEntry(polygon_name_input);
  clearGtkEntry(polygon_x_input);
  clearGtkEntry(polygon_y_input);
}

void clearAndRedraw()
{
  clear_surface();
  
  std::list<DrawableObject>::iterator it;
  for (it= display_file.begin(); it != display_file.end(); ++it)
  {                  
    drawNewObject(*it);
  }   
}


/* Button to add a new point
*/
extern "C" G_MODULE_EXPORT void button_add_point_clicked()
{ 	
  const gchar *name = gtk_entry_get_text( point_name_input );
  double x = getDoubleFromGtkEntry(point_x_input);
  double y = getDoubleFromGtkEntry(point_y_input);

	addPoint(x, y, name);
  clearPointInput();
} 


/* Button to add a new line
*/
extern "C" G_MODULE_EXPORT void button_add_line_clicked()
{ 	
	const gchar *name = gtk_entry_get_text( line_name_input );
  double x_start = getDoubleFromGtkEntry(line_start_x_input);
  double y_start = getDoubleFromGtkEntry(line_start_y_input);
  double x_end = getDoubleFromGtkEntry(line_end_x_input);
  double y_end = getDoubleFromGtkEntry(line_end_y_input);

  addLine(x_start, y_start, x_end, y_end, name); 
  clearLineInput();
} 


extern "C" G_MODULE_EXPORT void button_add_point_to_polygon_clicked()
{ 
  double x = getDoubleFromGtkEntry(polygon_x_input); 
  double y = getDoubleFromGtkEntry(polygon_y_input); 

  Point2D point;
  point.setX(x);
  point.setY(y);

  polygon_point_list.push_back(point);
  clearPolygonPointInput();
}

extern "C" G_MODULE_EXPORT void button_add_polygon_clicked()
{
  const gchar *name = gtk_entry_get_text( polygon_name_input );
  addPolygon(polygon_point_list, name);

  //removes all elements from list
  polygon_point_list.clear();
  clearPolygonInput();
}



extern "C" G_MODULE_EXPORT void button_navigate_left_clicked()
{
  main_window.setXMax(main_window.getXMax() - NAVIGATE_SCALE);
  main_window.setXMin(main_window.getXMin() - NAVIGATE_SCALE);
  clearAndRedraw();
}

extern "C" G_MODULE_EXPORT void button_navigate_right_clicked()
{
  main_window.setXMax(main_window.getXMax() + NAVIGATE_SCALE);
  main_window.setXMin(main_window.getXMin() + NAVIGATE_SCALE);
  clearAndRedraw();

}

extern "C" G_MODULE_EXPORT void button_navigate_up_clicked()
{
  main_window.setYMax(main_window.getYMax() + NAVIGATE_SCALE);
  main_window.setYMin(main_window.getYMin() + NAVIGATE_SCALE);
  clearAndRedraw();

}

extern "C" G_MODULE_EXPORT void button_navigate_down_clicked()
{
  main_window.setYMax(main_window.getYMax() - NAVIGATE_SCALE);
  main_window.setYMin(main_window.getYMin() - NAVIGATE_SCALE);
  clearAndRedraw();

}

extern "C" G_MODULE_EXPORT void button_zoom_plus_clicked()
{
  main_window.setXMax(main_window.getXMax() / ZOOM_FACTOR);
  main_window.setXMin(main_window.getXMin() / ZOOM_FACTOR);
  main_window.setYMax(main_window.getYMax() / ZOOM_FACTOR);
  main_window.setYMin(main_window.getYMin() / ZOOM_FACTOR);
  clearAndRedraw();

}

extern "C" G_MODULE_EXPORT void button_zoom_minus_clicked()
{
  main_window.setXMax(main_window.getXMax() * ZOOM_FACTOR);
  main_window.setXMin(main_window.getXMin() * ZOOM_FACTOR);
  main_window.setYMax(main_window.getYMax() * ZOOM_FACTOR);
  main_window.setYMin(main_window.getYMin() * ZOOM_FACTOR);
  clearAndRedraw();
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
  list_box = GTK_LIST_BOX( gtk_builder_get_object( builder, "display_file_list" ) );

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


  //line init
  polygon_name_input = GTK_ENTRY( gtk_builder_get_object( builder, "polygon_name_input" ) );
  polygon_x_input = GTK_ENTRY( gtk_builder_get_object( builder, "polygon_x_input" ) );
  polygon_y_input = GTK_ENTRY( gtk_builder_get_object( builder, "polygon_y_input" ) );


  g_signal_connect (drawing_area, "draw", G_CALLBACK (redraw), NULL);
  g_signal_connect (drawing_area, "configure-event", G_CALLBACK (create_surface), NULL);

  gtk_builder_connect_signals(builder, NULL);
  gtk_widget_show_all(window);
  
  
  main_window.setXMin(-WINDOW_SIZE);
  main_window.setYMin(-WINDOW_SIZE);
  main_window.setXMax(WINDOW_SIZE);
  main_window.setYMax(WINDOW_SIZE);


  viewport.setXMin(-VIEWPORT_SIZE);
  viewport.setYMin(-VIEWPORT_SIZE);
  viewport.setXMax(VIEWPORT_SIZE);
  viewport.setYMax(VIEWPORT_SIZE);

  gtk_main();  

  return 0;
}