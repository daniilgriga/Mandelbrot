#ifndef MANDELBROT_H
#define MANDELBROT_H

#include "graphics.hpp"

#define SIZE_ARR 8

struct Params_t
{
    float xc,
          yc,
       scale;

    const float dx = 2.0f/SIZE_X;
    const float dy = 2.0f/SIZE_Y;
    const float ds = 1.2f;
};

typedef float mandl_t;

const int   N_max = 256;
const float r_2_max = 10.f;

const float ROI_X = -1.325f;  // Region Of Interest
const float ROI_Y = 0;

double RunMandelbrot_v1 (sf::Image* image, struct Params_t* cond, bool GraphicsFlag);

double RunMandelbrot_v2 (sf::Image* image, struct Params_t* cond, bool GraphicsFlag);

double RunMandelbrot_v3 (sf::Image* image, struct Params_t* cond, bool GraphicsFlag);

#endif // MANDELBROT_H
