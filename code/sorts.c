#include "sorts.h"
#include <SDL2/SDL.h>
int *iterators;
int globals_exchg(int *iterator)
{
    iterators = iterator;
    return 0;
}
void bubble_sort(SDL_FRect *array, int size)
{
    int i, j, temp;
    for (i = 0; i < size; i++)
    {
        for (j = 0; j < size - i - 1; j++)
        {
            if (array[j].h > array[j + 1].h)
            {
                temp = array[j].h;
                array[j].h = array[j + 1].h;
                array[j + 1].h = temp;
            }
            iterators[0] = j;
            iterators[1] = j + 1;
            SDL_Delay(2);
        }
    }
}

void insertion_sort(SDL_FRect *array, int size)
{
    int i, j, temp;
    for (i = 1; i < size; i++)
    {
        temp = array[i].h;
        j = i - 1;
        while (j >= 0 && array[j].h > temp)
        {
            array[j + 1].h = array[j].h;
            j--;
            iterators[0] = j;
            iterators[1] = i;
            SDL_Delay(2);
        }

        array[j + 1].h = temp;
    }
}

void selection_sort(SDL_FRect *array, int size)
{
    int i, j, min, temp;
    for (i = 0; i < size - 1; i++)
    {
        min = i;
        for (j = i + 1; j < size; j++)
        {
            if (array[j].h < array[min].h)
            {
                min = j;
            }
            iterators[0] = min;
            iterators[1] = j;
            SDL_Delay(2);
        }
        temp = array[i].h;
        array[i].h = array[min].h;
        array[min].h = temp;
    }
}

void quick_sort(SDL_FRect *array, int size)
{
    //set both iterators to -1
    iterators[0] = -1;
    iterators[1] = -1;

    int i, j, pivot, temp;
    if (size < 2)
    {
        return;
    }
    pivot = array[size / 2].h;
    for (i = 0, j = size - 1;; i++, j--)
    {
        while (array[i].h < pivot)
        {
            i++;
            SDL_Delay(2);
        }
        while (array[j].h > pivot)
        {
            j--;
            SDL_Delay(2);
        }
        if (i >= j)
        {
            break;
        }
        temp = array[i].h;
        array[i].h = array[j].h;
        array[j].h = temp;
        SDL_Delay(2);
    }
    SDL_Delay(2);
    quick_sort(array, i);
    quick_sort(array + i, size - i);
}

void merge(SDL_FRect *array, int left, int middle, int right)
{
    
    int i, j, k;
    int n1 = middle - left + 1;
    int n2 = right - middle;
    SDL_FRect *left_array = malloc(n1 * sizeof(SDL_FRect));
    SDL_FRect *right_array = malloc(n2 * sizeof(SDL_FRect));
    for (i = 0; i < n1; i++)
    {
        left_array[i].h = array[left + i].h;
        SDL_Delay(2);
    }
    for (j = 0; j < n2; j++)
    {
        right_array[j].h = array[middle + 1 + j].h;
        SDL_Delay(2);
    }
    i = 0;
    j = 0;
    k = left;
    while (i < n1 && j < n2)
    {
        if (left_array[i].h <= right_array[j].h)
        {
            array[k].h = left_array[i].h;
            i++;
        }
        else
        {
            array[k].h = right_array[j].h;
            j++;
        }
        k++;
        SDL_Delay(2);
    }

    while (i < n1)
    {
        array[k].h = left_array[i].h;
        i++;
        k++;

        SDL_Delay(2);
    }
    while (j < n2)
    {
        array[k].h = right_array[j].h;
        j++;
        k++;
        SDL_Delay(2);
    }
    free(left_array);
    free(right_array);
}

void merge_sort(SDL_FRect *array, int left, int right)
{
    //set both iterators to -1
    iterators[0] = -1;
    iterators[1] = -1;
    
    if (left < right)
    {
        int middle = left + (right - left) / 2;
        merge_sort(array, left, middle);
        merge_sort(array, middle + 1, right);
        merge(array, left, middle, right);
    }
    SDL_Delay(2);
}

void merge_sort_s(SDL_FRect *array, int size)
{
    merge_sort(array, 0, size);
}

void heapify(SDL_FRect *array, int size, int i)
{
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    if (left < size && array[left].h > array[largest].h)
    {
        largest = left;
    }
    if (right < size && array[right].h > array[largest].h)
    {
        largest = right;
    }
    if (largest != i)
    {
        int temp = array[i].h;
        array[i].h = array[largest].h;
        array[largest].h = temp;

        heapify(array, size, largest);
    }
    SDL_Delay(2);
}

void heap_sort(SDL_FRect *array, int size)
{
    //set both iterators to -1
    iterators[0] = -1;
    iterators[1] = -1;
    
    int i;
    for (i = size / 2 - 1; i >= 0; i--)
    {
        heapify(array, size, i);
        SDL_Delay(2);
    }
    for (i = size - 1; i >= 0; i--)
    {
        int temp = array[0].h;
        array[0].h = array[i].h;
        array[i].h = temp;
        heapify(array, i, 0);
        SDL_Delay(2);
    }
}
