![Linux](https://img.shields.io/badge/Linux-FCC624?style=for-the-badge&logo=linux&logoColor=black)
![C++](https://img.shields.io/badge/c++-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white)

# Mandelbrot

## General information

Lab work on programming from the ded32 course on optimization of calculation. As an algorithm of calculation I use the Mandelbrot set.
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
    - [Productivity measurement](#productivity)

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

We keep track of the number of steps it takes for the point to go outside the circle (```Rmax```). If this does not happen before ```N``` iterations, the loop stops.

My const values:
- ```Rmax``` = 10
- ```N``` = 256

### Modes

- **Image output**. Used to verify the algorithm and calculate the FPS
- **Timing**. Used to measure the running time of n iterations of the Mandelbrot algorithm
