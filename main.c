#include "main.h"

char* help =
        "Справка:\n\n"
        "Формат ввода координаты: x,y\n"
        "Формат ввода цвета: r,g,b\n\n"
        "-h --help - вызывает данное сообщение\n\n"
        "-c --circle - рисование окружности\n"
        "\t-f --first - первая точка или центр\n"
        "\t-r --radius - радиус\n"
        "\tor\n"
        "\t-s --second - вторая точка\n"
        "\t-l --color - цвет\n"
        "\t-t --thickness - толщина окружности\n"
        "\t-f --fill - заливка окружности\n"
        "\t[-o --out] - название файла для вывода\n\n"
        "-t --setcomp - сменить компонент\n"
        "\t-p --component - компонент\n"
        "\t-v --value - значение\n"
        "\t[-o --out] - название файла для вывода\n\n"
        "-r --rectoutline - обвести прямоугольники\n"
        "\t-l --color - цвет прямоугольника\n"
        "\t-n --line - цвет обводки\n"
        "\t-t --thickness - толщина обводки\n"
        "\t[-o --out] - название файла для вывода\n\n"
        "-s --split - разделить изображение на части\n"
        "\t-x --xcount - кол-во частей по оси x\n"
        "\t-y --ycount - кол-во частей по оси y\n"
        "\t-l --color - цвет линий\n"
        "\t-t --thickness - толщина линий\n"
        "\t[-o --out] - название файла для вывода\n\n"
        "-n --new - создать новое изображение\n"
        "\t-w --width - ширина изображения\n"
        "\t-h --height - высота изображения\n"
        "\t-b --background - цвет фона\n\n"
;

parseError parseCircle(int argc, char **argv) {
    int c;
    static struct option long_options[] = {
            {"first", 1, 0, 'f'},
            {"second", 1, 0, 's'},
            {"radius", 1, 0, 'r'},
            {"color", 1, 0, 'l'},
            {"thickness", 1, 0, 't'},
            {"fill", 1, 0, 'i'},
            {"out", 1, 0, 'o'},
    };
    int optionI;

    vec first = {-1, -1};
    int firstInp = 0;
    vec second = {-1, -1};
    int secondInp = 0;
    float radius = -1;
    int radiusInp = 0;
    pixel color = {0, 0, 0};
    int colorInp = 0;
    float thickness = -1;
    int thickInp = 0;
    pixel fill = {0, 0, 0};
    int fillInp = 0;
    char* fileName;
    int fileInp = 0;
    char* outFile;
    int outInp = 0;

    while(1) {
        c = getopt_long(argc, argv, "f:s:r:l:t:i:o:", long_options, &optionI);
        if(c == -1)
            break;
        switch(c) {
            case 'f':
                if(sscanf(optarg, "%lf,%lf", &(first.x), &(first.y)) != 2)
                    return PERR_WRONG_FORMAT;
                firstInp = 1;
                break;
            case 's':
                if(sscanf(optarg, "%lf,%lf", &(second.x), &(second.y)) != 2)
                    return PERR_WRONG_FORMAT;
                secondInp = 1;
                break;
            case 'r':
                if(sscanf(optarg, "%f", &(radius)) != 1)
                    return PERR_WRONG_FORMAT;
                radiusInp = 1;
                break;
            case 'l':
                if(sscanf(optarg, "%hhu,%hhu,%hhu", &(color.r), &(color.g), &(color.b)) != 3)
                    return PERR_WRONG_FORMAT;
                colorInp = 1;
                break;
            case 't':
                if(sscanf(optarg, "%f", &(thickness)) != 1)
                    return PERR_WRONG_FORMAT;
                thickInp = 1;
                break;
            case 'i':
                if(sscanf(optarg, "%hhu,%hhu,%hhu", &(fill.r), &(fill.g), &(fill.b)) != 3)
                    return PERR_WRONG_FORMAT;
                fillInp = 1;
                break;
            case 'o':
                outFile = malloc(sizeof(char)*(strlen(optarg) + 1));
                strcpy(outFile, optarg);
                outInp = 1;
                break;
            case ':':
                return PERR_MISSING_ARGS;
            default:
                break;
        }
    }
    // printf("%d %d %d %d %d\n", firstInp, colorInp, thickInp, radiusInp, secondInp);
    if(optind < argc) {
        fileInp = 1;
        fileName = malloc(sizeof(char)*(strlen(argv[optind]) + 1));
        strcpy(fileName, argv[optind]);
    }

    if(!firstInp || !colorInp || !thickInp || !(radiusInp || secondInp) || !fileInp) {
        return PERR_MISSING_OPTS;
    }

    /* printf("Here it should draw a circle:\n"
           "Center or First: %d %d\n"
           "Second: %d %d\n"
           "Radius: %d\n"
           "Color: %hhu %hhu %hhu\n"
           "Thickness: %d\n"
           "Fill: %hhu %hhu %hhu\n"
           "File: %s\n",
           (int)floorf(first.x), (int)floorf(first.y), (int)floorf(second.x), (int)floorf(second.y),
           (int)floorf(radius), color.r, color.g, color.b, (int)floorf(thickness),
           fill.r, fill.g, fill.b, fileName); */
    image img;
    fileError fcode = readImage(fileName, &img);
    printFerr(fcode);
    if(fcode) {
        return PERR_FILE_ERR;
    }
    imageError code;
    if(radiusInp)
        code = drawCircle(&img, first, radius, thickness, color, fill, fillInp);
    else
        code = rectCircle(&img, first, second, thickness, color, fill, fillInp);
    printIerr(code);
    if(code) {
        return PERR_CMD_ERR;
    }
    if(outInp) {
        writeImage(outFile, &img);
        free(outFile);
    }
    else
        writeImage(fileName, &img);
    free(fileName);
    freeImg(img);
    return PERR_NONE;
}

