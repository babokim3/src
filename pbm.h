//
//  pbm.h
//  pbm_rw
//
//  Created by Artem Lenskiy on 4/16/15.
//  Copyright (c) 2015 Artem Lenskiy. All rights reserved.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h> //for memcpy

struct image_header{
    char format[3]; //Image format, example: P5
    int rows;       //Image height
    int cols;       //Image width
    int levels;     //Number of gray/each color levels
};

char *readImage(const char *file_name, unsigned long *length, struct image_header *ih);
void writeImage(const char *filename, const char *image_data, const struct image_header ih);

