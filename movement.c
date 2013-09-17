#include "movement.h"
	
static struct Graphics graphics;
static struct timeval movement_time;
static double movement_interval = 0;
static double movement_count = 0;
static double movement_value = 0;
static gboolean mouse_down = FALSE;

struct Graphics {
	Graphic *graphic;
	Graphic *movement_graphic;
};

static gboolean motion_notify_event(GtkWidget *widget, GdkEventMotion *event, struct Graphics *graphics) {
	Graphic *graphic = graphics->graphic;
	Graphic *movement_graphic = graphics->movement_graphic;
	int x;
	int y;
	Coord coord;
	GdkModifierType state;
	static gboolean first_run = TRUE;
	struct timeval new_time;
	double interval;

	gettimeofday(&new_time, NULL);

	if(first_run == FALSE) {
		gdk_window_get_pointer(event->window, &x, &y, &state);
	
		if(mouse_down == TRUE) {
			interval = (new_time.tv_sec - movement_time.tv_sec) + ((double) (new_time.tv_usec - movement_time.tv_usec))/1000000;
			movement_interval += interval;

			if(movement_interval >= MOVEMENT_INTERVAL) {
				coord = pixel_to_coord_graphic(movement_graphic, x, y);
				add_coord_to_graphic(movement_graphic, coord);
	
				if(movement_graphic->points.count >= 3) {
					Point *point = graphic->points.first;
					Point *A = movement_graphic->points.first;
					Point *B = A->next;
					Point *C = B->next;
					Coord new_coord;

					movement_value += graphic->calculate(A->coord, B->coord, C->coord, movement_interval);
					movement_count++;

					if(movement_interval >= MOVEMENT_DRAW_INTERVAL) {
						new_coord.x = movement_interval;

						if(point != NULL)
							new_coord.x += point->coord.x;
	
						new_coord.y = movement_value / movement_count;
						add_coord_to_graphic(graphic, new_coord);
						movement_count = 0;
						movement_interval = 0;
						movement_value = 0;
					}
				}
			}
		}	
	}
	else
		first_run = FALSE;

	movement_time = new_time;

	return TRUE;
}

static gboolean button_press_event(GtkWidget *widget, GdkEventButton *event, struct Graphics *graphics) {
	if(mouse_down == TRUE)
		mouse_down = FALSE;
	else
		mouse_down = TRUE;
	return TRUE;
}

void movement_mouse_up() {
	mouse_down = FALSE;
}

void listen_movement(Graphic *movement_graphic, Graphic *graphic) {
	graphics.movement_graphic = movement_graphic;
	graphics.graphic = graphic;

	g_signal_connect(G_OBJECT(movement_graphic->widget), "motion_notify_event", G_CALLBACK(motion_notify_event), &graphics);
	g_signal_connect(G_OBJECT(movement_graphic->widget), "button_press_event", G_CALLBACK(button_press_event), &graphics);
}