parseError parseComponent(int argc, char **argv) {
    int c;
    static struct option long_options[] = {
            {"component", 1, 0, 'p'},
            {"value", 1, 0, 'v'},
            {"out", 1, 0, 'o'},
    };
    int optionI;

    char component;
    int compInp = 0;
    uint8_t value;
    int valInp = 0;
    char* fileName;
    int fileInp = 0;
    char* outFile;
    int outInp = 0;

    while(1) {
        c = getopt_long(argc, argv, "p:v:o:", long_options, &optionI);
        if(c == -1)
            break;
        switch(c) {
            case 'p':
                if(sscanf(optarg, "%c", &component) != 1)
                    return PERR_WRONG_FORMAT;
                component = tolower(component);
                compInp = 1;
                break;
            case 'v':
                if(sscanf(optarg, "%hhu", &(value)) != 1)
                    return PERR_WRONG_FORMAT;
                valInp = 1;
                break;
            case 'o':
                outFile = malloc(sizeof(char)*(strlen(optarg) + 1));
                strcpy(outFile, optarg);
                outInp = 1;
                break;
            case ':':
                return PERR_MISSING_ARGS;
            default:
                break;
        }
    }
    // printf("%d %d %d %d %d\n", firstInp, colorInp, thickInp, radiusInp, secondInp);
    if(optind < argc) {
        fileInp = 1;
        fileName = malloc(sizeof(char)*(strlen(argv[optind]) + 1));
        strcpy(fileName, argv[optind]);
    }
    // printf("%d %d %d %d\n", compInp, valInp, outInp, fileInp);
    if(!compInp || !valInp || !fileInp) {
        return PERR_MISSING_OPTS;
    }

    /* printf("Here it should change component:\n"
           "Component: %c\n"
           "Value: %hhu\n"
           "File: %s\n",
           component, value, fileName); */
    image img;
    fileError fcode = readImage(fileName, &img);
    printFerr(fcode);
    if(fcode) {
        return PERR_FILE_ERR;
    }
    imageError code = setComponent(&img, component, value);
    printIerr(code);
    if(code) {
        return PERR_CMD_ERR;
    }
    if(outInp) {
        writeImage(outFile, &img);
        free(outFile);
    }
    else
        writeImage(fileName, &img);
    free(fileName);
    freeImg(img);
    return PERR_NONE;
}

