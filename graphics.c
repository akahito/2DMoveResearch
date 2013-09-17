#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "graphics.h"

static void draw_point(Graphic *graphic, Point *point) {
	if(point != NULL && point->next!= NULL) {
		Coord from_pixel = coord_to_pixel_graphic(graphic, point->coord);
		Coord to_pixel = coord_to_pixel_graphic(graphic, point->next->coord);

		gdk_draw_line(graphic->widget->window,
			graphic->widget->style->fg_gc[gtk_widget_get_state(graphic->widget)],
			from_pixel.x,
			from_pixel.y,
			to_pixel.x,
			to_pixel.y);
		draw_point(graphic, point->next);
	}
}

static void draw_coordinate_system(Graphic *graphic) {
	double x;
	double y;
	double start_x;
	double start_y;
	double center_x;
	double center_y;
	double fraction;
	double integer;
	int number;
	int number_first;
	int number_count;
	char *number_str;
	Coord center_pixel;
	Coord left_top_coord;
	Coord right_bottom_coord;
	Coord interval_pixel = graphic->interval;

	interval_pixel.x *= graphic->pixel_in_coord.x;
	interval_pixel.y *= graphic->pixel_in_coord.y;

	center_pixel.x = 0;
	center_pixel.y = 0;
	center_pixel = coord_to_pixel_graphic(graphic, center_pixel);

	left_top_coord = pixel_to_coord_graphic(graphic, 0, 0);
	right_bottom_coord = pixel_to_coord_graphic(graphic, graphic->pixel_width, graphic->pixel_height);

	if(0 > center_pixel.y)
		center_y = 0;
	else if(center_pixel.y > graphic->pixel_height - 20)
		center_y = graphic->pixel_height - 20;
	else center_y = center_pixel.y;

	fraction = modf((double) center_pixel.x / interval_pixel.x, &integer);

	number_first = -((int) integer + 1);
	start_x = - interval_pixel.x + fraction * interval_pixel.x;

	fraction = modf((double) graphic->pixel_width / interval_pixel.x, &integer);
	number_count = (int) integer + 2;

	if(fraction > 0)
		number_count++;

	x = start_x;
	number = number_first;

	for(int i = 0; i < number_count; i++) {
		if(number != 0) {
			number_str = g_strdup_printf("%.1f", (number * graphic->interval.x));
			gdk_draw_text(graphic->widget->window,
				gdk_font_load("-misc-fixed-medium-r-normal-*-100-*-*-*-*-*-*"),
				graphic->widget->style->fg_gc[gtk_widget_get_state(graphic->widget)],
				x,
				center_y + 10,
				number_str,
				strlen(number_str));
			g_free(number_str);
		}

		x += interval_pixel.x;
		number++;
	}

	if(center_pixel.x > graphic->pixel_width - 20)
		center_x = graphic->pixel_width - 20;
	else if(center_pixel.x < 0)
		center_x = 0;
	else
		center_x = center_pixel.x;
		
	fraction = modf((double) center_pixel.y / interval_pixel.y, &integer);

	number_first = -((int) integer + 1);
	start_y = - interval_pixel.y + fraction * interval_pixel.y;

	fraction = modf((double) graphic->pixel_height / interval_pixel.y, &integer);
	number_count = (int) integer + 2;

	if(fraction > 0)
		number_count++;

	y = start_y;
	number = number_first;

	for(int i = 0; i < number_count; i++) {
		if(number != 0) {
			number_str = g_strdup_printf("%.1f", (-number * graphic->interval.y));
			gdk_draw_text(graphic->widget->window,
				gdk_font_load("-misc-fixed-medium-r-normal-*-100-*-*-*-*-*-*"),
				graphic->widget->style->fg_gc[gtk_widget_get_state(graphic->widget)],
				center_x + 5,
				y,
				number_str,
				strlen(number_str));
			g_free(number_str);
		}

		y += interval_pixel.y;
		number++;
	}

	// frame
	gdk_draw_line(graphic->widget->window,
		graphic->widget->style->fg_gc[gtk_widget_get_state(graphic->widget)],
		graphic->pixel_width - 1,
		0,
		graphic->pixel_width - 1,
		graphic->pixel_height);

	// ordinate cursor
	gdk_draw_line(graphic->widget->window,
		graphic->widget->style->fg_gc[gtk_widget_get_state(graphic->widget)],
		center_x - 10,
		10,
		center_x,
		0);
	gdk_draw_line(graphic->widget->window,
		graphic->widget->style->fg_gc[gtk_widget_get_state(graphic->widget)],
		center_x + 10,
		10,
		center_x,
		0);

	// abscissa cursor
	gdk_draw_line(graphic->widget->window,
		graphic->widget->style->fg_gc[gtk_widget_get_state(graphic->widget)],
		graphic->pixel_width - 10,
		center_y - 10,
		graphic->pixel_width,
		center_y);
	gdk_draw_line(graphic->widget->window,
		graphic->widget->style->fg_gc[gtk_widget_get_state(graphic->widget)],
		graphic->pixel_width - 10,
		center_y + 10,
		graphic->pixel_width,
		center_y);

	// abscissa line
	gdk_draw_line(graphic->widget->window,
		graphic->widget->style->fg_gc[gtk_widget_get_state(graphic->widget)],
		0,
		center_y,
		graphic->pixel_width,
		center_y);

	// ordinate line
	gdk_draw_line(graphic->widget->window,
		graphic->widget->style->fg_gc[gtk_widget_get_state(graphic->widget)],
		center_x,
		0,
		center_x,
		graphic->pixel_height);

	// center O
	gdk_draw_text(graphic->widget->window,
		gdk_font_load("-misc-fixed-medium-r-normal-*-200-*-*-*-*-*-*"),
		graphic->widget->style->fg_gc[gtk_widget_get_state(graphic->widget)],
		center_x + 3,
		center_pixel.y - 3,
		"O",
		1);

	// abscissa name
	gdk_draw_text(graphic->widget->window,
		gdk_font_load("-misc-fixed-medium-r-normal-*-200-*-*-*-*-*-*"),
		graphic->widget->style->fg_gc[gtk_widget_get_state(graphic->widget)],
		graphic->pixel_width - 20,
		center_y - 20,
		graphic->abscissa,
		strlen(graphic->abscissa));

	// ordinate name
	gdk_draw_text(graphic->widget->window,
		gdk_font_load("-misc-fixed-medium-r-normal-*-200-*-*-*-*-*-*"),
		graphic->widget->style->fg_gc[gtk_widget_get_state(graphic->widget)],
		center_x + 20,
		20,
		graphic->ordinate,
		strlen(graphic->ordinate));
}

