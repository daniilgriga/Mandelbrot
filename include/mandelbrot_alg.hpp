#ifndef MANDELBROT_H
#define MANDELBROT_H

struct Params_t
{
    float xc,
          yc,
       scale;

    const float dx = 1.0f/400;
    const float dy = 1.0f/400;
    const float ds = 1.2f;
};

const int   N_max = 256;
const int r_2_max = 10;

const float ROI_X = -1.325f;  // Region Of Interest
const float ROI_Y = 0;

float RunMandelbrot_v1 (sf::Image* image, struct Params_t* cond, bool GraphicsFlag);

#endif // MANDELBROT_H
