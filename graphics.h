#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <gtk/gtk.h>

#define POINTS_COUNT 1000

typedef enum Graphic_type {MOVING_ABSCISSA, STATIC_COORDINATES} Graphic_type;

typedef struct Coord {
	double x;
	double y;
} Coord;

typedef struct Point {
	struct Point *next;
	struct Point *prev;
	Coord coord;
} Point;

typedef struct Points {
	Point *first;
	Point *end;
	int count;
} Points;

typedef struct Graphic {
	Points points;

	int pixel_width;
	int pixel_height;
	double (*calculate)(Coord A, Coord B, Coord C, double interval);
	char *abscissa;
	char *ordinate;
	Graphic_type type;
	Coord pixel_in_coord;
	Coord displacement;
	Coord center_displacement;
	Coord interval;
	GtkWidget *widget;
	GdkPixmap *pixmap;
} Graphic;

void create_graphic(Graphic *graphic);
void clear_graphic(Graphic *graphic);
void add_coord_to_graphic(Graphic *graphic, Coord coord);
void draw_graphic(Graphic *graphic, gboolean repain);
void configure_screen_graphic(Graphic *graphic);
Coord pixel_to_coord_graphic(Graphic *graphic, double x, double y);
Coord coord_to_pixel_graphic(Graphic *graphic, Coord coord);

#endif /* GRAPHICS_H */
