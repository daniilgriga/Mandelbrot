#include <SFML/Graphics.hpp>
#include "mandelbrot_alg.hpp"
#include <graphics.hpp>

int GraphicsPart (struct Params_t* cond, int* version)
{
// =================== main Window =================== //
    sf::RenderWindow    window (sf::VideoMode (SIZE_X, SIZE_Y), "Mandelbrot");
    sf::Image image;
    sf::Texture texture;
// =================================================== //

    sf::Font font;
    if (!font.loadFromFile ("build/arial_b.ttf"))
    {
        fprintf (stderr, "Error: Cannot load font\n");
        return 1;
    }

    sf::RectangleShape fpsField (sf::Vector2f (110.f, 50.f));
    fpsField.setPosition (4, 746);
    fpsField.setFillColor (sf::Color::Black);
    fpsField.setOutlineThickness (2);
    fpsField.setOutlineColor (sf::Color::White);

    sf::Text fpsText;
    fpsText.setFont (font);
    fpsText.setCharacterSize (24);
    fpsText.setFillColor (sf::Color::White);
    fpsText.setPosition ( fpsField.getPosition ().x + 10,
                          fpsField.getPosition ().y + (fpsField.getSize ().y - fpsText.getGlobalBounds ().height) / 2 - 12);

    sf::Clock clock;
    float lastTime = clock.getElapsedTime().asSeconds();

    float totalTime = 0.0f;
    int frameCount = 0;
    float averageFps = 0.0f;
    char resultFps[15] = {};

    sf::Clock restartClock;

    while (window.isOpen())
    {
        image.create (SIZE_X, SIZE_Y, sf::Color::Black);

        switch (*version)
        {
            case 1:
            {
                fprintf (stderr, "Mandelbrot [graphics] v1 started >>>\n");

                RunMandelbrot_v1 (&image, cond, true);
                break;
            }

            case 2:
            {
                fprintf (stderr, "Mandelbrot [graphics] v2 started >>>\n");

                RunMandelbrot_v2 (&image, cond, true);
                break;
            }

            case 3:
            {
                fprintf (stderr, "Mandelbrot [graphics] v3 started >>>\n");

                RunMandelbrot_v3 (&image, cond, true);
                break;
            }

            default:
                fprintf (stderr, "Version wasn't selected - usage: [--v<number> -g | --v<number> --graphics]");
        }

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

        fprintf (stderr, "xc = %.5f yc = %.5f scale = %.5f\n", cond->xc, cond->yc, cond->scale);

// ============================ FPS ============================= //
        //float currentTime = clock.restart().asSeconds(); // REVIEW

        float currentTime = clock.getElapsedTime().asSeconds();

        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;
        totalTime += deltaTime;
        frameCount++;

        if (totalTime > 0.0f)
            averageFps = (float) frameCount / totalTime;

        snprintf (resultFps, sizeof (resultFps), "FPS: %.f", averageFps);

        fpsText.setString (resultFps);

        if (restartClock.getElapsedTime().asSeconds() >= 1.0f)
        {
            totalTime = 0.0f;
            frameCount = 0;
            restartClock.restart();
        }
// ============================================================== //

        window.clear();
        window.draw (sprite);
        window.draw (fpsField);
        window.draw (fpsText);
        window.display();
    }

    return 0;
}
