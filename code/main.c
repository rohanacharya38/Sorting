
#include "headers/threading.h"
#include "headers/sorts.h"
#include "headers/font.h"
#define STB_IMAGE_IMPLEMENTATION
#include "headers/stb_image.h"
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>

const char *display_str = "0. Bubble Sort\n1. Insertion Sort\n2. Selection Sort\n3. Quick Sort\n4. Merge Sort\n5. Heap Sort";
SDL_Texture *load_img(char *image_path, SDL_Renderer *renderer);
void randomize_array(SDL_Renderer *renderer, STBTTF_Font *font);
// defines
#define SCREEN_WIDTH 900
#define SCREEN_HEIGHT 700
#define RECT_COUNT 175
const float sWIDTH = (SCREEN_WIDTH) / RECT_COUNT;
// global variables
SDL_FRect sRects[RECT_COUNT];
// declaring a iterator to highlight the rect we currently are in
// we need to store 2 values which we are comparing so an array would be good
int iterator[2];
/*main idea we will take the integer number as height of rects and sort them accordingly*/
// a boolean to know weather thread finishined it work
bool thread_created = false;

/*since srect is global so no need for arguments but for threads we need to take void * argument
most likely we will pass NULL*/
/*Looks like thrd_create function needs a function pointer of return type int32_t thrd_so */

// thread 1
// TODO:Add other sorts in other threads
char renderSortText[128];
char finalSortText[256];

int32_t switch_function(void *keycode);
SDL_Scancode key;
uint32_t start_time = 0;
uint32_t elapsed_time = 0;
uint32_t delay_count = 0;
int main(int argc, char **argv)
{
    // we need random numbers for height so lets seed them

    srand((unsigned int)time(NULL));
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window *window = SDL_CreateWindow("Sorting Visualizer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_Texture *bg_texture = load_img("../misc/bg.png", renderer);
    STBTTF_Font *font = STBTTF_OpenFont(renderer, "../misc/font.ttf", 19);
    if (font == NULL)
    {
        printf("Error: couldn't open font file");
        return -1;
    }
    /*We are setting up the values for all rectangels in sRects Array*/
    // linearly vary height of rects
    float max = 0;
    float heightDiff = (700.0f-60.0f)/ RECT_COUNT;
    for (int i = 0; i < RECT_COUNT; i++)
    {
        sRects[i].x = i * sWIDTH + 20;
        sRects[i].y = SCREEN_HEIGHT - 20;
        sRects[i].w = sWIDTH;
        sRects[i].h = -heightDiff * i;
        if (sRects[i].h < max)
            max = sRects[i].h;
    }
    bool is_running = true;
    bool once = false;
    thrd_t thread;
    SDL_Event event;
    int arr[] = {0, RECT_COUNT};
    bool esc = true;
    globals_exchg(iterator, &thread_created, &delay_count);
    while (is_running)
    {
        if (thread_created)
        {
            elapsed_time = SDL_GetTicks() - start_time - delay_count;
        }
        if (once && !thread_created)
        {
            for (int i = 0; i < RECT_COUNT; i++)
            {
                SDL_SetRenderDrawColor(renderer, 144, 238, 144, 255);
                SDL_RenderFillRectF(renderer, &sRects[i]);
                SDL_RenderPresent(renderer);
                SDL_Delay(2);
            }
            SDL_Delay(50);
            once = false;
        }

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                is_running = false;
            if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
            {
                thread_created = false;
                // need sth like thread terminate here
                thrd_terminate(thread);
                once = false;
                esc = true;
            }
            if (!thread_created && !once)
            {
                SDL_RenderCopy(renderer, bg_texture, NULL, NULL);
                switch (event.type)
                {
                case SDL_KEYDOWN:
                    key = event.key.keysym.scancode;
                    switch (key)
                    {
                    case SDL_SCANCODE_0:
                    case SDL_SCANCODE_1:
                    case SDL_SCANCODE_2:
                    case SDL_SCANCODE_3:
                    case SDL_SCANCODE_4:
                    case SDL_SCANCODE_5:
                    case SDL_SCANCODE_6:
                    case SDL_SCANCODE_7:
                    case SDL_SCANCODE_8:
                    case SDL_SCANCODE_9:
                    case SDL_SCANCODE_KP_0:
                    case SDL_SCANCODE_KP_1:
                    case SDL_SCANCODE_KP_2:
                    case SDL_SCANCODE_KP_3:
                    case SDL_SCANCODE_KP_4:
                    case SDL_SCANCODE_KP_5:
                    case SDL_SCANCODE_KP_6:
                    case SDL_SCANCODE_KP_7:
                    case SDL_SCANCODE_KP_8:
                    case SDL_SCANCODE_KP_9:
                        thread_created = true;
                        esc = false;
                        randomize_array(renderer, font);
                        start_time = SDL_GetTicks();
                        thrd_create(&thread, switch_function, (void *)&key);
                        break;
                    default:
                        break;
                    }
                    break;
                default:
                    break;
                }
            }
        }

        // rendering one by one is a pain so we are goiing this way
        if (thread_created)
        {
            once = true;
            SDL_SetRenderDrawColor(renderer, 0, 35, 149, 255);
            SDL_RenderClear(renderer);
            SDL_SetRenderDrawColor(renderer, 144, 238, 144, 255);
            sprintf(finalSortText, "%s,Sorting Time: %f", renderSortText, (float)elapsed_time / 10000);
            STBTTF_RenderText(renderer, font, 250, 19, finalSortText);
            SDL_SetRenderDrawColor(renderer, 93, 173, 226, 255);
            SDL_RenderFillRectsF(renderer, sRects, RECT_COUNT);
            if (iterator[0] != -1 && iterator[1] != -1)
            {
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                SDL_RenderFillRectF(renderer, &sRects[iterator[0]]);
                SDL_RenderFillRectF(renderer, &sRects[iterator[1]]);
            }
            SDL_Delay(2);
        }

        SDL_RenderPresent(renderer);
    }

    return 0;
}