parseError parseOutline(int argc, char **argv) {
    int c;
    static struct option long_options[] = {
            {"color", 1, 0, 'l'},
            {"line", 1, 0, 'n'},
            {"thickness", 1, 0, 't'},
            {"out", 1, 0, 'o'},
    };
    int optionI;

    pixel color;
    int colorInp = 0;
    pixel line;
    int lineInp = 0;
    uint32_t thickness;
    int thickInp = 0;
    char* fileName;
    int fileInp = 0;
    char* outFile;
    int outInp = 0;

    while(1) {
        c = getopt_long(argc, argv, "l:n:t:o:", long_options, &optionI);
        if(c == -1)
            break;
        switch(c) {
            case 'l':
                if(sscanf(optarg, "%hhu,%hhu,%hhu", &(color.r), &(color.g), &(color.b)) != 3)
                    return PERR_WRONG_FORMAT;
                colorInp = 1;
                break;
            case 'n':
                if(sscanf(optarg, "%hhu,%hhu,%hhu", &(line.r), &(line.g), &(line.b)) != 3)
                    return PERR_WRONG_FORMAT;
                lineInp = 1;
                break;
            case 't':
                if(sscanf(optarg, "%u", &(thickness)) != 1)
                    return PERR_WRONG_FORMAT;
                thickInp = 1;
                break;
            case 'o':
                outFile = malloc(sizeof(char)*(strlen(optarg) + 1));
                strcpy(outFile, optarg);
                outInp = 1;
                break;
            case ':':
                return PERR_MISSING_ARGS;
            default:
                break;
        }
    }
    // printf("%d %d %d %d %d\n", firstInp, colorInp, thickInp, radiusInp, secondInp);
    if(optind < argc) {
        fileInp = 1;
        fileName = malloc(sizeof(char)*(strlen(argv[optind]) + 1));
        strcpy(fileName, argv[optind]);
    }
    // printf("%d %d %d %d\n", colorInp, lineInp, thickInp, fileInp);
    if(!colorInp || !lineInp || !thickInp || !fileInp) {
        return PERR_MISSING_OPTS;
    }

    /*printf("Here it should outline rectangles:\n"
           "Color: %hhu, %hhu, %hhu\n"
           "Line: %hhu, %hhu, %hhu\n"
           "Thickness: %u\n"
           "File: %s\n",
           color.r, color.g, color.b, line.r, line.g, line.b, thickness, fileName);*/
    image img;
    fileError fcode = readImage(fileName, &img);
    printFerr(fcode);
    if(fcode) {
        return PERR_FILE_ERR;
    }
    imageError code = outlineRectangles(&img, color, line, thickness);
    printIerr(code);
    if(code) {
        return PERR_CMD_ERR;
    }
    if(outInp) {
        writeImage(outFile, &img);
        free(outFile);
    }
    else
        writeImage(fileName, &img);
    free(fileName);
    freeImg(img);
    return PERR_NONE;
}

