#include <stdio.h>
#include <string.h>
#include <SFML/Graphics.hpp>

#include "mandelbrot_alg.hpp"

int main (int argc, const char* argv[])
{
    if (argc != 2)
    {
        fprintf (stderr, "Usage:\n%s --run=<number of iterations> [without grpahics]\nor\n%s --graphics [for graphics]\n", argv[0], argv[0]);
        return 1;
    }

    bool useGraphics = (argc == 2 && strcmp(argv[1], "--graphics") == 0);

    if (useGraphics)
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
    }
    else
    {
        if (strncmp (argv[1], "--run=", 6) != 0)
        {
            fprintf (stderr, "Error: Argument must be in the form --run=<number>\n");
            return 1;
        }

        const char* number_str = argv[1] + 6;
        char* endptr;
        long iterations = strtol (number_str, &endptr, 10);

        if (*endptr != '\0' || number_str == endptr)
        {
            fprintf(stderr, "Error: Invalid number in --run argument\n");
            return 1;
        }

        if (iterations < 0)
        {
            fprintf(stderr, "Error: Number of iterations must be non-negative\n");
            return 1;
        }

        for (int i = 0; i < iterations; i++)
            RunMandelbrot (nullptr, false);
    }
    return 0;
}
