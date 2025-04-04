#include <stdio.h>
#include <time.h>
#include <SFML/Graphics.hpp>
#include <immintrin.h>

#include "graphics.hpp"
#include "mandelbrot_alg.hpp"

#define SIZE_ARR 8

const __m256 r_2_max_arr = _mm256_set1_ps (r_2_max);

double RunMandelbrot_v1 (sf::Image* image, struct Params_t* cond, bool GraphicsFlag)
{
    struct timespec start, end;
    clock_gettime (CLOCK_MONOTONIC, &start);

    for (unsigned int iy = 0; iy < SIZE_Y; iy++)
    {
        float x_0 =  (                       - (float) SIZE_X*cond->scale/2) * cond->dx + cond->xc;
        float y_0 =  ((float) iy*cond->scale - (float) SIZE_Y*cond->scale/2) * cond->dy + cond->yc;

        for (unsigned int ix = 0; ix < SIZE_X; ix++, x_0 += cond->dx*cond->scale)
        {
            float x = x_0;
            float y = y_0;

            int N = 0;

            for ( ; N < N_max; N++)
            {
                float x_2 = x * x;
                float y_2 = y * y;
                float x_y = x * y;

                float r_2 = x_2 + y_2;

                if (r_2 >= r_2_max)
                    break;

                x = x_2 - y_2 + x_0;
                y = x_y + x_y + y_0;
            }

            if (GraphicsFlag)
            {
                sf::Color color;
                color = sf::Color::Black;

                if (N != N_max)
                {
                    if (N % 2 == 1)
                        color = sf::Color::Blue;
                    else
                        color = sf::Color::Black;
                    if (N > 75)
                        color = sf::Color::White;
                }

                image->setPixel (ix, iy, color);
            }
        }
    }

    clock_gettime (CLOCK_MONOTONIC, &end);

    return (double) (end.tv_sec  - start.tv_sec) +
           (double) (end.tv_nsec - start.tv_nsec) / 1000000000.0;
}


double RunMandelbrot_v2 (sf::Image* image, struct Params_t* cond, bool GraphicsFlag)
{
    struct timespec start, end;
    clock_gettime (CLOCK_MONOTONIC, &start);

    float x_0_array[SIZE_ARR] = {};
    float x[SIZE_ARR] = {};
    float y[SIZE_ARR] = {};

    for (unsigned int iy = 0; iy < SIZE_Y; iy++)
    {
        float x_0 =  (                       - (float) SIZE_X*cond->scale/2) * cond->dx + cond->xc;
        float y_0 =  ((float) iy*cond->scale - (float) SIZE_Y*cond->scale/2) * cond->dy + cond->yc;

        for (unsigned int ix = 0; ix < SIZE_X; ix += SIZE_ARR, x_0 += cond->dx*cond->scale*SIZE_ARR)
        {
            for (int i = 0; i < SIZE_ARR; i++) x_0_array[i] = x_0 + cond->dx*((float)i)*cond->scale;
            for (int i = 0; i < SIZE_ARR; i++) x[i] = x_0_array[i];
            for (int i = 0; i < SIZE_ARR; i++) y[i] = y_0;

            int N[SIZE_ARR] = {};

            for (int n = 0; n < N_max; n++)
            {
                float x_2[SIZE_ARR] = {}; for (int i = 0; i < SIZE_ARR; i++) x_2[i] = x[i] * x[i];
                float y_2[SIZE_ARR] = {}; for (int i = 0; i < SIZE_ARR; i++) y_2[i] = y[i] * y[i];
                float x_y[SIZE_ARR] = {}; for (int i = 0; i < SIZE_ARR; i++) x_y[i] = x[i] * y[i];

                float r_2[SIZE_ARR] = {};
                for (int i = 0; i < SIZE_ARR; i++) r_2[i] = x_2[i] + y_2[i];

                int compare[SIZE_ARR] = {};
                for (int i = 0; i < SIZE_ARR; i++) if (r_2[i] <= r_2_max) compare[i] = 1;

                int mask = 0;
                for (int i = 0; i < SIZE_ARR; i++) mask |= (compare[i] << i);
                if (!mask)
                    break;

                for (int i = 0; i < SIZE_ARR; i++) N[i] = N[i] + compare[i];

                for (int i = 0; i < SIZE_ARR; i++) x[i] = x_2[i] - y_2[i] + x_0_array[i];
                for (int i = 0; i < SIZE_ARR; i++) y[i] = x_y[i] * 2 + y_0;
            }

            if (GraphicsFlag)
            {
                sf::Color color;
                color = sf::Color::Black;

                for (unsigned int i = 0; i < SIZE_ARR; i++)
                {
                    if (N[i] < N_max)
                    {
                        if (N[i] % 2 == 1)
                            color = sf::Color::Blue;
                        else
                            color = sf::Color::Black;
                        if (N[i] > 75)
                            color = sf::Color::White;
                    }

                    image->setPixel (ix + i, iy, color);
                }
            }
        }
    }

    clock_gettime (CLOCK_MONOTONIC, &end);

    return (double) (end.tv_sec  - start.tv_sec) +
           (double) (end.tv_nsec - start.tv_nsec) / 1000000000.0;
}

