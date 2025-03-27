#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <SFML/Graphics.hpp>

#include "mandelbrot_alg.hpp"

static struct option long_options[] =
{
    {"graphics",       no_argument, nullptr, 'g'},
    {  "run"   , required_argument, nullptr, 'r'},
    {  nullptr ,         0        , nullptr,  0 }
};

int main (int argc, char* argv[]) // REVIEW
{
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
                iterations = atoi (optarg);
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
        GraphicsPart ();       // if i add keyboard handler -> needs a error handler
    else
    {
        for (int i = 0; i < iterations; i++)
            RunMandelbrot (nullptr, false);
    }

    return 0;
}
