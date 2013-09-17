#!/bin/bash

gcc main.c physics.c graphics.c models.c movement.c --std=c11 `pkg-config --cflags --libs gtk+-2.0` -o physical_movement
