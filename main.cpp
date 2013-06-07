//      main.cpp
//
//      Copyright (C) 2010 Ali ElSayed <aelsay@gmail.com>
//
//      This program is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; either version 2 of the License, or
//      (at your option) any later version.
//
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//
//      You should have received a copy of the GNU General Public License
//      along with this program; if not, see <http://www.gnu.org/licenses/>.
//

#include <gtk/gtk.h>
#include <cairo.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <stack>
#include <cstdlib>
#include <climits>
#include <ctime>
#include <fstream>
#include <sstream>
#include <gdk/gdk.h>
#include <gdk/gdkkeysyms.h>

#include "data.h"
#include "graph.h"
#include "token.h"
#include "parse.h"
#include "postfix.h"
#include "evaluate.h"

#define WINDOW_TITLE		"xGraph"
#define WINDOW_WIDTH		900
#define WINDOW_HEIGHT	680
#define WINDOW_RESIZE	FALSE
#define TOOLBAR_HEIGHT	76
#define ICON_PATH			"icon.png"

using namespace std;

/**
 * Features to be added (if possible):
 * x_axis maximum and minimum should be able to change through interface by user (done)
 * user should be able to set the scale zoom (done - as 2 in 1 with above)
 * add line width option and line color option (line width - done)
 * background color option
 * graph auto scale depending on equation
 * graph numbers and labels (partial - using lines instead)
 * separate window(panel) for buttons and control (not needed atm)
 * pan over (or scroll through) graph (with smooth animations if possible)
 * Minima and Maxima enable/disable toggle
 * graph paper design interface (done)
 * if user clicks a point on the graph, display that point
 * option to show "zeros" where graph hits the x-axis
 * save graph as image (done)
 * add program icon (done)
 * zoom in/out by scrolling (was done, taken out for spin button issues)
 * scientific calculator feature (done!)
 * **/

/*g++ -Wall -o xGraph main.cpp data.cpp graph.cpp parse.cpp postfix.cpp evaluate.cpp token.cpp $(pkg-config --cflags --libs cairo gtk+-2.0 gthread-2.0)*/

Graph *get_graph_instance()
{
	Data *data = Data::get_data_instance(0, TOOLBAR_HEIGHT, WINDOW_WIDTH, 
		WINDOW_HEIGHT);
	Graph *graph = data->get_graph_instance();
	return graph;
}

static gboolean plot_graph(GtkWidget *widget, gpointer data)
{
	Graph *graph = get_graph_instance();
	graph->graph_plot_on_window(widget, data);
	if(graph->graph_is_function_get() == false)
	{
		ostringstream stream;
		stream << graph->graph_constant_get();
		string value = stream.str();
		gtk_entry_set_text(GTK_ENTRY(data), value.c_str());
	}
	return FALSE;
}

static gboolean set_x_axis_range(GtkSpinButton *spin, gpointer data)
{
	double x_range = gtk_spin_button_get_value(GTK_SPIN_BUTTON(spin));
	Graph *graph = get_graph_instance();
	graph->graph_x_range_set(x_range);
	return FALSE;
}

static gboolean set_y_axis_range(GtkSpinButton *spin, gpointer data)
{
	double y_range = gtk_spin_button_get_value(GTK_SPIN_BUTTON(spin));
	Graph *graph = get_graph_instance();
	graph->graph_y_range_set(y_range);
	return FALSE;
}

static gboolean on_key_press(GtkWidget *widget, GdkEventKey *event, gpointer data)
{
	if(event->type == GDK_KEY_PRESS && 
		(event->keyval == GDK_Return || event->keyval == GDK_KP_Enter))
	{
		plot_graph(widget, data);
	}
	return FALSE;
}

static gboolean set_graph_quality(GtkWidget *widget, gpointer data)
{
	Graph *graph = get_graph_instance();
	if(gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(widget)) == TRUE)
		graph->graph_x_points_set(2000);
	else
		graph->graph_x_points_set(200);
	return FALSE;
}