void randomize_array(SDL_Renderer *renderer, STBTTF_Font *font)
{
    for (int i = 0; i < RECT_COUNT; i++)
    {
        int j = rand() % RECT_COUNT;
        float temp = sRects[i].h;
        sRects[i].h = sRects[j].h;
        sRects[j].h = temp;
        SDL_SetRenderDrawColor(renderer, 0, 35, 149, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 144, 238, 144, 255);
        STBTTF_RenderText(renderer, font, 600, 25, "Randomizing ...");
        SDL_SetRenderDrawColor(renderer, 93, 173, 226, 255);
        SDL_RenderFillRectsF(renderer, sRects, RECT_COUNT);
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRectF(renderer, &sRects[i]);
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_RenderFillRectF(renderer, &sRects[j]);
        SDL_RenderPresent(renderer);
        SDL_Delay(3);
    }
}

int32_t switch_function(void *keycode)
{
    delay_count = 0;
    SDL_Scancode *key = (SDL_Scancode *)keycode;
    switch (*key)
    {
    case SDL_SCANCODE_KP_1:
    case SDL_SCANCODE_1:
        strcpy(renderSortText, "Heap Sort,Rects Count:175");
        heap_sort(sRects, RECT_COUNT);
        break;
    case SDL_SCANCODE_KP_2:
    case SDL_SCANCODE_2:
        strcpy(renderSortText, "Quick Sort,Rects Count:175");
        quick_sort(sRects, RECT_COUNT);
        break;
    case SDL_SCANCODE_KP_3:
    case SDL_SCANCODE_3:
        strcpy(renderSortText, "Merge Sort,Rects Count:175");
        merge_sort_s(sRects, RECT_COUNT);
        break;
    case SDL_SCANCODE_KP_4:
    case SDL_SCANCODE_4:
        strcpy(renderSortText, "Bubble Sort,Rects Count:175");
        bubble_sort(sRects, RECT_COUNT);
        break;
    case SDL_SCANCODE_KP_5:
    case SDL_SCANCODE_5:
        strcpy(renderSortText, "Insertion Sort,Rects Count:175");
        insertion_sort(sRects, RECT_COUNT);
        break;
    case SDL_SCANCODE_KP_6:
    case SDL_SCANCODE_6:
        strcpy(renderSortText, "Selection Sort,Rects Count:175");
        selection_sort(sRects, RECT_COUNT);
        break;
    default:
        strcpy(renderSortText, "Default");

        break;
    }
    return 0;
}
SDL_Texture *load_img(char *image_path, SDL_Renderer *renderer)
{
    int req_format = STBI_rgb_alpha;
    int width, height, orig_format;
    uint8_t *data = stbi_load(image_path, &width, &height, &orig_format, req_format);
    Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif

    int depth, pitch;
    depth = 32;
    pitch = 4 * width;

    SDL_Surface *surf = SDL_CreateRGBSurfaceFrom((void *)data, width, height, depth, pitch,
                                                 rmask, gmask, bmask, amask);

    SDL_Texture *ret = SDL_CreateTextureFromSurface(renderer, surf);
    return ret;
}