#ifndef MANDELBROT_H
#define MANDELBROT_H

struct Init_t
{
    float xc = 0.f,
          yc = 0.f,
       scale = 1.f;

    const float dx = 1.0f/400;
    const float dy = 1.0f/400;
    const float ds = 1.2f;
};

const int   N_max = 256;
const int r_2_max = 10;

const float ROI_X = -1.325f;  // Region Of Interest
const float ROI_Y = 0;

int MandelbrotAlgorithm (float x_0, float y_0);

float RunMandelbrot (sf::Image* image, struct Init_t* cond, bool GraphicsFlag);

#endif // MANDELBROT_H
