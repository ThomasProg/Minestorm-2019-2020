#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <stdio.h>

#include "dynamicArray.h"

void dynamicArray_RemoveRange(t_dynamicArray* array, unsigned int first, unsigned int lastPlusOne)
{
    while (first < lastPlusOne)
    {
        array->isUsed[first] = false;
        first++;
    }
}

void dynamicArray_Init(t_dynamicArray* array, size_t itemSize)
{
    array->isUsed = malloc(8 * sizeof(bool));
    dynamicArray_RemoveRange(array, 0, 4);
    array->data = malloc(8 * itemSize);
    //fprintf(stderr, "MEMORY ALLOCATED : %lu\n", 8 * itemSize);
    array->nbItems = 4;
    array->itemSize = itemSize;
}

void* dynamicArray_AddItem(t_dynamicArray* array)
{
    unsigned int i = 0;
    do
    {
        if (i >= array->nbItems)
        {
            array->data = realloc(array->data, 2 * array->nbItems * array->itemSize);
            array->isUsed = realloc(array->data, 2 * array->nbItems * sizeof(bool));
            
            dynamicArray_RemoveRange(array, array->nbItems, 2 * array->nbItems);

            array->nbItems *= 2;
        }

        if (array->isUsed[i])
            i++;    

    } while (array->isUsed[i]);

    array->isUsed[i] = true;
    return & ((char*)array->data) [i * array->itemSize];
}

void dynamicArray_RemoveItem(t_dynamicArray* array, unsigned int index)
{
    assert(index < array->nbItems && index >= 0); //index is not valid
    array->isUsed[index] = false;
}

void* dynamicArray_GetItem(t_dynamicArray* array, unsigned int index)
{
    assert(index < array->nbItems && index >= 0); //index is not valid
    return & ((char*)array->data) [index * array->itemSize];
}

void dynamicArray_Destroy(t_dynamicArray* array)
{
    free(array->data);
    free(array->isUsed);
}

bool dynamicArray_ForEachUsed(t_dynamicArray* array, unsigned int* i)
{
    if (*i > array->nbItems)
        return false;

    while (!array->isUsed[*i])
    {
        (*i)++;
        if (*i > array->nbItems)
            return false;
    }

    return true;
}