#include <SFML/Graphics.hpp>
#include "mandelbrot_alg.hpp"
#include <graphics.hpp>

int GraphicsPart (struct Params_t* cond)
{
// =================== main Window =================== //
    sf::RenderWindow    window (sf::VideoMode (SIZE_X, SIZE_Y), "Mandelbrot");
    sf::Image image;
    sf::Texture texture;
// =================================================== //

// =================== FPS Window ==================== //
    sf::RenderWindow fpsWindow (sf::VideoMode (200,  100), "FPS");

    sf::Font font;
    if (!font.loadFromFile ("build/arial_b.ttf"))
    {
        fprintf (stderr, "Error: Cannot load font\n");
        return 1;
    }

    sf::Text fpsText;
    fpsText.setFont (font);
    fpsText.setCharacterSize (24);
    fpsText.setFillColor (sf::Color::White);
    fpsText.setPosition (10, 10);

    float fps = 0;
    const int frameCount = 100;
    float frameTimes[frameCount] = {};
    int frameIndex = 0;
    int filledFrames = 0;
    char result[15] = {};

    while (window.isOpen())
    {
        image.create (SIZE_X, SIZE_Y, sf::Color::Black);

        sf::Clock clock;

        RunMandelbrot (&image, cond, true);
        texture.loadFromImage (image);

        sf::Sprite sprite (texture);
        sf::Event event;

        while (window.pollEvent (event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
            window.close();

        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Right)
            cond->xc += cond->dx * cond->scale;

        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Left)
            cond->xc -= cond->dx * cond->scale;

        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Down)
            cond->yc += cond->dy * cond->scale;

        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Up)
            cond->yc -= cond->dy * cond->scale;

        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Z)
            cond->scale /= cond->ds;

        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::A)
            cond->scale *= cond->ds;

        fprintf (stderr, "xc = %f yc = %f scale = %f\n", cond->xc, cond->yc, cond->scale);

// ============================ FPS ============================= // // FIXME
        float currentTime = clock.restart().asSeconds(); // REVIEW

        frameTimes[frameIndex] = currentTime;
        frameIndex = (frameIndex + 1) % frameCount;

        if (filledFrames < frameCount)
            filledFrames++;

        float totalTime = 0.f;
        for (int i = 0; i < filledFrames; i++)
            totalTime += frameTimes[i];

        float averageTime = totalTime / (float)filledFrames;

        if (averageTime > 0)
            fps = 1.f / averageTime;
        else
            fps = 0;

        snprintf (result, sizeof (result), "FPS: %.f", fps);
        fpsText.setString (result);

        fpsWindow.clear (sf::Color::Black);
        fpsWindow.draw (fpsText);
        fpsWindow.display ();
// ============================================================== //

        window.clear();
        window.draw (sprite);
        window.display();
    }

    return 0;
}
