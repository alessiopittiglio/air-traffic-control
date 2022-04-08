#pragma once 

// framerate
const int FPS = 30;          // frames per second

// speed parameters in km/h
const int DEFAULT_SPEED = 700;
const int MIN_SPEED = 400;
const int MAX_SPEED = 800;
const int DELTA_SPEED = 100;

// altitude parameters in foots
const int DEFAULT_ALTITUDE = 30000;
const int MIN_ALTITUDE = 25000;
const int MAX_ALTITUDE = 33000;
const int DELTA_ALTITUDE = 1000;

// distances for map generation (in pixel)
const int D_MIN = 400;
const int D_MAX = 800;
const double RADIUS_NODE = 15.0;

// time spawn
const int MIN_TIME = 12;
const int MAX_TIME = 15;

// convert entity speed from km/h to pixel/s (divided by SPEEDCONVERT)
const float SPEEDCONVERT = 20.0f;

// define the number of ticks used to predict future 
const unsigned int NTICKS = 500;

// define the minimum vertical distance (altitude) between two airplanes
// before detecting a collision
const unsigned int DMINVERTCOL = 1000;
