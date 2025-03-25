#include <stdio.h>
#include <SFML/Graphics.hpp>

#include "mandelbrot_alg.hpp"

int main (int argc, const char* argv[])
{
    sf::RenderWindow window (sf::VideoMode (800, 800), "Mandelbrot");

    sf::Image image;
    image.create (800, 800, sf::Color::Black);

    RunMandelbrot (image);

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
