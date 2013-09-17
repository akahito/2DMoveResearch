#ifndef MODELS_H
#define MODELS_H

#include "graphics.h"

typedef struct Model {
	char *name;
	char *ordinate;
	char *abscissa;
	Coord interval;
	Coord pixel_in_coord;
	double (*calculate)(Coord A, Coord B, Coord C, double interval);
} Model;

Model** get_models();
int get_models_count();
void models_init();

#endif /* MODELS_H */
