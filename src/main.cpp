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

int main (int argc, char* argv[])
{
    struct Params_t startParams = { ROI_X, 0.f, 1.f };

    bool useGraphics = false;
    int iterations = 0;
    int option = 0;

    while ((option = getopt_long (argc, argv, "gr:", long_options, nullptr)) != -1)
    {
        switch (option)
        {
            case 'g':
            {
                useGraphics = true;
                break;
            }

            case 'r':
            {
                int error = sscanf (optarg, "%d", &iterations);
                if (error == 0)
                {
                    fprintf (stderr, "Incorrect form -> usage:\n%s [-r<number>|--run=<number>]\n", argv[0]);
                    return 1;
                }
                break;
            }

            case '?':
            {
                fprintf (stderr, "Usage:\n%s [-g|--graphics]\nor\n%s [-r<number>|--run=<number>]\n", argv[0], argv[0]);
                return 1;
            }

            default:
                fprintf (stderr, "Error: Unexpected option processing error\n");
        }
    }

    if (useGraphics)
    {
        GraphicsPart (&startParams);       // if i add keyboard handler -> needs a error handler
    }
    else
    {
        double fullTime = 0;

        for (int i = 0; i < iterations; i++)
            fullTime += RunMandelbrot_v2 (nullptr, &startParams, false);

        fprintf (stderr, "Mandelbrot algorithm <%d> times took: <%g> seconds!\n", iterations, fullTime);
    }

    return 0;
}
