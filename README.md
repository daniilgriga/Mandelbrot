![Linux](https://img.shields.io/badge/Linux-FCC624?style=for-the-badge&logo=linux&logoColor=black)
![C++](https://img.shields.io/badge/c++-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![Bash Script](https://img.shields.io/badge/bash_script-%23121011.svg?style=for-the-badge&logo=gnu-bash&logoColor=white)

# Mandelbrot

![fractal](img/fractal.png)

## General information

Lab work on programming in the the [ded32](https://github.com/ded32) course on optimization of calculation. As an algorithm of calculation I use the Mandelbrot set.
I have to measure the time spent on calculation and try to reduce it with my optimizations.

To debug the algorithm I draw the Mandelbrot set with SFML.


## Context

- [Program](#program)
    - [Algorithm](#algorithm)
    - [Modes](#modes)
    - [Graphics](#graphics)
- [Optimization](#optimization)
    - [btop++](#btop)
    - [My hardware](#hardware)
    - [Performance measurement](#productivity)
- [Results](#results)

## Program

### Algorithm

The Mandelbrot set uses x and y axes, where ```c = x + yi```. Start with ```z = 0```. Iterate using ```z = z² + c```. In x-y terms:
```
z_x = z_x² - z_y² + x
z_y = 2 * z_xy + y
```
where:
- ```x``` -----  X coordinate of the calculated pixel
- ```y``` -----  Y coordinate of the calculated pixel
- ```z_x``` ----- real part
- ```z_y``` ----- imaginary part
- ```z_xy``` ----- ```z_x * z_y```

We keep track of the number of steps it takes for the point to go outside the circle. If this does not happen before N iterations, the loop stops.

My const values:
- Size of graphics window = `800*800`
- Radius of circle = `10`
- N = `256`

There are three implementations of this:

**1.** Simple - *pixel-by-pixel calculation*

<details>
<summary>Click to expand/collapse</summary>

```C
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

```
</details>

**2.** Vector - *vectorization*, help compiler to optimize (consider arrays of 8 pixels)

<details>
<summary>Click to expand/collapse</summary>

``` C
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
```
</details>

**3.** AVX - *vectorization with `AVX`*, using SIMD intrinsics

<details>
<summary>Click to expand/collapse</summary>

```C
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
            __m256 x = x_0_array;
            __m256 y = y_0_array;

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
```
</details>

### Modes

- **Image output**. Used to verify the algorithm and calculate the FPS
- **Timing**. Used to measure the running time of n iterations of the Mandelbrot algorithm

### Graphics

The color is compiled by this algorithm for each of the rgb colors:

|                color.r                |                color.g               |      color.b      |
|---------------------------------------|--------------------------------------|-------------------|
|     ```r = (255*2.5 * t) + 7;```      |     ```g = (255*2.5 * t) + 7;```     |         0         |
| ```color.r = (r > 255 ? 255 : r);```  | ```color.g = (g > 255 ? 255 : g);``` | ```color.b = b``` |

In this way, the black areas from which the point failed to escape will be visible

## Optimization

### btop++

It is a modern, colorful command-line resource monitor for Linux, macOS, and BSD systems. Written in C++ as an evolution of *bashtop* and *bpytop*, it provides real-time stats on CPU, memory, disks, network, and processes.

I will use btop to track the load on the processor cores and its temperature.

### My hardware

- **Processor:** ```AMD Ryzen 5 4500U 2.3 GHz (4 GHz in Turbo)```
- **OS:** ```Ubuntu 24.04.2 LTS```

Stabilized processor parameters during measurements:
- **Temperature:** ```76°C```
- **Frequency:** ```1.4 ± 0.1 GHz```

![btop](img/btop.png)

### Performance measurement

We will measure 20 times for each version of the algorithm, running the algorithm itself 300 times.

I used the following compilation parameters:
- `g++`, with `-O3`
- `g++`, with `-O2`
- `clang++`, with `-O3`
- `clang++`, with `-O2`



The full table of measurements can be found [here](https://github.com/daniilgriga/Mandelbrot/blob/main/data.md)

Based on these results, I constructed a histogram:

![histo](img/histo.png)

## Results

The third version of the function, using intrinsics for the Mandelbrot set, runs **3.3** times faster than the first (simple) with `g++` and **5.7** times faster with `clang++`. This is due to efficient SIMD instructions in the third function, processing multiple pixels in parallel. `clang++` achieves greater speedup through aggressive optimizations: better code generation for intrinsics, efficient SIMD register management, and reduced overhead for branching and memory access. `g++` optimizes intrinsics less effectively, leading to smaller speedup.
