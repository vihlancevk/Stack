#ifndef FILE_OPERATIONS_H_
#define FILE_OPERATIONS_H_

#include <stdio.h>
#include <fstream>
#include <assert.h>
#include <string.h>
#include "stack.h"

/// Описание строки текста.
struct Line {
    char *str;  ///< Строка текста.
    int sizeStr;///< Размер строки.
};

int getFileSize(FILE *finput);
void *readFile(FILE *finput, char *str, int numberBytesSize);
int countNumberLines(char *str, int numberBytesFile);
void splitToLines(Line *lines, int linesCount, char *str);
void *fillStructLine(const char* nameFile ,int *linesCount, char *str);
StackErrorCode writeFile(FILE *foutput, Line *lines, int linesCount, stack_t *stack);

#endif
