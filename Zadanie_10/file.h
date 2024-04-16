//
// Created by Martin on 3/11/2024.
//

#ifndef ZADANIE_4_FILE_H
#define ZADANIE_4_FILE_H

#endif //ZADANIE_4_FILE_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#define BUFFER_SIZE 256

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

char* generate_file_path(char* path)
{
    char base_name[BUFFER_SIZE] = {0};
    char extension[BUFFER_SIZE] = {0};
    char* generated_name = (char*) calloc(BUFFER_SIZE, sizeof(char));
    const char* dot = strrchr(path, '.');

    time_t raw_time;
    struct tm* time_info;
    struct timeval tv;
    time(&raw_time);
    time_info = localtime(&raw_time);
    gettimeofday(&tv, NULL);

    if (dot != NULL)
    {
        strncpy(base_name, path, dot - path);
        base_name[dot - path] = '\0';
        strcpy(extension, dot);
    }
    else
    {
        strcpy(base_name, path);
        strcpy(extension, "");
    }

    snprintf(
            generated_name, BUFFER_SIZE, "%s_%02d-%02d-%04d_%02d-%02d-%02d-%03ld%s",
            base_name, time_info->tm_mday, time_info->tm_mon + 1,
            time_info->tm_year + 1900, time_info->tm_hour,
            time_info->tm_min, time_info->tm_sec, tv.tv_usec/1000, extension
    );

    return generated_name;
}