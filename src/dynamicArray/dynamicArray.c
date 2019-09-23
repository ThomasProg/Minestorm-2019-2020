#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "dynamicArray.h"

void dynamicArray_RemoveRange(t_dynamicArray* array, unsigned int first, unsigned int lastPlusOne)
{
    if (first < array->unusedItemLowerBound)
        array->unusedItemLowerBound = first;

    //use first as index to prevent another declaration
    //and to gain performance
    while (first < lastPlusOne)
    {
        array->isUsed[first] = false;
        first++;
    }
}

void dynamicArray_Init(t_dynamicArray* array, size_t itemSize, unsigned int defaultNbItems)
{
    assert(defaultNbItems > 0); //SIZE CAN'T BE NULL

    array->isUsed = malloc(defaultNbItems * sizeof(bool));
    array->data = malloc(defaultNbItems * itemSize);
    array->nbItems = defaultNbItems;
    array->itemSize = itemSize;
    array->unusedItemLowerBound = 0;
    array->usedItems = 0;
    //memset()
    dynamicArray_RemoveRange(array, 0, defaultNbItems);
    //fprintf(stderr, "MEMORY ALLOCATED : %lu\n", 8 * itemSize);
}

void* dynamicArray_AddItem(t_dynamicArray* array)
{
    unsigned int i = array->unusedItemLowerBound;
    while (i < array->nbItems && array->isUsed[i])
    {
        i++;
    }
    array->unusedItemLowerBound = i;

    if (i >= array->nbItems)
    {
        array->data = realloc(array->data, 2 * array->nbItems * array->itemSize);
        array->isUsed = realloc(array->isUsed, 2 * array->nbItems * sizeof(bool));
        
        for (unsigned int k = array->nbItems; k < array->nbItems * 2; k++)
        {
            array->isUsed[k] = false;
        }
        //dynamicArray_RemoveRange(array, array->nbItems, 2 * array->nbItems);

        array->nbItems *= 2;
    }

    array->usedItems++;
    array->isUsed[i] = true;
    return & ((char*)array->data) [i * array->itemSize];
}

void dynamicArray_RemoveItem(t_dynamicArray* array, unsigned int index)
{
    assert(index < array->nbItems && index >= 0); //index is not valid

    //optimisation
    if (index < array->unusedItemLowerBound)
        array->unusedItemLowerBound = index;
    array->usedItems--;

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

bool dynamicArray_GetValidItemIndex(t_dynamicArray* array, unsigned int* i)
{
    if (*i >= array->nbItems)
        return false;

    while (!array->isUsed[*i])
    {
        (*i)++;
        if (*i >= array->nbItems)
            return false;
    }

    return true;
}