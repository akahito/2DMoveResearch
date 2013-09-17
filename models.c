#include <math.h>
#include <stdlib.h>
#include "models.h"

#define MODELS_COUNT 3

static const int models_count = MODELS_COUNT;
static Model* models[MODELS_COUNT];
static Model speed;
static Model acceleration;
static Model centripetal;

static double get_distance(Coord A, Coord B) {
	double distance = sqrt((A.x - B.x) * (A.x - B.x) + (A.y - B.y) * (A.y - B.y));

	return distance;
}

static double get_speed(Coord A, Coord B, double interval) {
	double distance = get_distance(A, B);
	double speed = distance / interval;

	return speed;
}

static double get_acceleration(Coord A, Coord B, Coord C, double interval) {
	double speed_1 = get_speed(B, C, interval);
	double speed_2 = get_speed(A, B, interval);
	double acceleration = (speed_1 - speed_2)/interval;

	return acceleration;
}

double calculate_speed(Coord A, Coord B, Coord C, double interval) {
	return get_speed(A, B, interval);
}

double calculate_acceleration(Coord A, Coord B, Coord C, double interval) {
	return get_acceleration(A, B, C, interval);
}

double calculate_centripetal(Coord A, Coord B, Coord C, double interval) {
	double v = get_speed(A, B, interval);
	double a = get_distance(B, C);
	double b = get_distance(A, C);
	double c = get_distance(A, B);
	double p = (a + b + c ) / 2;
	double R = a * b * c / (4 * (p - a) * (p - b) * (p - c));

	if(R != 0)
		return v * v / R;
	return 0;
}

Model** get_models() {
	return models;
}

int get_models_count() {
	return models_count;
}

void models_init() {
	speed.name = "Скорость";
	speed.ordinate = "v";
	speed.abscissa = "t";
	speed.interval.x = 1;
	speed.interval.y = 1;
	speed.pixel_in_coord.x = 50;
	speed.pixel_in_coord.y = 20;
	speed.calculate = &calculate_speed;
	acceleration.name = "Ускорение";
	acceleration.ordinate = "a";
	acceleration.abscissa = "t";
	acceleration.interval.x = 1;
	acceleration.interval.y = 10;
	acceleration.pixel_in_coord.x = 60;
	acceleration.pixel_in_coord.y = 5;
	acceleration.calculate = &calculate_acceleration;
	centripetal.name = "Центростремительное ускорение";
	centripetal.ordinate = "a";
	centripetal.abscissa = "t";
	centripetal.interval.x = 1;
	centripetal.interval.y = 1;
	centripetal.pixel_in_coord.x = 60;
	centripetal.pixel_in_coord.y = 20;
	centripetal.calculate = &calculate_centripetal;

	models[0] = &speed;
	models[1] = &acceleration;
	models[2] = &centripetal;
}