parseError parseSplit(int argc, char **argv) {
    int c;
    static struct option long_options[] = {
            {"xcount", 1, 0, 'x'},
            {"ycount", 1, 0, 'y'},
            {"color", 1, 0, 'l'},
            {"thickness", 1, 0, 't'},
            {"out", 1, 0, 'o'},
    };
    int optionI;

    uint32_t xcount;
    int xInp = 0;
    uint32_t ycount;
    int yInp = 0;
    pixel color;
    int colorInp = 0;
    uint32_t thickness;
    int thickInp = 0;
    char* fileName;
    int fileInp = 0;
    char* outFile;
    int outInp = 0;

    while(1) {
        c = getopt_long(argc, argv, "x:y:l:t:", long_options, &optionI);
        if(c == -1)
            break;
        switch(c) {
            case 'l':
                if(sscanf(optarg, "%hhu,%hhu,%hhu", &(color.r), &(color.g), &(color.b)) != 3)
                    return PERR_WRONG_FORMAT;
                colorInp = 1;
                break;
            case 'x':
                if(sscanf(optarg, "%u", &(xcount)) != 1)
                    return PERR_WRONG_FORMAT;
                xInp = 1;
                break;
            case 'y':
                if(sscanf(optarg, "%u", &(ycount)) != 1)
                    return PERR_WRONG_FORMAT;
                yInp = 1;
                break;
            case 't':
                if(sscanf(optarg, "%u", &(thickness)) != 1)
                    return PERR_WRONG_FORMAT;
                thickInp = 1;
                break;
            case 'o':
                outFile = malloc(sizeof(char)*(strlen(optarg) + 1));
                strcpy(outFile, optarg);
                outInp = 1;
                break;
            case ':':
                return PERR_MISSING_ARGS;
            default:
                break;
        }
    }
    // printf("%d %d %d %d %d\n", xInp, yInp, colorInp, thickInp, fileInp);
    if(optind < argc) {
        fileInp = 1;
        fileName = malloc(sizeof(char)*(strlen(argv[optind]) + 1));
        strcpy(fileName, argv[optind]);
    }
    //printf("%d %d %d %d %d\n", xInp, yInp, colorInp, thickInp, fileInp);
    if(!colorInp || !xInp || !yInp || !thickInp || !fileInp) {
        return PERR_MISSING_OPTS;
    }

    /*printf("Here it should split image:\n"
           "Color: %hhu, %hhu, %hhu\n"
           "X count: %u\n"
           "Y count: %u\n"
           "Thickness: %u\n"
           "File: %s\n",
           color.r, color.g, color.b, xcount, ycount, thickness, fileName); */
    image img;
    fileError fcode = readImage(fileName, &img);
    printFerr(fcode);
    if(fcode) {
        return PERR_FILE_ERR;
    }
    imageError code = splitImage(&img, ycount, xcount, thickness, color);
    printIerr(code);
    if(code) {
        return PERR_CMD_ERR;
    }
    if(outInp) {
        writeImage(outFile, &img);
        free(outFile);
    }
    else
        writeImage(fileName, &img);
    free(fileName);
    freeImg(img);
    return PERR_NONE;
}

parseError parseNew(int argc, char **argv) {
    int c;
    static struct option long_options[] = {
            {"background", 1, 0, 'b'},
            {"width", 1, 0, 'w'},
            {"height", 1, 0, 'h'},
    };
    int optionI;

    pixel color;
    int colorInp = 0;
    uint32_t width;
    int widthInp = 0;
    uint32_t height;
    int heightInp = 0;
    char* fileName;
    int fileInp = 0;

    while(1) {
        c = getopt_long(argc, argv, "b:w:h:", long_options, &optionI);
        if(c == -1)
            break;
        switch(c) {
            case 'b':
                if(sscanf(optarg, "%hhu,%hhu,%hhu", &(color.r), &(color.g), &(color.b)) != 3)
                    return PERR_WRONG_FORMAT;
                colorInp = 1;
                break;
            case 'w':
                if(sscanf(optarg, "%u", &(width)) != 1)
                    return PERR_WRONG_FORMAT;
                widthInp = 1;
                break;
            case 'h':
                if(sscanf(optarg, "%u", &(height)) != 1)
                    return PERR_WRONG_FORMAT;
                heightInp = 1;
                break;
            case ':':
                return PERR_MISSING_ARGS;
            default:
                break;
        }
    }
    // printf("%d %d %d %d %d\n", firstInp, colorInp, thickInp, radiusInp, secondInp);
    if(optind < argc) {
        fileInp = 1;
        fileName = malloc(sizeof(char)*(strlen(argv[optind]) + 1));
        strcpy(fileName, argv[optind]);
    }
    // printf("%d %d %d %d\n", colorInp, lineInp, thickInp, fileInp);
    if(!colorInp || !widthInp || !heightInp || !fileInp) {
        return PERR_MISSING_OPTS;
    }

    image img = generateImage(width, height, color);
    writeImage(fileName, &img);
    free(fileName);
    freeImg(img);
    return PERR_NONE;
}

