#ifndef __BMP_H
#define __BMP_H
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

extern int *lcd;
int show_bmp(char *filename, int x1, int y1);



#endif