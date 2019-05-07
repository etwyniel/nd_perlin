#include <iostream>
#include "nd_vector.hh"
#include "nd_perlin.hh"
#include <SDL2/SDL.h>

int WIDTH = 500;
int HEIGHT = 300;
static double v = 0, w = 0;

template <size_t N>
void update(SDL_Window* window, const Perlin<N>& p)
{
    auto* surface = SDL_GetWindowSurface(window);
    SDL_LockSurface(surface);

    const size_t bpp = surface->format->BytesPerPixel;
    unsigned char* pixels = (unsigned char*)surface->pixels;
    int pitch = surface->pitch;
    int width = surface->w;
    int height = surface->h;
#pragma omp parallel for
    for (size_t y = 0; y < height; y++)
    {
        for (size_t x = 0; x < width; x++)
        {
            Vector<4> pos;
            pos[0] = ((double)x) / 30;
            pos[1] = ((double)y) / 30;
            pos[2] = v / 2;
            pos[3] = w / 2;

            double val = p.value_at(pos);
            if (val < 0)
                val = 0;
            else if (val > 1)
                val = 1;
            unsigned char pixval = val * 255;
            unsigned char* pixel = &pixels[y * pitch + x * bpp];
            for (size_t i = 0; i < bpp; i++)
                pixel[i] = pixval;
        }
    }

    SDL_UnlockSurface(surface);
    SDL_UpdateWindowSurface(window);
}

void render()
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    auto* window = SDL_CreateWindow("perlin noise",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            WIDTH, HEIGHT, SDL_WINDOW_BORDERLESS | SDL_WINDOW_RESIZABLE);
    Perlin<4> p;
    bool wplus = false;
    bool wminus = false;
    bool vplus = false;
    bool vminus = false;
    size_t counter = 0;
    size_t time_sum = 0;
    while (1)
    {
        SDL_GetWindowSize(window, &HEIGHT, &WIDTH);
        SDL_Event input;
        while (SDL_PollEvent(&input))
        {
            if (input.type == SDL_QUIT)
                return;
            else
            {
                bool down = input.type == SDL_KEYDOWN;
                SDL_Scancode sc = input.key.keysym.scancode;
                if (sc == SDL_SCANCODE_ESCAPE)
                    return;
                else if (sc == SDL_SCANCODE_UP)
                    vplus = down;
                else if (sc == SDL_SCANCODE_DOWN)
                    vminus = down;
                else if (sc == SDL_SCANCODE_LEFT)
                    wplus = down;
                else if (sc == SDL_SCANCODE_RIGHT)
                    wminus = down;
            }
        }
        if (wplus)
            w += .1;
        if (wminus)
            w -= .1;
        if (vplus)
            v += .1;
        if (vminus)
            v -= .1;
        int start = SDL_GetTicks();
        update(window, p);
        int time = SDL_GetTicks() - start;
        if (time < 0)
            continue;
        time_sum += time;
        if (counter++ == 20)
        {
            counter = 0;
            printf("Average render time: %lums\n", time_sum / 20);
            time_sum = 0;
        }
        /* printf("Frame took %dms to render\n", time); */
        const double framerate = 30;
        int sleepTime = (1000 / framerate) - time;
        printf("Sleeping for %dms\n", sleepTime);
        if (sleepTime > 0)
            SDL_Delay(sleepTime);
    }
}

int main(void)
{
    render();
    return 0;
}
