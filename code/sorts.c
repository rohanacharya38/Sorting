#include "headers/sorts.h"
#include <SDL2/SDL.h>
int *iterators;
bool *thrd_created;
uint32_t *delay;
uint32_t delayTime;

int globals_exchg(int *iterator, bool *is_thrd_created, uint32_t *delay_time)
{
    iterators = iterator;
    thrd_created = is_thrd_created;
    delay = delay_time;
    return 0;
}
static inline void Delay_Function(const unsigned int delay_time)
{
    delayTime = SDL_GetTicks();
    SDL_Delay(delay_time);
    *delay += (SDL_GetTicks() - delayTime - 10);
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
            Delay_Function(1);
        }
    }
    *thrd_created = false;
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
            Delay_Function(1);
        }

        array[j + 1].h = temp;
    }
    *thrd_created = false;
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
            Delay_Function(1);
        }
        temp = array[i].h;
        array[i].h = array[min].h;
        array[min].h = temp;
    }
    *thrd_created = false;
}

void q_sort(SDL_FRect *array, int size)
{
    // set both iterators to -1
    iterators[0] = -1;
    iterators[1] = -1;

    int i, j;
    float  pivot, temp;
    if (size < 2)
    {
        return;
    }
    pivot = array[size / 2].h;
    for (i = 0, j = size - 1; ;i++, j--)
    {
        while (array[i].h < pivot)
        {
            i++;
            iterators[0] = i;
            iterators[1] = size / 2;
            Delay_Function(2);
        }
        while (j>0 && array[j].h > pivot)
        {
            iterators[0] = j;
            iterators[1] = size / 2;
            j--;
            Delay_Function(2);
        }
        if (i >= j)
        {
            break;
        }
        iterators[0] = i;
        iterators[1] = j;
        temp = array[i].h;
        array[i].h = array[j].h;
        array[j].h = temp;
        Delay_Function(2);
    }
    Delay_Function(2);
    q_sort(array, i);
    q_sort(array + i, size - i);
}
void quick_sort(SDL_FRect *array, int size)
{
    q_sort(array, size);
    *thrd_created = false;
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
        Delay_Function(2);
    }
    for (j = 0; j < n2; j++)
    {
        right_array[j].h = array[middle + 1 + j].h;
        Delay_Function(2);
    }
    i = 0;
    j = 0;
    k = left;
    while (i < n1 && j < n2)
    {
        iterators[0] = i;
        iterators[1] = j;
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
        Delay_Function(2);
    }

    while (i < n1)
    {
        array[k].h = left_array[i].h;
        i++;
        k++;

        Delay_Function(2);
    }
    while (j < n2)
    {
        array[k].h = right_array[j].h;
        j++;
        k++;
        Delay_Function(2);
    }
    free(left_array);
    free(right_array);
}

void merge_sort(SDL_FRect *array, int left, int right)
{
    // set both iterators to -1
    iterators[0] = -1;
    iterators[1] = -1;

    if (left < right)
    {
        int middle = (left + right) / 2;
        merge_sort(array, left, middle);
        merge_sort(array, middle + 1, right);
        merge(array, left, middle, right);
    }
    delayTime = SDL_GetTicks();
    SDL_Delay(2);
    *delay += (SDL_GetTicks() - delayTime - 20);
}

void merge_sort_s(SDL_FRect *array, int size)
{
    merge_sort(array, 0, size);
    *thrd_created = false;
}

void heapify(SDL_FRect *array, int size, int i)
{
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    if (left < size && array[left].h > array[largest].h)
    {
        largest = left;
        iterators[0] = left;
        iterators[1] = largest;
        Delay_Function(2);
    }
    if (right < size && array[right].h > array[largest].h)
    {
        largest = right;
        iterators[0] = right;
        iterators[1] = largest;
        Delay_Function(2);
    }
    if (largest != i)
    {
        int temp = array[i].h;
        array[i].h = array[largest].h;
        array[largest].h = temp;
        heapify(array, size, largest);
        Delay_Function(2);
    }
}

void heap_sort(SDL_FRect *array, int size)
{
    // set both iterators to -1
    iterators[0] = -1;
    iterators[1] = -1;

    int i;
    for (i = size / 2 - 1; i >= 0; i--)
    {
        heapify(array, size, i);
        Delay_Function(2);
    }
    for (i = size - 1; i >= 0; i--)
    {
        float temp = array[0].h;
        array[0].h = array[i].h;
        array[i].h = temp;
        heapify(array, i, 0);
        Delay_Function(2);
    }
    *thrd_created = false;
}
