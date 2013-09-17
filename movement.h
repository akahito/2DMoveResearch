#ifndef MOVEMENT_H
#define MOVEMENT_H

#include <sys/time.h>
#include "graphics.h"

#define MOVEMENT_DRAW_INTERVAL 0.050000
#define MOVEMENT_INTERVAL 0.000100

void movement_mouse_up();
void listen_movement(Graphic *movement_graphic, Graphic* graphic);

#endif /* MOVEMENT_H */
