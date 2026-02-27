#include <SFML/Graphics.hpp>
#include <cstdint>
#include <optional>
#include "mandelbrot_alg.hpp"
#include <graphics.hpp>

int GraphicsPart (struct Params_t* cond, int* version)
{
// =================== main Window =================== //
    sf::RenderWindow window (sf::VideoMode ({SIZE_X, SIZE_Y}), "Mandelbrot");
    sf::Image image;
    sf::Texture texture;
// =================================================== //

    sf::Font font;
    const bool hasFont = font.openFromFile ("assets/fonts/DejaVuSans-Bold.ttf");
    if (!hasFont)
        fprintf (stderr, "Warning: Cannot load font assets/fonts/DejaVuSans-Bold.ttf, FPS overlay disabled\n");

    sf::RectangleShape fpsField (sf::Vector2f (150.f, 50.f));
    fpsField.setPosition ({4.f, 746.f});
    fpsField.setFillColor (sf::Color::Black);
    fpsField.setOutlineThickness (2);
    fpsField.setOutlineColor (sf::Color::White);

    std::optional<sf::Text> fpsText;
    if (hasFont)
    {
        fpsText.emplace (font, "", 24);
        fpsText->setFillColor (sf::Color::White);
        fpsText->setPosition ({fpsField.getPosition ().x + 10.f,
                               fpsField.getPosition ().y + (fpsField.getSize ().y - fpsText->getGlobalBounds ().size.y) / 2.f - 12.f});
    }

    sf::Clock clock;
    float lastTime = clock.getElapsedTime().asSeconds();

    float totalTime = 0.0f;
    int frameCount = 0;
    float averageFps = 0.0f;
    char resultFps[15] = {};

    sf::Clock restartClock;

    while (window.isOpen())
    {
        image.resize ({SIZE_X, SIZE_Y}, sf::Color::Black);

        switch (*version)
        {
            case 1:
            {
                fprintf (stderr, "Mandelbrot [graphics] v1 >>> ");

                RunMandelbrot_v1 (&image, cond, true);
                break;
            }

            case 2:
            {
                fprintf (stderr, "Mandelbrot [graphics] v2 >>> ");

                RunMandelbrot_v2 (&image, cond, true);
                break;
            }

            case 3:
            {
                fprintf (stderr, "Mandelbrot [graphics] v3 >>> ");

                RunMandelbrot_v3 (&image, cond, true);
                break;
            }

            default:
                fprintf (stderr, "Version wasn't selected - usage: [--v<number> -g | --v<number> --graphics]");
        }

        if (!texture.loadFromImage (image))
        {
            fprintf (stderr, "Error: Cannot load image to texture\n");
            return 1;
        }

        sf::Sprite sprite (texture);
        while (const std::optional event = window.pollEvent ())
        {
            if (event->is<sf::Event::Closed> ())
            {
                window.close();
                continue;
            }

            const auto* keyPressed = event->getIf<sf::Event::KeyPressed> ();
            if (keyPressed == nullptr)
                continue;

            if (keyPressed->code == sf::Keyboard::Key::Escape)
                window.close();

            if (keyPressed->code == sf::Keyboard::Key::Right)
                cond->xc += cond->dx * cond->scale;

            if (keyPressed->code == sf::Keyboard::Key::Left)
                cond->xc -= cond->dx * cond->scale;

            if (keyPressed->code == sf::Keyboard::Key::Down)
                cond->yc += cond->dy * cond->scale;

            if (keyPressed->code == sf::Keyboard::Key::Up)
                cond->yc -= cond->dy * cond->scale;

            if (keyPressed->code == sf::Keyboard::Key::Z)
                cond->scale /= cond->ds;

            if (keyPressed->code == sf::Keyboard::Key::A)
                cond->scale *= cond->ds;
        }

        fprintf (stderr, "xc = %.5f yc = %.5f scale = %.5f\n", cond->xc, cond->yc, cond->scale);

// ============================ FPS ============================= //
        // float currentTime = clock.restart().asSeconds();

        float currentTime = clock.getElapsedTime().asSeconds();

        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;
        totalTime += deltaTime;
        frameCount++;

        if (totalTime > 0.0f)
            averageFps = (float) frameCount / totalTime;

        snprintf (resultFps, sizeof (resultFps), "FPS: %.1f", averageFps);

        if (fpsText.has_value ())
            fpsText->setString (resultFps);

        if (restartClock.getElapsedTime().asSeconds() >= 1.0f)
        {
            totalTime = 0.0f;
            frameCount = 0;
            restartClock.restart();
        }
// ============================================================== //

        window.clear();
        window.draw (sprite);
        if (fpsText.has_value ())
        {
            window.draw (fpsField);
            window.draw (*fpsText);
        }
        window.display();
    }

    return 0;
}

void SetPixels (sf::Image* image, unsigned int ix, unsigned int iy, int* N)
{
    sf::Color color;
    color = sf::Color::Black;

    for (unsigned int i = 0; i < SIZE_ARR; i++)
    {
        int abs_N = abs (N[i]);

        if (abs_N == N_max)
        {
            color.r = 0;
            color.g = 0;
            color.b = 0;
        }
        else
        {
            mandl_t t = (mandl_t) abs_N / (mandl_t) N_max;
            int r = (int) (255*2.5 * t) + 7;
            int g = (int) (255*2.5 * t) + 7;
            int b = 0;
            color.r = static_cast<std::uint8_t> (r > 255 ? 255 : r);
            color.g = static_cast<std::uint8_t> (g > 255 ? 255 : g);
            color.b = static_cast<std::uint8_t> (b);
        }

        image->setPixel ({ix + i, iy}, color);
    }
}
