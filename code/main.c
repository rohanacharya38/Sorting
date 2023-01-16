const char *display_str="0.bubble_sort\n1.insertion_sort\n2.selection_sort\n3.quick_sort\n4.merge_sort_s\n5.heap_sort\n";

#include "threading.h"
#include "sorts.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>

//defines
#define SCREEN_WIDTH 1350
#define SCREEN_HEIGHT 700
#define RECT_COUNT 190
#define sWIDTH 6
#define PADDING 1

//emum for bool
typedef enum
{
    false,
    true
} bool;

//global variables
SDL_Rect sRects[RECT_COUNT];
//declaring a iterator to highlight the rect we currently are in
//we need to store 2 values which we are comparing so an array would be good
int iterator[2];
/*main idea we will take the integer number as height of rects and sort them accordingly*/
//a boolean to know weather thread finishined it work
bool thread_created = false;


void render_string(const char *, const int, SDL_Texture *, SDL_Renderer *);
SDL_Rect get_char_rect_cood(const char);
SDL_Texture *load_img(char *, SDL_Renderer *);
void array_scramble(void);
/*since srect is global so no need for arguments but for threads we need to take void * argument
most likely we will pass NULL*/
/*Looks like thrd_create function needs a function pointer of return type int32_t thrd_so */

//thread 1
//TODO:Add other sorts in other threads
int32_t switch_function(void *keycode);
int main(int argc, char **argv)
{
    //we need random numbers for height so lets seed them

    srand((unsigned int)time(NULL));
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window *window = SDL_CreateWindow("Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
    SDL_Texture *img_texture = load_img("../misc/font.png", renderer);
    const int display_text_len = strlen(display_str);
    /*We are setting up the values for all rectangels in sRects Array*/
    array_scramble();
    bool is_running = true;
    bool once = false;
    thrd_t thread;
    SDL_Event event;
    int arr[] = {0, RECT_COUNT};
    bool esc = true;
    globals_exchg(iterator);
    while (is_running)
    {
        if (once && !thread_created)
        {
            for (int i = 0; i < RECT_COUNT; i++)
            {
                SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
                SDL_RenderFillRect(renderer, &sRects[i]);
                SDL_RenderPresent(renderer);
               SDL_Delay(2);
            }
            array_scramble();
            once = false;
        }

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                is_running = false;
           /* if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
            {
                thread_created = false;
                //need sth like thread terminate here

                array_scramble();
                once = false;
                esc = true;
            }*/
            // if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_LSHIFT)
            // {
            //     thread_created = false;
            //     once = false;
            //     esc = true;
            // }
            if (!thread_created && !once)
            {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderClear(renderer);
                render_string(display_str, display_text_len, img_texture, renderer);
                switch (event.type)
                {
                case SDL_KEYDOWN:
                SDL_Scancode key=event.key.keysym.scancode;
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
                        thread_created=true;
                        esc=false;
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

        //rendering one by one is a pain in the ass so we are goiing this way
        if (thread_created)
        {
            once = true;
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderFillRects(renderer, sRects, RECT_COUNT);
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_RenderFillRect(renderer, &sRects[iterator[0]]);
            SDL_RenderFillRect(renderer, &sRects[iterator[1]]);
            SDL_Delay(2);
        }

        SDL_RenderPresent(renderer);
    }

    return 0;
}
void array_scramble(void)
{
    sRects[0].x = PADDING;
    sRects[0].w = sWIDTH;
    sRects[0].y = SCREEN_HEIGHT - 20;
    sRects[0].h = -((rand() % (SCREEN_HEIGHT - 10)) + 10);
    for (int i = 1; i < RECT_COUNT; i++)
    {

        //need same width for all rectangles so better #define them
        sRects[i].w = sWIDTH;
        sRects[i].x = sRects[i - 1].x + sWIDTH + PADDING;
        sRects[i].y = SCREEN_HEIGHT - 20;
        sRects[i].h = -((rand() % (SCREEN_HEIGHT - 4)) + 4);
    }
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

SDL_Rect get_char_rect_cood(const char ch)
{
    SDL_Rect tmp = {0, 0, 14, 18};
    tmp.x = ((ch - 32) % 18) * tmp.w;
    tmp.y = ((ch - 32) / 18) * tmp.h;
    return tmp;
}
void render_string(const char *str, const int length, SDL_Texture *img_texture, SDL_Renderer *renderer)
{
    SDL_Rect char_rect;
    SDL_Rect d_rect = {.w = 14, .h = 18, .x = 0, .y = 0};
    for (int i = 0; i < length; i++)
    {
        if (str[i] == '\n')
        {
            d_rect.y += d_rect.h;
            d_rect.x = 0;
            continue;
        }
        char_rect = get_char_rect_cood(str[i]);
        SDL_RenderCopy(renderer, img_texture, &char_rect, &d_rect);
        d_rect.x += d_rect.w;
    }
}
int32_t switch_function(void *keycode)
{
SDL_Scancode key=*((SDL_Scancode *)keycode);
switch (key)

{
case SDL_SCANCODE_0:
case SDL_SCANCODE_KP_0:
bubble_sort(sRects, RECT_COUNT);
break;
case SDL_SCANCODE_1:
case SDL_SCANCODE_KP_1:
insertion_sort(sRects, RECT_COUNT);
break;
case SDL_SCANCODE_2:
case SDL_SCANCODE_KP_2:
selection_sort(sRects, RECT_COUNT);
break;
case SDL_SCANCODE_3:
case SDL_SCANCODE_KP_3:
quick_sort(sRects, RECT_COUNT);
break;
case SDL_SCANCODE_4:
case SDL_SCANCODE_KP_4:
merge_sort_s(sRects, RECT_COUNT);
break;
case SDL_SCANCODE_5:
case SDL_SCANCODE_KP_5:
heap_sort(sRects, RECT_COUNT);
break;
default:
break;
}
thread_created=false;
return 0;
}