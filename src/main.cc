#include <iostream>
#include "nd_vector.hh"
#include "nd_perlin.hh"
#include <SDL2/SDL.h>

const size_t WIDTH = 400;
const size_t HEIGHT = 400;
static double scale = 50;
static double timescale = 30;

static size_t counter = 0;

template <size_t N>
void update(SDL_Window* window, const Perlin<N>& p)
{
    auto* surface = SDL_GetWindowSurface(window);

    const size_t bpp = surface->format->BytesPerPixel;
    unsigned char* pixels = (unsigned char*)surface->pixels;
#pragma omp parallel for
    for (size_t y = 0; y < HEIGHT; y++)
    {
        for (size_t x = 0; x < WIDTH; x++)
        {
            Vector<3> v;
            v[0] = x / scale;
            v[1] = y / scale;
            v[2] = counter / timescale;
            double val = p.value_at(v);
            if (val < 0)
                val = 0;
            else if (val > 1)
                val = 1;
            unsigned char pixval = val * 255;
            unsigned char* pixel = &pixels[y * surface->pitch + x * bpp];
            for (size_t i = 0; i < bpp; i++)
                pixel[i] = pixval;
        }
    }

    std::cout << "step " << counter << '\n';
    counter++;

    SDL_UpdateWindowSurface(window);
}

void render()
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    auto* window = SDL_CreateWindow("perlin noise",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            WIDTH, HEIGHT, 0);
    Perlin<3> p;
    while (1)
    {
        SDL_Event input;
        while (SDL_PollEvent(&input))
        {
            if (input.type == SDL_QUIT)
                return;
            else if (input.type == SDL_KEYDOWN)
            {
                SDL_Scancode sc = input.key.keysym.scancode;
                if (sc == SDL_SCANCODE_ESCAPE)
                    return;
                else if (sc == SDL_SCANCODE_UP)
                    scale += .1;
                else if (sc == SDL_SCANCODE_DOWN)
                    scale -= .1;
                else if (sc == SDL_SCANCODE_LEFT)
                    timescale += 1;
                else if (sc == SDL_SCANCODE_RIGHT)
                    timescale -= 1;
            }
        }
        int start = SDL_GetTicks();
        update(window, p);
        int time = SDL_GetTicks() - start;
        if (time < 0)
            continue;
        int sleepTime = (1000 / 60) - time;
        if (sleepTime > 0)
            SDL_Delay(sleepTime);
    }
}

int main(void)
{
    render();
    return 0;
}
