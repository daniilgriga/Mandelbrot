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
    {  "v1"    , optional_argument, nullptr, 'v'},
    {  "v2"    , optional_argument, nullptr, 'f'},
    {  "v3"    , optional_argument, nullptr, 'i'},
    {  nullptr ,         0        , nullptr,  0 }
};

int main (int argc, char* argv[])
{
    struct Params_t startParams = { ROI_X, 0.f, 1.f };

    bool useGraphics = false;
    int iterations = 0;
    int version = 0;
    int option = 0;

    while ((option = getopt_long (argc, argv, "gr:v::f::i::", long_options, nullptr)) != -1)
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
                    fprintf (stderr, "Incorrect form -> usage:\n%s [--v<number> -r<number> | --v<number> --run=<number>]\n", argv[0]);
                    return 1;
                }
                break;
            }

            case 'v':
            {
                version = 1;
                break;
            }

            case 'f':
            {
                version = 2;
                break;
            }

            case 'i':
            {
                version = 3;
                break;
            }

            case '?':
            {
                fprintf (stderr, "Usage:\n%s [-g|--graphics]\nor\n%s [--v<number> -r<number> | --v<number> --run=<number>]\n", argv[0], argv[0]);
                return 1;
            }

            default:
                fprintf (stderr, "Error: Unexpected option processing error\n");
        }
    }


    if (useGraphics)
    {
        GraphicsPart (&startParams, &version);       // if i add keyboard handler -> needs a error handler
    }
    else
    {
        double fullTime = 0;

        switch (version)
        {
            case 1:
            {
                fprintf (stderr, "Mandelbrot v1 started >>>\n");

                for (int i = 0; i < iterations; i++)
                    fullTime += RunMandelbrot_v1 (nullptr, &startParams, false);

                break;
            }

            case 2:
            {
                fprintf (stderr, "Mandelbrot v2 started >>>\n");

                for (int i = 0; i < iterations; i++)
                    fullTime += RunMandelbrot_v2 (nullptr, &startParams, false);

                break;
            }

            case 3:
            {
                fprintf (stderr, "Mandelbrot v3 started >>>\n");

                for (int i = 0; i < iterations; i++)
                    fullTime += RunMandelbrot_v3 (nullptr, &startParams, false);

                break;
            }

            default:
                fprintf (stderr, "Version dont choosed - usage: [--v<number> -r<number> | --v<number> --run=<number>]");
        }

        for (int i = 0; i < iterations; i++)
            fullTime += RunMandelbrot_v2 (nullptr, &startParams, false);

        fprintf (stderr, "Mandelbrot algorithm <%d> times took: <%g> seconds!\n", iterations, fullTime);
    }

    return 0;
}
// TODO     1. inline
//          2. intrinsics