parseError parseWhite(int argc, char** argv) {
    int c;
    static struct option long_options[] = {
            {"color", 1, 0, 'l'},
            {"out", 1, 0, 'o'},
    };
    int optionI;

    pixel color;
    int colorInp = 0;
    char* fileName;
    int fileInp = 0;
    char* outFile;
    int outInp = 0;

    while(1) {
        c = getopt_long(argc, argv, "l:o:", long_options, &optionI);
        if(c == -1)
            break;
        switch(c) {
            case 'l':
                if(sscanf(optarg, "%hhu,%hhu,%hhu", &(color.r), &(color.g), &(color.b)) != 3)
                    return PERR_WRONG_FORMAT;
                colorInp = 1;
                break;
            case 'o':
                outFile = malloc(sizeof(char)*(strlen(optarg) + 1));
                strcpy(outFile, optarg);
                outInp = 1;
                break;
            case ':':
                return PERR_MISSING_ARGS;
            default:
                break;
        }
    }

    if(optind < argc) {
        fileInp = 1;
        fileName = malloc(sizeof(char)*(strlen(argv[optind]) + 1));
        strcpy(fileName, argv[optind]);
    }

    if(!colorInp || !fileInp) {
        return PERR_MISSING_OPTS;
    }

    image img;
    fileError fcode = readImage(fileName, &img);
    printFerr(fcode);
    if(fcode) {
        return PERR_FILE_ERR;
    }
    imageError code = outlineWhite(&img, color);
    printIerr(code);
    if(code) {
        return PERR_CMD_ERR;
    }
    if(outInp) {
        writeImage(outFile, &img);
        free(outFile);
    }
    else
        writeImage(fileName, &img);
    free(fileName);
    freeImg(img);
    return PERR_NONE;
}

void printData(image img) {
    printf("Image data:\n"
           "Sign: %hu, Size: %d, Byte Offset: %d, Res1: %d, Res2: %d\n"
           "Size: %d, Width: %d, Height: %d, Planes: %d\n"
           "Bit Count: %d, Compression: %d, Image Size: %d, X per M: %d, Y per M: %d\n"
           "Color Size: %d, Color Amt: %d, Masks: R: %d, G: %d, B: %d, A: %d\n",
           img.header.sign, img.header.size, img.header.byteOffset, img.header.res1, img.header.res2,
           img.info.size, img.info.width, img.info.height, img.info.planes,
           img.info.bitCount, img.info.compression, img.info.imageSize, img.info.pxlXPerM, img.info.pxlYPerM,
           img.info.colorsSize, img.info.colorsAmt,
           *(img.fullHeader + 54), *(img.fullHeader + 58), *(img.fullHeader + 62), *(img.fullHeader + 64));
}

int main(int argc, char **argv) {
    int c;
    static struct option long_options[] = {
            {"help", 0, 0, 'h'},
            {"circle", 0, 0, 'c'},
            {"setcomp", 0, 0, 't'},
            {"rectoutline", 0, 0, 'r'},
            {"split", 0, 0, 's'},
            {"new", 0, 0, 'n'},
            {"whiteoutline", 0, 0, 'w'},
    };
    int optionI;

    while(1) {
        c = getopt_long(argc, argv, "chtrsn", long_options, &optionI);
        if(c == -1)
            break;
        switch(c) {
            case 'c':
                exitPerr(parseCircle(argc, argv));
                break;
            case 'h':
                printf("%s", help);
                break;
            case 't':
                exitPerr(parseComponent(argc, argv));
                break;
            case 'r':
                exitPerr(parseOutline(argc, argv));
                break;
            case 's':
                exitPerr(parseSplit(argc, argv));
                break;
            case 'n':
                exitPerr(parseNew(argc, argv));
                break;
            case 'w':
                exitPerr(parseWhite(argc, argv));
                break;
            default:
                break;
        }
    }
    return 0;
}
