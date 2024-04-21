#ifndef CW_TESTS_MEMORY_H
#define CW_TESTS_MEMORY_H

#include "main.h"

#define ARRADD 20

void expandArray(void** array, size_t elsize, size_t* nel);
void freeImg(image img);

#endif //CW_TESTS_MEMORY_H