void about_dialog(GtkWidget *widget, gpointer data)
{
	GtkWidget *dialog = gtk_about_dialog_new();
	gtk_about_dialog_set_program_name(GTK_ABOUT_DIALOG(dialog), "xGraph");
	gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(dialog), "0.5.2 beta");
	gtk_about_dialog_set_copyright(GTK_ABOUT_DIALOG(dialog), "Copyright (C) Ali ElSayed <aelsay@gmail.com>");
	gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(dialog), "This software is a simple and awesome graphing calculator program.");
	const gchar* gLicense = "This program is a graphing calculator project.\nCopyright (C) 2010  Ali ElSayed <aelsay@gmail.com>\n\nThis program is free software; you can redistribute it and/or\nmodify it under the terms of the GNU General Public License\nas published by the Free Software Foundation; either version 2\nof the License, or (at your option) any later version.\n\nThis program is distributed in the hope that it will be useful,\nbut WITHOUT ANY WARRANTY; without even the implied warranty of\nMERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the\nGNU General Public License for more details.\nYou should have received a copy of the GNU General Public License\nalong with this program; if not, write to the Free Software\nFoundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.\n\nFeel free to contact me at aelsay@gmail.com.\n";
	gtk_about_dialog_set_license(GTK_ABOUT_DIALOG(dialog), gLicense);
	gtk_dialog_run(GTK_DIALOG (dialog));
	gtk_widget_destroy(dialog);
}

void save_graph(GtkWidget *widget, gpointer data)
{
	Graph *graph = get_graph_instance();
	graph->graph_plot_on_surface(widget, data);
}

void set_line_width(GtkWidget *widget, gpointer data)
{
	double line_width = gtk_range_get_value(GTK_RANGE(widget));
	Graph *graph = get_graph_instance();
	graph->graph_line_width_set(line_width);
}

GdkPixbuf *create_icon(string icon_name)
{
	GError *error = NULL;
	GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(icon_name.c_str(), &error);
	if(pixbuf == NULL)
	{
		cout << error->message << endl;
		g_error_free(error);
	}
	return pixbuf;
}

