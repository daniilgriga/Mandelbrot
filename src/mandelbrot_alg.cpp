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

void RunMandelbrot (sf::Image* image)
{
    float xc = 0.f, yc = 0.f, scale = 1.f;

    for (unsigned int iy = 0; iy < 800; iy++)
    {
        float x_0 = ( (          - 400.f) * dx + ROI_X + xc ) * scale;
        float y_0 = ( ((float)iy - 400.f) * dy + ROI_Y + yc ) * scale;

        for (unsigned int ix = 0; ix < 800; ix++, x_0 += dx)
        {
            int N = MandelbrotAlgorithm (x_0, y_0);

            sf::Color color;
            color = sf::Color::Black;

            if (N != N_max)
            {
                color.r = (sf::Uint8) ((N * 255) / N_max);
                color.g = (sf::Uint8) ((N * 128) / N_max);
                color.b = (sf::Uint8) (255 - (N * 255) / N_max);

                color = sf::Color (color.r, color.g, color.b);
            }

            image->setPixel (ix, iy, color);

        }
    }
}
