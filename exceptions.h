#ifndef CW_TESTS_EXCEPTIONS_H
#define CW_TESTS_EXCEPTIONS_H
#include "main.h"

typedef enum {
    PERR_NONE,
    PERR_MISSING_OPTS,
    PERR_MISSING_ARGS,
    PERR_WRONG_FORMAT,
    PERR_FILE_ERR,
    PERR_CMD_ERR,
    PERR_LEN
} parseError;

typedef enum {
    IERR_NONE,
    IWAR_OOB,
    IERR_INVALID_THICK,
    IERR_INVALID_RADIUS,
    IERR_INVALID_COMPONENT,
    IERR_INVALID_DIMENSION,
    IERR_INVALID_RECTANGLE,
    IERR_LEN
} imageError;

typedef enum {
    FERR_NONE,
    FERR_WRONG_PATH,
    FERR_NOT_BMP,
    FERR_COMPRESS,
    FERR_ARRAY,
    FERR_BITS,
    FERR_VERSION,
    FERR_LEN
} fileError;

void exitPerr(parseError e);
void printIerr(imageError e);
void printFerr(fileError e);

#endif //CW_TESTS_EXCEPTIONS_H