void configure_screen_graphic(Graphic *graphic) {
	GtkAllocation allocation = graphic->widget->allocation;

	graphic->center_displacement.x = allocation.width / ( 2 * graphic->pixel_in_coord.x);
	graphic->center_displacement.y = allocation.height / ( 2 * graphic->pixel_in_coord.y);
	graphic->pixel_width = allocation.width;
	graphic->pixel_height = allocation.height;

	if(graphic->pixmap)
		g_object_unref(graphic->pixmap);

	graphic->pixmap = gdk_pixmap_new(graphic->widget->window,
		allocation.width,
		allocation.height,
		-1);
	gdk_draw_rectangle(graphic->pixmap,
		graphic->widget->style->white_gc,
		TRUE,
		0,
		0,
		allocation.width,
		allocation.height);
}

static gboolean configure_event(GtkWidget *widget, GdkEventConfigure *event, Graphic* graphic) {
	configure_screen_graphic(graphic);

	draw_graphic(graphic, TRUE);

	return TRUE;
}

static gboolean expose_event(GtkWidget *widget, GdkEventExpose *event, Graphic *graphic) {
	draw_graphic(graphic, TRUE);

	return TRUE;
}

void create_graphic(Graphic *graphic) {
	graphic->pixel_width = 0;
	graphic->pixel_height = 0;
	graphic->pixmap = NULL;
	graphic->points.first = NULL;
	graphic->points.end = NULL;
	graphic->points.count = 0;

	g_signal_connect(G_OBJECT(graphic->widget), "configure_event", G_CALLBACK(configure_event), graphic);
	g_signal_connect(G_OBJECT(graphic->widget), "expose_event", G_CALLBACK(expose_event), graphic);
}

void draw_graphic(Graphic *graphic, gboolean repain) {
	if(repain == TRUE || graphic->type == MOVING_ABSCISSA) {
  		gdk_draw_drawable(graphic->widget->window,
			graphic->widget->style->fg_gc[gtk_widget_get_state(graphic->widget)],
			graphic->pixmap,
			0, 0,
			0, 0,
			graphic->pixel_width, graphic->pixel_height);
		draw_coordinate_system(graphic);
	}

	draw_point(graphic, graphic->points.first);
}

static void add_point(Graphic *graphic, Coord coord) {
	Points *points = &graphic->points;
	Point *new_point = (Point *) malloc(sizeof(Point));
	new_point->next = NULL;
	new_point->prev = NULL;
	new_point->coord = coord;

	if(points->count == 0) {
		points->first = new_point;
		points->end = new_point;
		points->count = 1;
	}
	else {
		new_point->next = points->first;
		points->first->prev = new_point;
		points->first = new_point;

		if(points->count >= POINTS_COUNT) {
			Point *end = points->end;
			Point *prev = end->prev;
			Point *first = points->first;

			prev->next = NULL;
			points->end = prev;

			first->prev = new_point;
			points->first = new_point;

			free(end);
		}
		else
			points->count++;
	}
}

static void clear_point(Point *point) {
	Point *next;
	if(point != NULL) {
		next = point->next;

		free(point);
		clear_point(next);
	}
}

void clear_graphic(Graphic *graphic) {
	clear_point(graphic->points.first);
	graphic->points.first = NULL;
	graphic->points.end = NULL;
	graphic->points.count = 0;
	graphic->displacement.x = 0;
	graphic->displacement.y = 0;

	configure_screen_graphic(graphic);
	draw_graphic(graphic, TRUE);
}

void add_coord_to_graphic(Graphic *graphic, Coord coord) {
	Coord pixel = coord_to_pixel_graphic(graphic, coord);

	if(graphic->type == MOVING_ABSCISSA && graphic->points.first)
		graphic->displacement.x -= (coord.x - graphic->points.first->coord.x);

	add_point(graphic, coord);
	draw_graphic(graphic, FALSE);
}

Coord pixel_to_coord_graphic(Graphic *graphic, double x, double y) {
	Coord coord;

	coord.x = ((double) x / graphic->pixel_in_coord.x) - (graphic->displacement.x + graphic->center_displacement.x);
	coord.y = -(((double) y / graphic->pixel_in_coord.y) - (graphic->displacement.y + graphic->center_displacement.y));

	return coord;
}

Coord coord_to_pixel_graphic(Graphic *graphic, Coord coord) {
	Coord pixel;

	pixel.x = (coord.x + graphic->displacement.x + graphic->center_displacement.x) * graphic->pixel_in_coord.x;
	pixel.y = (-coord.y + graphic->displacement.y + graphic->center_displacement.y) * graphic->pixel_in_coord.y;

	return pixel;
}
