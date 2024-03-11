//
// Created by Martin on 3/11/2024.
//

#ifndef ZADANIE_4_FILE_H
#define ZADANIE_4_FILE_H

#endif //ZADANIE_4_FILE_H

#include <stdio.h>
#include <stdlib.h>

FILE* open_file(const char *path, const char *mode)
{
    FILE* file = fopen(path, mode);
    if (file == NULL) {
        perror("Error opening file");
        exit(1);
    }
    return file;
}

void close_file(FILE *file)
{
    if (fclose(file) != 0) {
        perror("Error closing file");
        exit(1);
    }
}

void write_to_file(FILE *file, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    vfprintf(file, format, args);
    va_end(args);
}
