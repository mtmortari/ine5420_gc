#include <gtk/gtk.h>
#include <iostream>
#include <vector>
#include <list>
#include "DrawableObject.h"
#include "ObjectType.cpp"
#include "View.h"

#define WINDOW_SIZE 200.0
//#define ZOOM_FACTOR 1.2


static cairo_surface_t *surface = NULL;

//list of objects that are gonna be drawn
std::list<DrawableObject> display_file;


//main window
GtkWidget *drawing_area;
GtkWidget *window;
 //GtkWidget *item;

//second window
GtkBuilder *gtkBuilder;
GtkWidget *second_window;

//listbox
//GtkListStore *list_store;
GtkWidget *combo_box;
//tkTreeIter iter;


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
std::list<Point3D> polygon_point_list;

//actions
GtkEntry *passo_input_navegation;
GtkEntry *passo_input_zoom;

View main_window;
View viewport;

//method declaratrions
static void clear_surface ();
static gboolean create_surface (GtkWidget *widget, GdkEventConfigure *event, gpointer data);
static gboolean redraw (GtkWidget *widget, cairo_t *cr, gpointer data);
void gtk_combo_box_text_append_text (GtkComboBoxText *combo_box, const gchar *text);

//static gboolean currentTab (GtkNotebook *notebook, GtkNotebookTab arg1, gpointer data);

//signals from GUI
//add objects
extern "C" G_MODULE_EXPORT void button_add_point_clicked();
extern "C" G_MODULE_EXPORT void button_add_line_clicked();
extern "C" G_MODULE_EXPORT void button_add_point_to_polygon_clicked();
extern "C" G_MODULE_EXPORT void button_add_polygon_clicked();
extern "C" G_MODULE_EXPORT void button_cancel();

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
void addPolygon(std::list<Point3D> pointList, std::string name);
double getDoubleFromGtkEntry(GtkEntry *entry);
void clearGtkEntry(GtkEntry *entry);
//void AddListItem (GtkWidget *listbox, char *sText);



void clearPointInput();
void clearLineInput();
void clearPolygonPointInput();
void clearPolygonInput();


void on_window_main_destroy()
{
    gtk_main_quit();
}

/*Clear the surface, removing the scribbles*/
static void clear_surface (){
  cairo_t *cr;
  cr = cairo_create (surface);
  cairo_set_source_rgb (cr, 1, 1, 1);
  cairo_paint (cr);
  cairo_destroy (cr);
}

/*Creates the surface*/
static gboolean create_surface (GtkWidget *widget, GdkEventConfigure *event, gpointer data)
{
  if (surface)
    cairo_surface_destroy (surface);

  auto width = gtk_widget_get_allocated_width (widget);
  auto height = gtk_widget_get_allocated_height (widget);
  surface = gdk_window_create_similar_surface (gtk_widget_get_window (widget),
                                       CAIRO_CONTENT_COLOR,
                                       width,
                                       height);

  viewport.setXMin(0);
  viewport.setXMax(width);
  viewport.setYMin(0);
  viewport.setYMax(height);

  clear_surface ();
  return TRUE;
}

/* Redraw the screen from the surface */
static gboolean redraw (GtkWidget *widget, cairo_t *cr, gpointer data){

  for (auto file: display_file) 
      drawNewObject(file);

  cairo_set_source_surface (cr, surface, 0, 0);
  cairo_paint (cr);

  return FALSE;
}

double getDoubleFromGtkEntry(GtkEntry *entry)
{ 
  const gchar *text = gtk_entry_get_text(entry );  
  return atof(text);
}

void clearGtkEntry(GtkEntry *entry)
{
  gtk_entry_set_text (entry,"");
}

