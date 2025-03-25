#ifndef MANDELBROT_H
#define MANDELBROT_H

const int   N_max = 256;
const int r_2_max = 10;

const float dx = 1.0f/800;
const float dy = 1.0f/800;

const float ROI_X = -1.325f;  // Region Of Interest
const float ROI_Y = 0;

int MandelbrotAlgorithm (float x_0, float y_0);

void RunMandelbrot (sf::Image* image);

#endif // MANDELBROT_H
