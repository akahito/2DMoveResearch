#include <stdio.h>
#include <gtk/gtk.h>
#include <unistd.h>
#include "graphics.h"
#include "models.h"
#include "movement.h"

static GtkWidget *widget;
static GtkWidget *movement_area;
static GtkWidget *graphic_area;
static Graphic movement_graphic;
static Graphic graphic;
static Model **models;

static gboolean quit(GtkWidget *widget, GdkEvent *event, Graphic* graphic) {
	gtk_main_quit();

	return TRUE;
}

static gboolean change_model(Model *model) {
	movement_mouse_up();
	graphic.ordinate = model->ordinate;
	graphic.abscissa = model->abscissa;
	graphic.calculate = model->calculate;
	graphic.interval = model->interval;
	graphic.pixel_in_coord = model->pixel_in_coord;
	clear_graphic(&graphic);
	clear_graphic(&movement_graphic);
}

static gboolean callback_change_model(GtkWidget *widget, Model *model) {
	change_model(model);

	return TRUE;
}

static gboolean callback_button_right(GtkWidget *widget, Graphic *graphic) {
	graphic->displacement.x -= (double) graphic->pixel_width / (4 * graphic->pixel_in_coord.x);
	draw_graphic(graphic, TRUE);

	return TRUE;
}

static gboolean callback_button_left(GtkWidget *widget, Graphic *graphic) {
	graphic->displacement.x += (double) graphic->pixel_width / (4 * graphic->pixel_in_coord.x);
	draw_graphic(graphic, TRUE);

	return TRUE;
}

static gboolean callback_button_up(GtkWidget *widget, Graphic *graphic) {
	graphic->displacement.y += (double) graphic->pixel_height / (4 * graphic->pixel_in_coord.y);
	draw_graphic(graphic, TRUE);

	return TRUE;
}

static gboolean callback_button_down(GtkWidget *widget, Graphic *graphic) {
	graphic->displacement.y -= (double) graphic->pixel_height / (4 * graphic->pixel_in_coord.y);
	draw_graphic(graphic, TRUE);

	return TRUE;
}
static gboolean callback_button_plus(GtkWidget *widget, Graphic *graphic) {
	if(graphic->interval.x >= 0.2 && graphic->interval.y >= 0.2) {
		graphic->pixel_in_coord.x *= 2;
		graphic->pixel_in_coord.y *= 2;

		graphic->interval.x /= 2;
		graphic->interval.y /= 2;

		configure_screen_graphic(graphic);
		draw_graphic(graphic, TRUE);
	}

	return TRUE;
}

static gboolean callback_button_minus(GtkWidget *widget, Graphic *graphic) {
	graphic->pixel_in_coord.x /= 2;
	graphic->pixel_in_coord.y /= 2;
	graphic->interval.x *= 2;
	graphic->interval.y *= 2;

	configure_screen_graphic(graphic);
	draw_graphic(graphic, TRUE);
	return TRUE;
}

