#include <stdio.h>
#include <time.h>
#include <SFML/Graphics.hpp>

#include "graphics.hpp"
#include "mandelbrot_alg.hpp"

#define SIZE_ARR 4

double RunMandelbrot_v1 (sf::Image* image, struct Params_t* cond, bool GraphicsFlag)
{
    struct timespec start, end;
    clock_gettime (CLOCK_MONOTONIC, &start);

    for (unsigned int iy = 0; iy < SIZE_Y; iy++)
    {
        float x_0 =  (                       - (float) SIZE_X*cond->scale/2) * cond->dx + cond->xc;
        float y_0 =  ((float) iy*cond->scale - (float) SIZE_Y*cond->scale/2) * cond->dy + cond->yc;

        for (unsigned int ix = 0; ix < SIZE_X; ix++, x_0 += cond->dx*cond->scale)
        {
            float x = x_0;
            float y = y_0;

            int N = 0;

            for ( ; N < N_max; N++)
            {
                float x_2 = x * x;
                float y_2 = y * y;
                float x_y = x * y;

                float r_2 = x_2 + y_2;

                if (r_2 >= r_2_max)
                    break;

                x = x_2 - y_2 + x_0;
                y = x_y + x_y + y_0;
            }

            if (GraphicsFlag)
            {
                sf::Color color;
                color = sf::Color::Black;

                if (N != N_max)
                {
                    if (N % 2 == 1)
                        color = sf::Color::Blue;
                    else
                        color = sf::Color::Black;
                    if (N > 75)
                        color = sf::Color::White;
                }

                image->setPixel (ix, iy, color);
            }
        }
    }

    clock_gettime (CLOCK_MONOTONIC, &end);

    return (double) (end.tv_sec  - start.tv_sec) +
           (double) (end.tv_nsec - start.tv_nsec) / 1000000000.0;
}


double RunMandelbrot_v2 (sf::Image* image, struct Params_t* cond, bool GraphicsFlag)
{
    struct timespec start, end;
    clock_gettime (CLOCK_MONOTONIC, &start);

    float x_0_array[SIZE_ARR] = {};
    float x[SIZE_ARR] = {};
    float y[SIZE_ARR] = {};

    for (unsigned int iy = 0; iy < SIZE_Y; iy++)
    {
        float x_0 =  (                       - (float) SIZE_X*cond->scale/2) * cond->dx + cond->xc;
        float y_0 =  ((float) iy*cond->scale - (float) SIZE_Y*cond->scale/2) * cond->dy + cond->yc;

        for (unsigned int ix = 0; ix < SIZE_X; ix += SIZE_ARR, x_0 += cond->dx*cond->scale*SIZE_ARR)
        {
            for (int i = 0; i < SIZE_ARR; i++) x_0_array[i] = x_0 + cond->dx*((float)i)*cond->scale;
            for (int i = 0; i < SIZE_ARR; i++) x[i] = x_0_array[i];
            for (int i = 0; i < SIZE_ARR; i++) y[i] = y_0;

            int N[SIZE_ARR] = {};

            for (int n = 0; n < N_max; n++)
            {
                float x_2[SIZE_ARR] = {};
                float y_2[SIZE_ARR] = {};
                float x_y[SIZE_ARR] = {};
                for (int i = 0; i < SIZE_ARR; i++) x_2[i] = x[i] * x[i];
                for (int i = 0; i < SIZE_ARR; i++) y_2[i] = y[i] * y[i];
                for (int i = 0; i < SIZE_ARR; i++) x_y[i] = x[i] * y[i];

                float r_2[SIZE_ARR] = {};
                for (int i = 0; i < SIZE_ARR; i++) r_2[i] = x_2[i] + y_2[i];

                int compare[SIZE_ARR] = {};
                for (int i = 0; i < SIZE_ARR; i++) if (r_2[i] <= r_2_max) compare[i] = 1;

                int mask = 0;
                for (int i = 0; i < SIZE_ARR; i++) mask |= (compare[i] << i);
                if (!mask)
                    break;

                for (int i = 0; i < SIZE_ARR; i++) N[i] = N[i] + compare[i];

                for (int i = 0; i < SIZE_ARR; i++) x[i] = x_2[i] - y_2[i] + x_0_array[i];
                for (int i = 0; i < SIZE_ARR; i++) y[i] = x_y[i] * 2 + y_0;
            }

            if (GraphicsFlag)
            {
                sf::Color color;
                color = sf::Color::Black;

                for (unsigned int i = 0; i < SIZE_ARR; i++)
                {
                    if (N[i] < N_max)
                    {
                        if (N[i] % 2 == 1)
                            color = sf::Color::Blue;
                        else
                            color = sf::Color::Black;
                        if (N[i] > 75)
                            color = sf::Color::White;
                    }

                    image->setPixel (ix + i, iy, color);
                }
            }
        }
    }

    clock_gettime (CLOCK_MONOTONIC, &end);

    return (double) (end.tv_sec  - start.tv_sec) +
           (double) (end.tv_nsec - start.tv_nsec) / 1000000000.0;
}
