#ifndef _DYNAMIC_ARRAY_H_
#define _DYNAMIC_ARRAY_H_

#include <stdbool.h>

typedef struct 
{
    void* data;
    bool* isUsed;
    unsigned int nbItems;
    size_t itemSize;
} t_dynamicArray;

void dynamicArray_Init(t_dynamicArray* array, size_t itemSize);

void* dynamicArray_AddItem(t_dynamicArray* array);
void  dynamicArray_RemoveItem(t_dynamicArray* array, unsigned int index);
void* dynamicArray_GetItem(t_dynamicArray* array, unsigned int index);
void  dynamicArray_Destroy(t_dynamicArray* array);
void  dynamicArray_RemoveRange(t_dynamicArray* array, unsigned int first, unsigned int lastPlusOne);

bool dynamicArray_ForEachUsed(t_dynamicArray* array, unsigned int* i);
/**
 * Usage :
 * for (unsigned int i = 0; dynamicArray_ForEachUsed(array, &i); i++) 
 * {
 *      //do stuff
 *      //get item with dynamicArray_GetItem(array, i);
 * }
 */

#endif