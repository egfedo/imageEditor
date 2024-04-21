#include "main.h"

fileError readFile(char* filePath, size_t* size, char** buffer) {
    FILE* filePtr;
    char* inpBuffer;
    size_t fileLen;

    filePtr = fopen(filePath, "rb");
    if (!filePtr) {
        return FERR_WRONG_PATH;
    }
    fseek(filePtr, 0, SEEK_END);
    fileLen = ftell(filePtr);
    rewind(filePtr);
    inpBuffer = malloc(fileLen * sizeof(char));
    fread(inpBuffer, fileLen, 1, filePtr);
    fclose(filePtr);

    *size = fileLen;
    *buffer = inpBuffer;
    return FERR_NONE;
}

fileError readImage(char* filePath, image* img) {
    size_t size;
    char* buffer;
    fileError err = readFile(filePath, &size, &buffer);

    if(err)
        return err;
    if(!(buffer[0] == 'B' && buffer[1] == 'M'))
        return FERR_NOT_BMP;

    img->header = *(imageHeader*)(buffer);
    img->info = *(imageInfo*)(buffer + 14);
    size_t offset = img->header.byteOffset;
    char* fullHeader = malloc(offset);
    memcpy((void*)fullHeader, (void*)buffer, offset);
    img->fullHeader = fullHeader;

    if(img->info.size < 40)
        return FERR_VERSION;
    if(img->info.compression != 0)
        return FERR_COMPRESS;
    if(img->info.bitCount != 24)
        return FERR_BITS;

    pixel** array = malloc(sizeof(pixel*)*(img->info.height));
    pixel* line;
    size_t lineLen;

    if(((sizeof(pixel)*(img->info.width)) % 4 != 0))
        lineLen = sizeof(pixel)*(img->info.width) + 4 - ((sizeof(pixel)*(img->info.width)) % 4);
    else
        lineLen = sizeof(pixel)*(img->info.width);

    size_t arraySize = img->info.height * lineLen;

    if((img->info.imageSize != arraySize) || (size < arraySize + offset))
        return FERR_ARRAY;
    // printf("%zu = %zu", size, arraySize+offset);
    for(size_t i = 0; i < img->info.height; i++) {
        line = malloc(lineLen);
        // printf("Malloced %zu bytes for line\n", lineLen);
        memcpy((void*)line, (void*)(buffer + offset + lineLen*i), lineLen);
        array[i] = line;
    }

    img->array = array;
    free(buffer);
    return FERR_NONE;
}

fileError writeImage(char* filePath, image* img) {
    size_t offset = img->header.byteOffset;
    size_t lineLen;

    if(((sizeof(pixel)*(img->info.width)) % 4 != 0))
        lineLen = sizeof(pixel)*(img->info.width) + 4 - ((sizeof(pixel)*(img->info.width)) % 4);
    else
        lineLen = sizeof(pixel)*(img->info.width);

    size_t arrayLen = lineLen * img->info.height;
    // fprintf(stderr, "%zu %zu %zu\n", offset, lineLen, arrayLen);
    char* output = malloc(offset + arrayLen);
    memcpy((void*)(output), (void*)(img->fullHeader), offset);
    memcpy((void*)output, (void*)(&(img->header)), 14);
    memcpy((void*)(output + 14), (void*)(&(img->info)), sizeof(imageInfo));

    for(size_t i = 0; i < img->info.height; i++) {
        memcpy((void*)(output + offset + lineLen*i), (void*)((img->array)[i]), lineLen);
    }

    // fprintf(stderr, "%c%c\n", output[0], output[1]);
    FILE* out = fopen(filePath, "wb");

    if(out == NULL)
        return FERR_WRONG_PATH;

    fwrite((void*)output, 1, offset + arrayLen, out);
    fclose(out);
    free(output);
    return 0;
}