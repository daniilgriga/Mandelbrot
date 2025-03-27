#include <stdio.h>
#include <SFML/Graphics.hpp>

#include "mandelbrot_alg.hpp"

int MandelbrotAlgorithm (float x_0, float y_0)
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

    return N;
}

float RunMandelbrot (sf::Image* image, struct Init_t* cond, bool GraphicsFlag)
{
    sf::Clock clock; // TODO

    for (unsigned int iy = 0; iy < 800; iy++)
    {
        float x_0 = ( (          - 400.f) * cond->dx + ROI_X + cond->xc ) * cond->scale;
        float y_0 = ( ((float)iy - 400.f) * cond->dy + ROI_Y + cond->yc ) * cond->scale;

        for (unsigned int ix = 0; ix < 800; ix++, x_0 += cond->dx)
        {
            int N = MandelbrotAlgorithm (x_0, y_0);

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

    return clock.getElapsedTime().asSeconds();
}
