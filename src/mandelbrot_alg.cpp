#define MEASURE

#include <stdio.h>
#include <time.h>
#include <SFML/Graphics.hpp>
#include <immintrin.h>

#include "graphics.hpp"
#include "mandelbrot_alg.hpp"

inline void mm256_add_epi32	(int* dst, int* a, int* b)	           { for (int i = 0; i < SIZE_ARR; i++) dst[i] = a[i] + b[i];          }
inline void mm256_add_ps    (mandl_t* dst, mandl_t* a, mandl_t* b) { for (int i = 0; i < SIZE_ARR; i++) dst[i] = a[i] + b[i];          }
inline void mm256_sub_ps    (mandl_t* dst, mandl_t* a, mandl_t* b) { for (int i = 0; i < SIZE_ARR; i++) dst[i] = a[i] - b[i];          }
inline void mm256_mul_ps    (mandl_t* dst, mandl_t* a, mandl_t* b) { for (int i = 0; i < SIZE_ARR; i++) dst[i] = a[i] * b[i];          }
inline void mm_cpy_ps		(mandl_t* dst, mandl_t* src) 		   { for (int i = 0; i < SIZE_ARR; i++) dst[i] = src[i];               }
inline void mm256_cmple_ps  (int* a, mandl_t* b, mandl_t val)      { for (int i = 0; i < SIZE_ARR; i++) { if (b[i] <= val) a[i] = 1; } }

inline void mm256_set1_ps  (mandl_t* dst, mandl_t e) { for (int i = 0; i < SIZE_ARR; i++) dst[i] = e; }

inline void mm256_set_ps   (mandl_t* dst, mandl_t e7, mandl_t e6, mandl_t e5, mandl_t e4, mandl_t e3, mandl_t e2, mandl_t e1, mandl_t e0)
{
    dst[0] = e0; dst[1] = e1;
    dst[2] = e2; dst[3] = e3;
    dst[4] = e4; dst[5] = e5;
    dst[6] = e6; dst[7] = e7;
}

inline int mm256_movemask_ps (int* a)
{
    int mask = 0;
    for (int i = 0; i < SIZE_ARR; i++) mask |= (a[i] << i);

    return mask;
}

volatile mandl_t v_arr[1] = {};

const __m256 r_2_max_arr = _mm256_set1_ps (r_2_max);