static void gui_init() {
	GtkWidget *box;
	GtkWidget *graphics_box;
	GtkWidget *movement_box;
	GtkWidget *graphic_box;
	GtkWidget *menu;
	GtkWidget *movement_menu;
	GtkWidget *graphic_menu;
	GtkWidget *button;
	GtkWidget *button_up_1;
	GtkWidget *button_down_1;
	GtkWidget *button_left_1;
	GtkWidget *button_right_1;
	GtkWidget *button_plus_1;
	GtkWidget *button_minus_1;
	GtkWidget *button_up_2;
	GtkWidget *button_down_2;
	GtkWidget *button_left_2;
	GtkWidget *button_right_2;
	GtkWidget *button_plus_2;
	GtkWidget *button_minus_2;
	Model *model;
	int models_count = get_models_count();

	widget = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	box = gtk_vbox_new(FALSE, 0);
	graphics_box = gtk_hbox_new(FALSE, 0);
	graphic_box = gtk_vbox_new(FALSE, 0);
	movement_box = gtk_vbox_new(FALSE, 0);
	movement_menu = gtk_vbox_new(FALSE, 0);
	graphic_menu = gtk_vbox_new(FALSE, 0);
	menu = gtk_hbox_new(FALSE, 0);
	movement_area = gtk_drawing_area_new();
	graphic_area = gtk_drawing_area_new();

	button_up_1 = gtk_button_new_with_label("↑");
	button_down_1 = gtk_button_new_with_label("↓");
	button_left_1 = gtk_button_new_with_label("←");
	button_right_1 = gtk_button_new_with_label("→");
	button_plus_1 = gtk_button_new_with_label("+");
	button_minus_1 = gtk_button_new_with_label("-");
	button_up_2 = gtk_button_new_with_label("↑");
	button_down_2 = gtk_button_new_with_label("↓");
	button_left_2 = gtk_button_new_with_label("←");
	button_right_2 = gtk_button_new_with_label("→");
	button_plus_2 = gtk_button_new_with_label("+");
	button_minus_2 = gtk_button_new_with_label("-");

	g_signal_connect (button_minus_1, "clicked", G_CALLBACK(callback_button_minus), &movement_graphic);
	g_signal_connect (button_plus_1, "clicked", G_CALLBACK(callback_button_plus), &movement_graphic);
	g_signal_connect (button_up_1, "clicked", G_CALLBACK(callback_button_up), &movement_graphic);
	g_signal_connect (button_down_1, "clicked", G_CALLBACK(callback_button_down), &movement_graphic);
	g_signal_connect (button_left_1, "clicked", G_CALLBACK(callback_button_left), &movement_graphic);
	g_signal_connect (button_right_1, "clicked", G_CALLBACK(callback_button_right), &movement_graphic);
	g_signal_connect (button_minus_2, "clicked", G_CALLBACK(callback_button_minus), &graphic);
	g_signal_connect (button_plus_2, "clicked", G_CALLBACK(callback_button_plus), &graphic);
	g_signal_connect (button_up_2, "clicked", G_CALLBACK(callback_button_up), &graphic);
	g_signal_connect (button_down_2, "clicked", G_CALLBACK(callback_button_down), &graphic);
	g_signal_connect (button_left_2, "clicked", G_CALLBACK(callback_button_left), &graphic);
	g_signal_connect (button_right_2, "clicked", G_CALLBACK(callback_button_right), &graphic);

	gtk_drawing_area_size(GTK_DRAWING_AREA(movement_area), 650, 700);
	gtk_drawing_area_size(GTK_DRAWING_AREA(graphic_area), 650, 700);

	g_signal_connect(G_OBJECT(widget), "delete_event", G_CALLBACK(quit), NULL);

	gtk_widget_set_events (movement_area, GDK_EXPOSURE_MASK
		| GDK_LEAVE_NOTIFY_MASK
		| GDK_BUTTON_PRESS_MASK
		| GDK_POINTER_MOTION_MASK
		| GDK_POINTER_MOTION_HINT_MASK);

	for(int i = 0; i < models_count; i++) {
		model = models[i];
		button = gtk_button_new_with_label(model->name);
		gtk_container_add(GTK_CONTAINER(menu), button);
		g_signal_connect (button, "clicked", G_CALLBACK(callback_change_model), model);
	}

	gtk_container_add(GTK_CONTAINER(movement_box), movement_area);
	gtk_container_add(GTK_CONTAINER(graphic_box), graphic_area);
	gtk_container_add(GTK_CONTAINER(graphics_box), movement_menu);
	gtk_container_add(GTK_CONTAINER(graphics_box), movement_box);
	gtk_container_add(GTK_CONTAINER(graphics_box), graphic_box);
	gtk_container_add(GTK_CONTAINER(graphics_box), graphic_menu);
	gtk_container_add(GTK_CONTAINER(box), graphics_box);
	gtk_container_add(GTK_CONTAINER(box), menu);
	gtk_container_add(GTK_CONTAINER(widget), box);
	gtk_container_add(GTK_CONTAINER(movement_menu), button_plus_1);
	gtk_container_add(GTK_CONTAINER(movement_menu), button_minus_1);
	gtk_container_add(GTK_CONTAINER(movement_menu), button_up_1);
	gtk_container_add(GTK_CONTAINER(movement_menu), button_down_1);
	gtk_container_add(GTK_CONTAINER(movement_menu), button_left_1);
	gtk_container_add(GTK_CONTAINER(movement_menu), button_right_1);
	gtk_container_add(GTK_CONTAINER(graphic_menu), button_plus_2);
	gtk_container_add(GTK_CONTAINER(graphic_menu), button_minus_2);
	gtk_container_add(GTK_CONTAINER(graphic_menu), button_up_2);
	gtk_container_add(GTK_CONTAINER(graphic_menu), button_down_2);
	gtk_container_add(GTK_CONTAINER(graphic_menu), button_left_2);
	gtk_container_add(GTK_CONTAINER(graphic_menu), button_right_2);
}

static void create_movement_graphic() {
	movement_graphic.pixmap = NULL;
	movement_graphic.pixel_in_coord.x = 50;
	movement_graphic.pixel_in_coord.y = 50;
	movement_graphic.displacement.x = 0;
	movement_graphic.displacement.y = 0;
	movement_graphic.interval.x = 1;
	movement_graphic.interval.y = 1;
	movement_graphic.abscissa = "x";
	movement_graphic.ordinate = "y";
	movement_graphic.type = STATIC_COORDINATES;
	movement_graphic.widget = movement_area;

	create_graphic(&movement_graphic);
	listen_movement(&movement_graphic, &graphic);
}

static void create_model_graphic() {
	graphic.pixmap = NULL;
	graphic.pixel_in_coord.x = 60;
	graphic.pixel_in_coord.y = 20;
	graphic.displacement.x = 0;
	graphic.displacement.y = 0;
	graphic.interval.x = 1;
	graphic.interval.y = 1;
	graphic.abscissa = "t";
	graphic.ordinate = "v";
	graphic.type = MOVING_ABSCISSA;
	graphic.widget = graphic_area;

	create_graphic(&graphic);
}

int main(int argc, char **argv) {
	gtk_init(&argc, &argv);
	models_init();
	models = get_models();
	gui_init();


	create_movement_graphic();
	create_model_graphic();

	gtk_widget_show_all(widget);
	change_model(models[0]);
	gtk_main();

	return 0;
}
