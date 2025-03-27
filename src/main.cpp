#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <SFML/Graphics.hpp>

#include "mandelbrot_alg.hpp"

static struct option long_options[] =
{
    {"graphics",       no_argument, NULL, 'g'},
    {  "run"   , required_argument, NULL, 'r'},
    {   NULL   ,         0        , NULL,  0 }
};

int GraphicsPart (void);

int main (int argc, char* argv[]) // REVIEW
{
    bool useGraphics = false;
    int iterations = 0;
    int option = 0;

    while ((option = getopt_long (argc, argv, "gr:", long_options, NULL)) != -1)
    {
        switch (option)
        {
            case 'g':
                useGraphics = true;
                break;

            case 'r':
                iterations = atoi (optarg);
                break;

            case '?':
                fprintf (stderr, "Usage:\n%s [-g|--graphics]\nor\n%s [-r|--run]=<number>\n", argv[0], argv[0]);
                return 1;

            default:
                fprintf (stderr, "Error: Unexpected option processing error\n");
        }
    }

    if (useGraphics)
        GraphicsPart ();       // if i add keyboard handler -> needs a error handler
    else
    {
        for (int i = 0; i < iterations; i++)
            RunMandelbrot (nullptr, false);
    }

    return 0;
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
