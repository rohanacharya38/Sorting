#pragma once
#include <stdio.h>
#include <SDL2/SDL.h>
#include <string.h>
void bubble_sort(SDL_FRect *array, int size);
void insertion_sort(SDL_FRect *array, int size);
void selection_sort(SDL_FRect *array, int size);
void quick_sort(SDL_FRect *array, int size);
void merge_sort_s(SDL_FRect *array, int size);
void heap_sort(SDL_FRect *array, int size);
typedef enum
{
    false,
    true
} bool;
int globals_exchg(int *iterators, bool *is_thrd_created,uint32_t *delay);

