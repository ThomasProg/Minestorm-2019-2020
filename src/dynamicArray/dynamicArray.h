#ifndef _DYNAMIC_ARRAY_H_
#define _DYNAMIC_ARRAY_H_

#include <stdbool.h>

typedef struct 
{
    unsigned int unusedItemLowerBound; //AddItem optimisation
    unsigned int usedItems;            //Length optimisation

    unsigned int nbItems; //stockage
    void* data;           //stockage
    bool* isUsed;         //stockage
    size_t itemSize;      //stockage


} t_dynamicArray;

void dynamicArray_Init(t_dynamicArray* array, size_t itemSize, unsigned int defaultNbItems);

void* dynamicArray_AddItem(t_dynamicArray* array);
void  dynamicArray_RemoveItem(t_dynamicArray* array, unsigned int index);
void* dynamicArray_GetItem(t_dynamicArray* array, unsigned int index);
void  dynamicArray_Destroy(t_dynamicArray* array);
void  dynamicArray_RemoveRange(t_dynamicArray* array, unsigned int first, unsigned int lastPlusOne);

bool dynamicArray_GetValidItemIndex(t_dynamicArray* array, unsigned int* i);
/**
 * Usage :
 * for (unsigned int i = 0; dynamicArray_dynamicArray_GetValidItemIndex(array, &i); i++) 
 * {
 *      //do stuff
 *      //get item with dynamicArray_GetItem(array, i);
 * }
 */

#endif