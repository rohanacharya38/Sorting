//inlcudes
#include "threading.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <SDL.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
//
//

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

//the string which we need to render

const char *display_str =
    "0.GG Sort\n1.Bubble Sort\n2.Insertion Sort\n3.Radix Sort\n4.Merge Sort\n";

//left and right arrays for mergesort
int L[RECT_COUNT], R[RECT_COUNT];
bool merge_finished = false;
//main thread
//render
//thread 0
int32_t gg_sort(void *);
int32_t bubble_sort(void *);
int32_t insertion_sort(void *);
int32_t mqsort(void *);
int32_t radixsort(void *);
int32_t merge_sort(void *arr);
// int32_t wave_sort(void *);

void countSort(int exp);
int getMax(void);
void merge(int l, int m, int r);

void render_string(const char *, const int, SDL_Texture *, SDL_Renderer *);
SDL_Rect get_char_rect_cood(const char);
SDL_Texture *load_img(char *, SDL_Renderer *);
void array_scramble(void);
/*since srect is global so no need for arguments but for threads we need to take void * argument
most likely we will pass NULL*/
/*Looks like thrd_create function needs a function pointer of return type int32_t so */

//thread 1
//TODO:Add other sorts in other threads
int main(int argc, char **argv)
{
    //we need random numbers for height so lets seed them

    srand((unsigned int)time(NULL));
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window *window = SDL_CreateWindow("Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
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
    while (is_running)
    {
        if (once && !thread_created)
        {
            for (int i = 0; i < RECT_COUNT; i++)
            {
                SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
                SDL_RenderFillRect(renderer, &sRects[i]);
                SDL_RenderPresent(renderer);
                SDL_Delay(5);
            }
            array_scramble();
            once = false;
        }

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                is_running = false;
            if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
            {
                thread_created = false;
                array_scramble();
                once = false;
                esc = true;
            }
            if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_LSHIFT)
            {
                thread_created = false;
                once = false;
                esc = true;
            }
            if (!thread_created && esc)
            {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderClear(renderer);
                render_string(display_str, display_text_len, img_texture, renderer);
                switch (event.type)
                {
                case SDL_KEYDOWN:
                    switch (event.key.keysym.scancode)
                    {
                    case SDL_SCANCODE_ESCAPE:
                        break;
                    case SDL_SCANCODE_0:
                    case SDL_SCANCODE_KP_0:
                        thread_created = true;
                        esc=false;
                        thrd_create(&thread, gg_sort, NULL);
                        break;
                    case SDL_SCANCODE_1:
                    case SDL_SCANCODE_KP_1:
                        esc=false;
                        thread_created = true;
                        thrd_create(&thread, bubble_sort, NULL);
                        break;
                    case SDL_SCANCODE_2:
                    case SDL_SCANCODE_KP_2:
                        esc=false;
                        thread_created = true;
                        thrd_create(&thread, insertion_sort, NULL);
                        break;
                    case SDL_SCANCODE_3:
                    case SDL_SCANCODE_KP_3:
                        esc=false;
                        thread_created = true;
                        thrd_create(&thread, radixsort, NULL);
                        break;
                    case SDL_SCANCODE_4:
                    case SDL_SCANCODE_KP_4:
                        esc=false;
                        thread_created = true;
                        thrd_create(&thread, merge_sort, arr);
                        break;
                    case SDL_SCANCODE_5:
                    case SDL_SCANCODE_KP_5:
                        // esc=false;
                        // thread_created = true;
                        // thrd_create(&thread, wave_sort, NULL);
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
            if (merge_finished)
                thread_created = false;
            merge_finished = false;
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

int32_t gg_sort(void *noUse)
{
    printf("Thread1 Created");
    uint32_t temp = 0;

    for (int i = 0; i < RECT_COUNT && thread_created; i++)
    {
        for (int j = i; j < RECT_COUNT && thread_created; j++)
        {
            if (sRects[j].h > sRects[i].h)
            {
                //swapping height nthng much
                //since every property is similar except height so only x switch is enough imo
                temp = sRects[j].h;
                sRects[j].h = sRects[i].h;
                sRects[i].h = temp;
                iterator[0] = i;
                iterator[1] = j;
                //10 ms delay for display
                SDL_Delay(10);
            }
        }
    }
    printf("Thread1 completed");
    thread_created = false;
    return 0;
}

int32_t insertion_sort(void *NoUse)
{
    int i, key, j;
    for (i = 1; i < RECT_COUNT && thread_created; i++)
    {
        key = sRects[i].h;
        j = i - 1;

        /* Move elements of sRects[0..i-1].h, that are
          greater than key, to one position ahead
          of their current position */
        while (j >= 0 && sRects[j].h < key && thread_created)
        {
            sRects[j + 1].h = sRects[j].h;
            j = j - 1;
            iterator[0] = i;
            iterator[1] = j;
            SDL_Delay(2);
        }
        sRects[j + 1].h = key;
    }
    thread_created = false;
    return 0;
}

int32_t bubble_sort(void *NotUsed)
{
    int32_t temp = 0;
    for (int i = 0; i < RECT_COUNT - 1 && thread_created; i++)
    {
        for (int j = 0; j < RECT_COUNT - i - 1 && thread_created; j++)
        {
            if (sRects[j].h < sRects[j + 1].h)
            {
                //swapping height nthng much
                //since every property is similar except height so only x switch is enough imo
                temp = sRects[j].h;
                sRects[j].h = sRects[j + 1].h;
                sRects[j + 1].h = temp;
                iterator[0] = i;
                iterator[1] = j;
            }

            //2 ms delay for display
            SDL_Delay(2);
        }
    }
    thread_created = false;

    return 0;
}

int getMax()
{
    int mx = -sRects[0].h;
    for (int i = 1; i < RECT_COUNT && thread_created; i++)
        if (-sRects[i].h > mx)
            mx = -sRects[i].h;
    return mx;
}

void countSort(int exp)
{
    int output[RECT_COUNT];
    int i, count[10] = {0};

    for (i = 0; i < RECT_COUNT && thread_created; i++)
    {
        count[(-sRects[i].h / exp) % 10]++;
    }

    for (i = 1; i < 10 && thread_created; i++)
    {
        count[i] += count[i - 1];
    }
    for (i = RECT_COUNT - 1; i >= 0 && thread_created; i--)
    {
        output[count[(-sRects[i].h / exp) % 10] - 1] = -sRects[i].h;
        count[(-sRects[i].h / exp) % 10]--;
    }

    for (i = 0; i < RECT_COUNT && thread_created; i++)
    {
        sRects[i].h = -output[i];
        SDL_Delay(5);
    }
}

// The main function to that sorts sRects[].h of sizeRECT_COUNT using
// Radix Sort
int32_t radixsort(void *NotUsed)
{
    thread_created = true;
    int m = getMax();

    for (int exp = 1; m / exp > 0 && thread_created; exp *= 10)
    {
        countSort(exp);
    }
    merge_finished = true;
    return 0;
}
void merge(int l, int m, int r)
{

    // iterator[0] = l;
    // iterator[1] = r;
    int n1 = m - l + 1, n2 = r - m;
    int i, j, k = l;
    for (i = 0; i < n1; i++)
    {
        L[i] = sRects[l + i].h;
        SDL_Delay(5);
    }
    for (j = 0; j < n2; j++)
    {
        R[j] = sRects[m + j + 1].h;
        SDL_Delay(5);
    }
    i = 0;
    j = 0;
    while (i < n1 && j < n2)
    {
        if (L[i] >= R[j])
        {
            sRects[k].h = L[i];
            i++;
        }
        else
        {
            sRects[k].h = R[j];
            j++;
        }

        k++;
    }
    while (j < n2)
    {
        sRects[k].h = R[j];
        j++;
        k++;
    }
    while (i < n1)
    {
        sRects[k].h = L[i];
        i++;
        k++;
    }
    if (n1 + n2 >= RECT_COUNT)
        merge_finished = true;
}
int32_t merge_sort(void *arr)
{
    int *nums = (int *)arr;
    int l = nums[0], r = nums[1];
    if (l < r)
    {
        int m = (l + r) / 2;
        int arr1[] = {l, m};
        merge_sort((void *)arr1);
        arr1[0] = m + 1;
        arr1[1] = r;
        merge_sort((void *)arr1);
        merge(l, m, r);
    }
    return 0;
}

// int32_t wave_sort(void *NotUsed)
// {
//     int temp;
//     for (int i = 0; i < RECT_COUNT - 1; i += 2)
//     {
//         temp=sRects[i].h;
//         sRects[i].h=sRects[i+1].h;
//         sRects[i+1].h=temp;
//     }
//     for (int i = 1; i < RECT_COUNT; i += 2)
//     {
//         if (sRects[i].h > sRects[i - 1].h)
//         {
//             temp = sRects[i].h;
//             sRects[i].h = sRects[i - 1].h;
//             sRects[i - 1].h = temp;
//             iterator[0] = i;
//             iterator[1] = i - 1;
//             SDL_Delay(5);
//         }
//         if ((sRects[i].h > sRects[i + 1].h) && (i <= RECT_COUNT - 2))
//         {
//             iterator[0] = i;
//             iterator[1] = i + 1;
//             temp = sRects[i].h;
//             sRects[i].h = sRects[i + 1].h;
//             sRects[i + 1].h = temp;
//             SDL_Delay(5);
//         }
//     }

//     return 0;
// }