int main(int argc, char** argv)
{
	/**Init the gtk library**/
	gtk_init(&argc, &argv);
	/**Create a window, set its size, position and title**/
	GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_widget_set_size_request(GTK_WIDGET(window), WINDOW_WIDTH, WINDOW_HEIGHT);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_window_set_title(GTK_WINDOW(window), WINDOW_TITLE);
	gtk_window_set_resizable(GTK_WINDOW(window), WINDOW_RESIZE);
	gtk_widget_set_app_paintable(GTK_WIDGET(window), TRUE);
	/**Add events to window**/
	gtk_widget_add_events(GTK_WIDGET(window), GDK_SCROLL_MASK);
	/**Set window icon**/
	gtk_window_set_icon(GTK_WINDOW(window), create_icon(ICON_PATH));
	/**Create a vbox and add it to the window container**/
	GtkWidget *vbox = gtk_vbox_new(FALSE, 0);
	gtk_container_add(GTK_CONTAINER(window), vbox);
	/**Create a toolbar, set its style and border width**/
	GtkWidget *toolbar = gtk_toolbar_new();
	gtk_toolbar_set_style(GTK_TOOLBAR(toolbar), GTK_TOOLBAR_ICONS);
	/**Set toolbar size**/
	gtk_widget_set_size_request(GTK_WIDGET(toolbar), WINDOW_WIDTH, 50);
	gtk_container_set_border_width(GTK_CONTAINER(toolbar), 0);
	/**Create three tool buttons from stock and two separators**/
	GtkToolItem *plot = gtk_tool_button_new_from_stock(GTK_STOCK_APPLY);
	GtkToolItem *separator1 = gtk_separator_tool_item_new();
	/**Set plot's label**/
	gtk_tool_button_set_label(GTK_TOOL_BUTTON(plot), "Plot");
	/**Create two widget items, a label and an entry**/
	GtkWidget *label = gtk_label_new(NULL);
	GtkWidget *xlabel = gtk_label_new(NULL);
	GtkWidget *ylabel = gtk_label_new(NULL);
	GtkWidget *xspin = gtk_spin_button_new_with_range(4.0, 100.0, 1.0);
	GtkWidget *yspin = gtk_spin_button_new_with_range(4.0, 100.0, 1.0);
	GtkWidget *entry = gtk_entry_new();
	GtkWidget *hscale = gtk_hscale_new_with_range(0.5, 2.5, 0.5); // hscale
	gtk_range_set_value(GTK_RANGE(hscale), 1.0); // set hscale value
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(xspin), 20.0); // set default x
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(yspin), 20.0); // & y range...
	gtk_widget_set_size_request(GTK_WIDGET(hscale), 50, 40); // set hscale size
	gtk_widget_set_size_request(GTK_WIDGET(entry), 150, 30); // set entry size
	gtk_entry_set_max_length(GTK_ENTRY(entry), 100); // set entry max length
	gtk_range_set_fill_level(GTK_RANGE(hscale), 100);
	/**Create a menu bar**/
	GtkAccelGroup *accel_group = gtk_accel_group_new();
	GtkWidget *menubar = gtk_menu_bar_new();
	GtkWidget *filemenu = gtk_menu_new(); //file menu
	GtkWidget *viewmenu = gtk_menu_new(); //view menu
	GtkWidget *helpmenu = gtk_menu_new(); //help menu
	GtkWidget *file = gtk_menu_item_new_with_mnemonic("_File");
	GtkWidget *view = gtk_menu_item_new_with_mnemonic("_View");
	GtkWidget *help = gtk_menu_item_new_with_mnemonic("_Help");
	GtkWidget *quit = gtk_image_menu_item_new_from_stock(GTK_STOCK_QUIT, accel_group);
	GtkWidget *save = gtk_image_menu_item_new_from_stock(GTK_STOCK_SAVE_AS, accel_group);
	GtkWidget *about = gtk_image_menu_item_new_from_stock(GTK_STOCK_ABOUT, NULL);
	GtkWidget *toggle = gtk_check_menu_item_new_with_label("High Quality");
	gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(toggle), TRUE);
	gtk_widget_add_accelerator(save, "activate", accel_group, GDK_s, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
	gtk_widget_add_accelerator(quit, "activate", accel_group, GDK_q, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
	gtk_widget_add_accelerator(toggle, "activate", accel_group, GDK_h, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
	gtk_window_add_accel_group(GTK_WINDOW(window), accel_group);
	/**Put all menu items in their menus**/
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(file), filemenu);
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(view), viewmenu);
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(help), helpmenu);
	gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), save);
	gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), quit);
	gtk_menu_shell_append(GTK_MENU_SHELL(viewmenu), toggle);
	gtk_menu_shell_append(GTK_MENU_SHELL(helpmenu), about);
	gtk_menu_shell_append(GTK_MENU_SHELL(menubar), file);
	gtk_menu_shell_append(GTK_MENU_SHELL(menubar), view);
	gtk_menu_shell_append(GTK_MENU_SHELL(menubar), help);
	gtk_box_pack_start(GTK_BOX(vbox), menubar, FALSE, FALSE, 0);
	/**Set label markup**/
	gtk_label_set_markup(GTK_LABEL(xlabel), "<span font='12.5' stretch= 'ultracondensed' style= 'italic'> x </span>");
	gtk_label_set_markup(GTK_LABEL(ylabel), "<span font='12.5' stretch= 'ultracondensed' style= 'italic'> y </span>");
	gtk_label_set_markup(GTK_LABEL(label), "<span font='12.5' stretch= 'ultracondensed' style= 'italic'> f(x)= </span>");
	/**Create new toolbar items that aren't buttons**/
	GtkToolItem *xlabel_item = gtk_tool_item_new();
	GtkToolItem *ylabel_item = gtk_tool_item_new();
	GtkToolItem *xspin_item = gtk_tool_item_new();
	GtkToolItem *yspin_item = gtk_tool_item_new();
	GtkToolItem *entry_item = gtk_tool_item_new();
	GtkToolItem *label_item = gtk_tool_item_new();
	GtkToolItem *hscale_item = gtk_tool_item_new();
	/**Add each gtk widget to its toolbar item container**/
	gtk_container_add(GTK_CONTAINER(xlabel_item), GTK_WIDGET(xlabel));
	gtk_container_add(GTK_CONTAINER(ylabel_item), GTK_WIDGET(ylabel));
	gtk_container_add(GTK_CONTAINER(xspin_item), GTK_WIDGET(xspin));
	gtk_container_add(GTK_CONTAINER(yspin_item), GTK_WIDGET(yspin));
	gtk_container_add(GTK_CONTAINER(entry_item), GTK_WIDGET(entry));
	gtk_container_add(GTK_CONTAINER(label_item), GTK_WIDGET(label));
	gtk_container_add(GTK_CONTAINER(hscale_item), GTK_WIDGET(hscale));
	/**Set items tooltip**/
	gtk_tool_item_set_tooltip_markup(GTK_TOOL_ITEM(plot), "Click Me To Plot Graph!");
	gtk_tool_item_set_tooltip_markup(GTK_TOOL_ITEM(xspin_item), "x-Axis Range");
	gtk_tool_item_set_tooltip_markup(GTK_TOOL_ITEM(yspin_item), "y-Axis Range");
	gtk_tool_item_set_tooltip_markup(GTK_TOOL_ITEM(hscale_item), "Adjust Line Width");
	gtk_tool_item_set_tooltip_markup(GTK_TOOL_ITEM(entry_item), "Enter Math Expression Here! (Ex: sin(x))");
	/**Allow entry_item to expand with win size**/
	gtk_tool_item_set_expand(GTK_TOOL_ITEM(entry_item), TRUE);
	/**Insert all items in the toolbar**/
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), xlabel_item, -1);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), xspin_item, -1);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), ylabel_item, -1);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), yspin_item, -1);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), hscale_item, -1);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), separator1, -1);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), label_item, -1);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), entry_item, -1);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), plot, -1);
	/**Start vbox**/
	gtk_box_pack_start(GTK_BOX(vbox), toolbar, FALSE, FALSE, 0);
	/**Connect signals**/
	g_signal_connect(G_OBJECT(save), "activate", G_CALLBACK(save_graph), NULL);
	g_signal_connect(G_OBJECT(quit), "activate", G_CALLBACK(gtk_main_quit), NULL);
	g_signal_connect(G_OBJECT(toggle), "activate", G_CALLBACK(set_graph_quality), NULL);
	g_signal_connect(G_OBJECT(about), "activate", G_CALLBACK(about_dialog), NULL);
	g_signal_connect(G_OBJECT(yspin), "value-changed", G_CALLBACK(set_y_axis_range), NULL);
	g_signal_connect(G_OBJECT(xspin), "value-changed", G_CALLBACK(set_x_axis_range), NULL);
	g_signal_connect(G_OBJECT(hscale), "value-changed", G_CALLBACK(set_line_width), NULL);
	g_signal_connect(G_OBJECT(plot), "clicked", G_CALLBACK(plot_graph), entry);
	g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
	g_signal_connect(G_OBJECT(window), "key-press-event", G_CALLBACK(on_key_press), entry);
	/**Show all widgets**/
	gtk_widget_show_all(window);
	/**Start gtk main loop and wait for events**/
	gtk_main();
	return 0;
}
