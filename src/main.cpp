#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <SFML/Graphics.hpp>

#include "mandelbrot_alg.hpp"
#include "graphics.hpp"

static struct option long_options[] =
{
    {"graphics",       no_argument, nullptr, 'g'},
    {  "run"   , required_argument, nullptr, 'r'},
    {  nullptr ,         0        , nullptr,  0 }
};

int main (int argc, char* argv[]) // REVIEW
{
    struct Init_t cond = { 0.f, 0.f, 1.f }; // params

    bool useGraphics = false;
    int iterations = 0;
    int option = 0;

    while ((option = getopt_long (argc, argv, "gr:", long_options, nullptr)) != -1)
    {
        switch (option)
        {
            case 'g':
                useGraphics = true;
                break;

            case 'r':
                iterations = atoi (optarg); // FIXME - not atoi -> scanf
                if (iterations == 0)
                {
                    fprintf (stderr, "Error: Wrong syntax >>> usage: %s [-r|--run]=<number>\n", argv[0]);
                    return 1;
                }
                break;

            case '?':
                fprintf (stderr, "Usage:\n%s [-g|--graphics]\nor\n%s [-r|--run]=<number>\n", argv[0], argv[0]);
                return 1;

            default:
                fprintf (stderr, "Error: Unexpected option processing error\n");
        }
    }

    if (useGraphics)
    {
        GraphicsPart (&cond);       // if i add keyboard handler -> needs a error handler
    }
    else
    {
        float fullTime = 0;

        for (int i = 0; i < iterations; i++)
            fullTime += RunMandelbrot (nullptr, &cond, false);

        fprintf (stderr, "Mandelbrot algorithm <%d> times took: <%g> seconds!\n", iterations, fullTime);
    }

    return 0;
}
