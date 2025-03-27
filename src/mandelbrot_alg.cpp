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

void RunMandelbrot (sf::Image* image, bool GraphicsFlag)
{
    float xc = 0.f, yc = 0.f, scale = 1.f;

    for (unsigned int iy = 0; iy < 800; iy++)
    {
        float x_0 = ( (          - 400.f) * dx + ROI_X + xc ) * scale;
        float y_0 = ( ((float)iy - 400.f) * dy + ROI_Y + yc ) * scale;

        for (unsigned int ix = 0; ix < 800; ix++, x_0 += dx)
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
}

int GraphicsPart (void)
{
    sf::RenderWindow window (sf::VideoMode (800, 800), "Mandelbrot");

    sf::Image image;
    image.create (800, 800, sf::Color::Black);

    RunMandelbrot (&image, true);

    sf::Texture texture;
    texture.loadFromImage (image);

    sf::Sprite sprite (texture);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent (event))
        {
            if (event.type == sf::Event::Closed)
                window.close ();
        }

        window.clear ();
        window.draw (sprite);
        window.display ();
    }

    return 0;
}