double RunMandelbrot_v3 (sf::Image* image, struct Params_t* cond, bool GraphicsFlag)
{
    struct timespec start, end;
    clock_gettime (CLOCK_MONOTONIC, &start);

    for (unsigned int iy = 0; iy < SIZE_Y; iy++)
    {
        float x_0 =  (                       - (float) SIZE_X*cond->scale/2) * cond->dx + cond->xc;
        float y_0 =  ((float) iy*cond->scale - (float) SIZE_Y*cond->scale/2) * cond->dy + cond->yc;

        for (unsigned int ix = 0; ix < SIZE_X; ix += SIZE_ARR, x_0 += cond->dx*cond->scale*SIZE_ARR)
        {
            __m256 x_0_array = _mm256_add_ps ( _mm256_set1_ps (x_0),
                                               _mm256_mul_ps  ( _mm256_set_ps (7.f, 6.f, 5.f, 4.f, 3.f, 2.f, 1.f, 0.f),
                                               _mm256_set1_ps (cond->dx*cond->scale) )                                 );
            __m256 y_0_array = _mm256_set1_ps (y_0);
            __m256 x = x_0_array;
            __m256 y = y_0_array;

            __m256i N  = _mm256_setzero_si256 ();
            __m256 r_2 = _mm256_setzero_ps ();

            for (int n = 0; n < N_max; n++)
            {
                __m256 x_2 = _mm256_mul_ps (x, x);
                __m256 y_2 = _mm256_mul_ps (y, y);
                __m256 x_y = _mm256_mul_ps (x, y);

                r_2 = _mm256_add_ps (x_2, y_2);

                __m256 compare = _mm256_cmp_ps (r_2, r_2_max_arr, _CMP_LE_OQ);

                int mask = 0; mask = _mm256_movemask_ps (compare);
                if (!mask)
                    break;

                N = _mm256_add_epi32 (N, _mm256_castps_si256 (compare));
                x = _mm256_add_ps (_mm256_sub_ps (x_2, y_2), x_0_array);
                y = _mm256_add_ps (_mm256_add_ps (x_y, x_y), y_0_array);
            }

            if (GraphicsFlag)
            {
                sf::Color color;
                color = sf::Color::Black;

                int N_arr[8] = {};

                _mm256_storeu_si256 ( (__m256i*) N_arr, N);

                for (unsigned int i = 0; i < SIZE_ARR; i++)
                {
                    int abs_N = abs (N_arr[i]);

                    if (abs_N == N_max)
                    {
                        color.r = 0;
                        color.g = 0;
                        color.b = 0;
                    }
                    else
                    {
                        float t = (float) abs_N / (float) N_max;
                        int r = (int) (255*2.5 * t) + 7;
                        int g = (int) (255*2.5 * t) + 7;
                        int b = 0;
                        color.r = (sf::Uint8) (r > 255 ? 255 : r);
                        color.g = (sf::Uint8) (g > 255 ? 255 : g);
                        color.b = (sf::Uint8) b;
                    }

                    image->setPixel (ix + i, iy, color);
                }
            }
        }
    }

    clock_gettime (CLOCK_MONOTONIC, &end);

    return (double) (end.tv_sec  - start.tv_sec) +
           (double) (end.tv_nsec - start.tv_nsec) / 1000000000.0;
}
