#include "main.h"

#ifndef CW_TESTS_FILE_H
#define CW_TESTS_FILE_H

fileError readFile(char* filePath, size_t* size, char** buffer);
fileError readImage(char* filePath, image* img);
fileError writeImage(char* filePath, image* img);

#endif //CW_TESTS_FILE_H
