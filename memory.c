#include "memory.h"

void expandArray(void** array, size_t elsize, size_t* nel) {
    *nel += ARRADD;
    void* temp = realloc(*array, elsize * (*nel));
    if(temp == NULL) {
        exit(1);
    }
    *array = temp;
}

void freeImg(image img) {
    size_t height = img.info.height;
    for(size_t i = 0; i < height; i++)
        free(img.array[i]);
    free(img.array);
    free(img.fullHeader);
}