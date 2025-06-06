#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

//errors
#define INVALID_SYNTX 1 
#define FILE_ERR 2

typedef uint8_t BYTE, CHAR;
typedef uint64_t VLINT;

FILE *openFile	(char *, char *);
void closeFile	(FILE *);
void closeFiles (VLINT, ...);

#endif