double RunMandelbrot_v1 (sf::Image* image, struct Params_t* cond, bool GraphicsFlag)
{
    struct timespec start, end;
    clock_gettime (CLOCK_MONOTONIC, &start);

    for (unsigned int iy = 0; iy < SIZE_Y; iy++)
    {
        mandl_t x_0 =  (                         - (mandl_t) SIZE_X*cond->scale/2) * cond->dx + cond->xc;
        mandl_t y_0 =  ((mandl_t) iy*cond->scale - (mandl_t) SIZE_Y*cond->scale/2) * cond->dy + cond->yc;

        for (unsigned int ix = 0; ix < SIZE_X; ix++, x_0 += cond->dx*cond->scale)
        {
            mandl_t x = x_0;
            mandl_t y = y_0;

            int N = 0;

            for ( ; N < N_max; N++)
            {
                mandl_t x_2 = x * x;
                mandl_t y_2 = y * y;
                mandl_t x_y = x * y;

                mandl_t r_2 = x_2 + y_2;

                if (r_2 >= r_2_max)
                    break;

                x = x_2 - y_2 + x_0;
                y = x_y + x_y + y_0;
            }
#ifndef MEASURE
            if (GraphicsFlag)
            {
                sf::Color color;
                color = sf::Color::Black;

                if (N == N_max)
                {
                    color.r = 0;
                    color.g = 0;
                    color.b = 0;
                }
                else
                {
                    mandl_t t = (mandl_t) N / (mandl_t) N_max;
                    int r = (int) (255*2.5 * t) + 7;
                    int g = (int) (255*2.5 * t) + 7;
                    int b = 0;
                    color.r = (sf::Uint8) (r > 255 ? 255 : r);
                    color.g = (sf::Uint8) (g > 255 ? 255 : g);
                    color.b = (sf::Uint8) b;
                }
                image->setPixel (ix, iy, color);
            }
#else
            v_arr[0] = x;
            v_arr[0] = y;
            v_arr[0] = (mandl_t)N;
#endif
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

    for (unsigned int iy = 0; iy < SIZE_Y; iy++)
    {
        mandl_t x_0 =  (                         - (mandl_t) SIZE_X*cond->scale/2) * cond->dx + cond->xc;
        mandl_t y_0 =  ((mandl_t) iy*cond->scale - (mandl_t) SIZE_Y*cond->scale/2) * cond->dy + cond->yc;

        for (unsigned int ix = 0; ix < SIZE_X; ix += SIZE_ARR, x_0 += cond->dx*cond->scale*SIZE_ARR)
        {
            mandl_t x_0_arr[SIZE_ARR] = {};
            mandl_t y_0_arr[SIZE_ARR] = {};
            mandl_t x[SIZE_ARR] = {};
            mandl_t y[SIZE_ARR] = {};

            mm256_set_ps (x_0_arr, x_0 + 7*cond->dx*cond->scale, x_0 + 6*cond->dx*cond->scale, x_0 + 5*cond->dx*cond->scale,
                                   x_0 + 4*cond->dx*cond->scale, x_0 + 3*cond->dx*cond->scale, x_0 + 2*cond->dx*cond->scale,
                                   x_0 + 1*cond->dx*cond->scale, x_0);
            mm256_set1_ps (y_0_arr, y_0);

            mm_cpy_ps (x, x_0_arr);
            mm_cpy_ps (y, y_0_arr);

            int N[SIZE_ARR] = {};

            for (int n = 0; n < N_max; n++)
            {
                mandl_t x_2[SIZE_ARR] = {}; mm256_mul_ps (x_2, x, x);
                mandl_t y_2[SIZE_ARR] = {}; mm256_mul_ps (y_2, y, y);
                mandl_t x_y[SIZE_ARR] = {}; mm256_mul_ps (x_y, x, y);

                mandl_t r_2[SIZE_ARR] = {}; mm256_add_ps (r_2, x_2, y_2);

                int compare[SIZE_ARR] = {}; mm256_cmple_ps (compare, r_2, r_2_max);

                int mask = mm256_movemask_ps (compare);
                if (!mask)
                    break;

                mm256_add_epi32 (N, N, compare);

                mm256_sub_ps (x, x_2, y_2);
                mm256_add_ps (x, x, x_0_arr);

                mm256_add_ps (y, x_y, x_y);
                mm256_add_ps (y, y, y_0_arr);
            }

#ifndef MEASURE
            if (GraphicsFlag)
                SetPixels (image, ix, iy, N);
#else
            v_arr[0] = *N;
            v_arr[0] = *x;
            v_arr[0] = *y;
#endif
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
        mandl_t x_0 =  (                         - (mandl_t) SIZE_X*cond->scale/2) * cond->dx + cond->xc;
        mandl_t y_0 =  ((mandl_t) iy*cond->scale - (mandl_t) SIZE_Y*cond->scale/2) * cond->dy + cond->yc;

        for (unsigned int ix = 0; ix < SIZE_X; ix += SIZE_ARR, x_0 += cond->dx*cond->scale*SIZE_ARR)
        {
            __m256 x_0_array = _mm256_add_ps ( _mm256_set1_ps (x_0),
                                               _mm256_mul_ps  ( _mm256_set_ps (7.f, 6.f, 5.f, 4.f, 3.f, 2.f, 1.f, 0.f),
                                               _mm256_set1_ps (cond->dx*cond->scale) ) );
            __m256 y_0_array = _mm256_set1_ps (y_0);
            __m256 x = _mm256_movehdup_ps (x_0_array);
            __m256 y = _mm256_movehdup_ps (y_0_array);

            __m256i N  = _mm256_setzero_si256 ();

            for (int n = 0; n < N_max; n++)
            {
                __m256 x_2 = _mm256_mul_ps (x, x);
                __m256 y_2 = _mm256_mul_ps (y, y);
                __m256 x_y = _mm256_mul_ps (x, y);

                __m256 r_2 = _mm256_add_ps (x_2, y_2);

                __m256 compare = _mm256_cmp_ps (r_2, r_2_max_arr, _CMP_LE_OQ);

                int mask = _mm256_movemask_ps (compare);
                if (!mask)
                    break;

                N = _mm256_add_epi32 (N, _mm256_castps_si256 (compare));
                x = _mm256_add_ps (_mm256_sub_ps (x_2, y_2), x_0_array);
                y = _mm256_add_ps (_mm256_add_ps (x_y, x_y), y_0_array);
            }

#ifndef MEASURE
            if (GraphicsFlag)
            {
                 int N_arr[8] = {};
                 _mm256_storeu_si256 ( (__m256i*) N_arr, N);

                SetPixels (image, ix, iy, N_arr);
            }
#endif
        }
    }

    clock_gettime (CLOCK_MONOTONIC, &end);


    return (double) (end.tv_sec  - start.tv_sec) +
           (double) (end.tv_nsec - start.tv_nsec) / 1000000000.0;
}

// TODO - гистрограмма