//draws a new object in the viewport asdasdas
void drawNewObject(DrawableObject obj)
{
  cairo_t *cr;
  cr = cairo_create (surface);

  switch (obj.geType())
  {
    case POINT:
    {
      Point3D point = obj.getPoints().back();
      cairo_move_to(cr, viewport.transformX(point.getX(), main_window), viewport.transformY(point.getY(), main_window));
      cairo_line_to(cr, viewport.transformX(point.getX(), main_window), viewport.transformY(point.getY(), main_window));
      cairo_stroke(cr);
      //gtk_list_store_insert_with_values(list_store, &iter, -1, cr);
      gtk_widget_queue_draw (window);
      break;
    }
    case LINE:
    {
      Point3D origin = obj.getPoints().front();
      Point3D end = obj.getPoints().back();
      cairo_move_to(cr, viewport.transformX(origin.getX(), main_window), viewport.transformY(origin.getY(), main_window));
      cairo_line_to(cr, viewport.transformX(end.getX(), main_window), viewport.transformY(end.getY(), main_window));
      cairo_stroke(cr);
      gtk_widget_queue_draw (window);
      break;
    }
    case POLYGON:
    {
      std::list<Point3D> points = obj.getPoints();

      Point3D origin = points.front();   

      std::list<Point3D>::iterator it;
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
  cairo_destroy(cr);
}

//adds a point to the surface
void addPoint(double x, double y, std::string name)
{
  Point3D point;
  point.setX(x);
  point.setY(y);  
  point.setZ(1.0);  

  std::list<Point3D> pointList;  
  pointList.push_front(point);

  DrawableObject obj;
  obj.setName(name);
  obj.setType(ObjectType::POINT);
  obj.setPoints(pointList);

  display_file.push_back(obj);
  drawNewObject(obj);
}

void addLine(double x1, double y1, double x2, double y2, std::string name)
{
  Point3D point1;
  point1.setX(x1);
  point1.setY(y1);    
  point1.setZ(1.0);  

  Point3D point2;
  point2.setX(x2);
  point2.setY(y2);    
  point2.setZ(1.0);  

  std::list<Point3D> pointList;  
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
void addPolygon(std::list<Point3D> pointList, std::string name )
{
  DrawableObject obj;
  obj.setName(name);  
  obj.setType(ObjectType::POLYGON);
  obj.setPoints(pointList);

  display_file.push_back(obj);
  drawNewObject(obj);
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


/*Function that will be called when the  button "Adicionar objeto" is pressed*/
 extern "C" G_MODULE_EXPORT void button_open_menu()
 {
  gtkBuilder = gtk_builder_new();
  gtk_builder_add_from_file(gtkBuilder, "second_window.glade", NULL);

  second_window = GTK_WIDGET( gtk_builder_get_object( gtkBuilder, "second_window") );

  //pega a referencia dos inputs
  point_x_input = GTK_ENTRY( gtk_builder_get_object( gtkBuilder, "entry_x_point" ) );
  point_y_input = GTK_ENTRY( gtk_builder_get_object( gtkBuilder, "entry_y_point" ) );
  point_name_input = GTK_ENTRY( gtk_builder_get_object( gtkBuilder, "entry_name" ) );

  //line init
  line_start_x_input = GTK_ENTRY( gtk_builder_get_object( gtkBuilder, "entry_x1_line" ) );
  line_start_y_input = GTK_ENTRY( gtk_builder_get_object( gtkBuilder, "entry_y1_line" ) );
  line_end_x_input = GTK_ENTRY( gtk_builder_get_object( gtkBuilder, "entry_x2_line" ) );
  line_end_y_input = GTK_ENTRY( gtk_builder_get_object( gtkBuilder, "entry_y2_line" ) );
  line_name_input = GTK_ENTRY( gtk_builder_get_object( gtkBuilder, "entry_name" ) );

  //polygon init
  polygon_name_input = GTK_ENTRY( gtk_builder_get_object(gtkBuilder, "entry_name" ) );
  polygon_x_input = GTK_ENTRY( gtk_builder_get_object( gtkBuilder, "entry_x_polygon" ) );
  polygon_y_input = GTK_ENTRY( gtk_builder_get_object( gtkBuilder, "entry_y_polygon" ) );

  
  //g_signal_connect (second_window, "focus-tab", G_CALLBACK (currentTab), NULL);

  gtk_builder_connect_signals(gtkBuilder, NULL);

  //mostra a segunda tela, o botão cancel deveria desaparecer a segunda tela, talvez alco como 
 // gtk_widget_hide(second_window); ??? não sei o nome do método, isto é um chute
  gtk_widget_show(second_window);
 } 

 /* Button to add a new point  na segunda tela*/ 
extern "C" G_MODULE_EXPORT void button_add_point_clicked()
{   
  const gchar *name = gtk_entry_get_text( point_name_input );
  double x = getDoubleFromGtkEntry(point_x_input);
  double y = getDoubleFromGtkEntry(point_y_input);

  g_print ("name %s\n", name);
  g_print ("x start %f\n", x);
  g_print ("y start %f\n", y);

  addPoint(x, y, name);
  clearPointInput();
  gtk_widget_destroy(GTK_WIDGET(second_window));
} 

 /* Button to add a new line  na segunda tela*/ 
extern "C" G_MODULE_EXPORT void button_add_line_clicked()
{   
  const gchar *name = gtk_entry_get_text(line_name_input);
  double x_start = getDoubleFromGtkEntry(line_start_x_input);
  double y_start = getDoubleFromGtkEntry(line_start_y_input);
  double x_end = getDoubleFromGtkEntry(line_end_x_input);
  double y_end = getDoubleFromGtkEntry(line_end_y_input);

  g_print ("name %s\n", name);
  g_print ("x start %f\n", x_start);
  g_print ("y start %f\n", y_start);
  g_print ("x end %f\n", x_end);
  g_print ("y end %f\n", y_end);

  addLine(x_start, y_start, x_end, y_end, name); 
  clearLineInput();
  gtk_widget_destroy(GTK_WIDGET(second_window));
} 

extern "C" G_MODULE_EXPORT void button_add_point_to_polygon_clicked()
{ 
  double x = getDoubleFromGtkEntry(polygon_x_input); 
  double y = getDoubleFromGtkEntry(polygon_y_input); 

  Point3D point;
  point.setX(x);
  point.setY(y);
  point.setZ(1.0);  

  g_print ("x start %f\n", x);
  g_print ("y start %f\n", y);

  polygon_point_list.push_back(point);
  clearPolygonPointInput();
}

 /* Button to add a new polygon  na segunda tela*/ 
extern "C" G_MODULE_EXPORT void button_add_polygon_clicked()
{
  const gchar *name = gtk_entry_get_text( polygon_name_input );
  addPolygon(polygon_point_list, name);

  //removes all elements from list
  polygon_point_list.clear();
  clearPolygonInput();
  gtk_widget_destroy(GTK_WIDGET(second_window));
}

/* Button to left navigate the window */ 
extern "C" G_MODULE_EXPORT void button_navigate_left_clicked()
{
  double navigate = getDoubleFromGtkEntry(passo_input_navegation); 
  main_window.setXMax(main_window.getXMax() - navigate);
  main_window.setXMin(main_window.getXMin() - navigate);
  clearAndRedraw();
}
/* Button to right navigate the window */ 
extern "C" G_MODULE_EXPORT void button_navigate_right_clicked()
{
  double navigate = getDoubleFromGtkEntry(passo_input_navegation); 
  main_window.setXMax(main_window.getXMax() + navigate);
  main_window.setXMin(main_window.getXMin() + navigate);
  clearAndRedraw();

}
/* Button to up navigate the window */ 
extern "C" G_MODULE_EXPORT void button_navigate_up_clicked()
{
  double navigate = getDoubleFromGtkEntry(passo_input_navegation); 
  main_window.setYMax(main_window.getYMax() + navigate);
  main_window.setYMin(main_window.getYMin() + navigate);
  clearAndRedraw();

}

/* Button to down navigate the window */ 
extern "C" G_MODULE_EXPORT void button_navigate_down_clicked()
{
  double navigate = getDoubleFromGtkEntry(passo_input_navegation); 
  main_window.setYMax(main_window.getYMax() - navigate);
  main_window.setYMin(main_window.getYMin() - navigate);
  clearAndRedraw();


}

/* Button to zoom in the window */ 
extern "C" G_MODULE_EXPORT void button_zoom_plus_clicked()
{
  double zoom = getDoubleFromGtkEntry(passo_input_zoom); 
  main_window.setXMax(main_window.getXMax() / zoom);
  main_window.setXMin(main_window.getXMin() / zoom);
  main_window.setYMax(main_window.getYMax() / zoom);
  main_window.setYMin(main_window.getYMin() / zoom);
  clearAndRedraw();

}

/* Button to zoom out the window */ 
extern "C" G_MODULE_EXPORT void button_zoom_minus_clicked()
{
  double zoom = getDoubleFromGtkEntry(passo_input_zoom); 
  main_window.setXMax(main_window.getXMax() * zoom);
  main_window.setXMin(main_window.getXMin() * zoom);
  main_window.setYMax(main_window.getYMax() * zoom);
  main_window.setYMin(main_window.getYMin() * zoom);
  clearAndRedraw();
}


 /* Button to cancel*/ 
extern "C" G_MODULE_EXPORT void button_cancel()
{
  gtk_widget_destroy(GTK_WIDGET(second_window));
}

/*void listbox_changed (GtkWidget *widget, gpointer *data)
{
    //GtkList *listbox;

    g_print ("%s\n", data);
    DisplaySelectedItems (widget);
}*/


int main(int argc, char *argv[])
{
  main_window.setXMin(-WINDOW_SIZE);
  main_window.setXMax(WINDOW_SIZE);
  main_window.setYMin(-WINDOW_SIZE);
  main_window.setYMax(WINDOW_SIZE);


  //inicialização da tela principal
  GtkBuilder  *builder;

  gtk_init(&argc, &argv);

  builder = gtk_builder_new();
  gtk_builder_add_from_file(builder, "window.glade", NULL);

  //todos os objetos do arquivo window.glade que precisamos pegar dados ou alterar dados deles
  window = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(builder), "main_window") );
  drawing_area = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(builder), "drawing_area") );


  passo_input_navegation = GTK_ENTRY( gtk_builder_get_object( builder, "passo_input_navegation" ) );
  passo_input_zoom = GTK_ENTRY( gtk_builder_get_object( builder, "passo_input_zoom" ) );

  combo_box = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(builder), "combo") );
  combo_box = gtk_combo_box_text_new();
  gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(combo_box), NULL, "Don't install.");
  gtk_combo_box_set_active(GTK_COMBO_BOX(combo_box), 1);




  //list_store = gtk_list_store_new( 1, G_TYPE_STRING );


     /* gtk_list_store_append( list_store, &iter );
    gtk_list_store_set( list_store, &iter, 0, "Hello World once", -1 );
    gtk_list_store_prepend( list_store, &iter );
    gtk_list_store_set( list_store, &iter, 0, "Hello World twice", -1 );
    gtk_list_store_insert( list_store, &iter, 1 );
    gtk_list_store_set( list_store, &iter, 0, "Hello World last time", -1 );*/
  /*list_store = gtk_list_store_new(3, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);
  for(int i=0; i<10; i++) {
    gtk_list_store_insert_with_values(list_store, NULL, -1, 0, "Default", 1, "white", 2, "black", -1);
  }*/

  //combo_box = gtk_combo_box_new_with_model(GTK_TREE_MODEL(list_store));

  /*const char *distros[] = {"Select distribution", "Fedora", "Mint", "Suse"};

  for (int i = 0; i < G_N_ELEMENTS (distros); i++)
  {
    gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (combo_box), distros[i]);
  }*/
  //g_signal_connect (combo_box, "changed", G_CALLBACK (on_changed), NULL);

  gtk_container_add (GTK_CONTAINER (window), combo_box);

  g_signal_connect (drawing_area, "draw", G_CALLBACK (redraw), NULL);
  g_signal_connect (drawing_area,"configure-event", G_CALLBACK (create_surface), NULL);


  gtk_builder_connect_signals(builder, NULL);
  gtk_widget_show(window);
  gtk_main ();
  return 0;
}