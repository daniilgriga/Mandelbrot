#ifndef GRAPHICS_H
#define GRAPHICS_H

#define SIZE_X 800
#define SIZE_Y 800

int GraphicsPart (struct Params_t* cond, int* version);

void SetPixels (sf::Image* image, unsigned int ix, unsigned int iy, int* N);

#endif // GRAPHICS_H
