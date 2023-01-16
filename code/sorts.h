#pragma once
#include <stdio.h>
#include <SDL2/SDL.h>
void bubble_sort(SDL_Rect *array, int size);
void insertion_sort(SDL_Rect *array, int size);
void selection_sort(SDL_Rect *array, int size);
void quick_sort(SDL_Rect *array, int size);
void merge_sort_s(SDL_Rect *array, int size);
void heap_sort(SDL_Rect *array, int size);
int globals_exchg(int *iterators);
