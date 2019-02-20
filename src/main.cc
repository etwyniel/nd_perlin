#include <iostream>
#include "nd_vector.hh"
#include "nd_perlin.hh"
#include <SDL2/SDL.h>

const size_t WIDTH = 400;
const size_t HEIGHT = 400;
static double scale = 80;
static double timescale = 60;

static double counter = 0;

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
            v[2] = counter;
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
    counter += 1 / timescale;

    SDL_UpdateWindowSurface(window);
}

void render()
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    auto* window = SDL_CreateWindow("perlin noise",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            WIDTH, HEIGHT, SDL_WINDOW_BORDERLESS);
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
                    timescale *= 1.1;
                else if (sc == SDL_SCANCODE_RIGHT && timescale > 0.0000000001)
                    timescale *= .9;
            }
        }
        int start = SDL_GetTicks();
        update(window, p);
        int time = SDL_GetTicks() - start;
        if (time < 0)
            continue;
        double rate = 30 - timescale;
        rate = rate < 15 ? 15 : rate;
        rate = rate > 30 ? 30 : rate;
        int sleepTime = (1000 / rate) - time;
        if (sleepTime > 0)
            SDL_Delay(sleepTime);
    }
}

int main(void)
{
    render();
    return 0;
}
