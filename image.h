#include "main.h"

#ifndef CW_TESTS_IMAGE_H
#define CW_TESTS_IMAGE_H
;
#pragma pack(push, 1)
typedef struct {
    uint16_t sign; // сигнатура bmp
    uint32_t size; // размер файла в байтах
    uint16_t res1; // резерв 1
    uint16_t res2; // резерв 2
    uint32_t byteOffset; // сдвиг начала данных
} imageHeader;

typedef struct {
    uint32_t size; // размер в байтах
    uint32_t width; // ширина
    uint32_t height; // высота
    uint16_t planes;
    uint16_t bitCount;
    uint32_t compression; // сжатие
    uint32_t imageSize; // размер массива пикселей
    uint32_t pxlXPerM;
    uint32_t pxlYPerM;
    uint32_t colorsSize;
    uint32_t colorsAmt;
} imageInfo;

typedef struct {
    uint8_t b;
    uint8_t g;
    uint8_t r;
} pixel;

#pragma pack(pop)

typedef struct {
    imageHeader header;
    imageInfo info;
    char* fullHeader;
    pixel** array;
} image;

typedef struct {
    double x;
    double y;
} vec;

imageError paintPixel(image* img, vec loc, pixel color);
double findDist(vec a, vec b);
imageError drawPxLine(image* img, vec a, vec b, pixel color);
imageError drawLine(image* img, vec a, vec b, pixel color, uint32_t thick);
int drawPxCircle(image *img, vec center, double r, pixel color);
imageError setComponent(image* img, char comp, size_t val);
imageError drawCircle(image *img, vec center, double r, double w, pixel color, pixel flood, int fcond);
int drawPxRectangle(image* img, vec a, vec b, pixel color);
pixel getPixel(image* img, vec loc);
int checkRectangle(image* img, vec a, vec* b, pixel color);
imageError outlineRectangles(image *img, pixel rectColor, pixel outline, uint32_t thick);
imageError drawRectangle(image* img, vec a, vec b, pixel color, uint32_t thick);
imageError splitImage(image* img, uint32_t n, uint32_t m, uint32_t thick, pixel color);
image generateImage(uint32_t width, uint32_t height, pixel background);
imageError rectCircle(image *img, vec a, vec b, double w, pixel color, pixel flood, int fcond);
int checkNearby(image* img, vec pos);
imageError outlineWhite(image* img, pixel outline);

#endif //CW_TESTS_IMAGE_H
