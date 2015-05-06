//
//  pbm.c
//  pbm_rw
//
//  Created by Artem Lenskiy on 4/16/15.
//  Copyright (c) 2015 Artem Lenskiy. All rights reserved.
//

#include "pbm.h"

//char *rgb2ycbcr(FILE *pgmFile, char *RGB_2_YCbCr, char *image_data, struct image_header *ih){
//
//	int j, i;
//	char YCbCr[3];
//	for (j = 0; j < ih->rows; ++j)
//		for (i = 0; i < ih->cols; ++i) {
//			RGB_2_YCbCr = &image_data[j*(ih->cols * 3) + i * 3];
//			RGB_2_YCbCr[0] = (char)fgetc(pgmFile);	//R
//			RGB_2_YCbCr[1] = (char)fgetc(pgmFile);	//G
//			RGB_2_YCbCr[2] = (char)fgetc(pgmFile);	//B
//
//			YCbCr[0] = (19595*RGB_2_YCbCr[0]+38470*RGB_2_YCbCr[1]+7471*RGB_2_YCbCr[2])>>16; //Y
//			YCbCr[1] = (36962*(RGB_2_YCbCr[2]-YCbCr[0])>>16)+128;			//Cb
//			YCbCr[2] = (46727*(RGB_2_YCbCr[0]-YCbCr[0])>>16)+128;			//Cr
//
//			RGB_2_YCbCr[0] = YCbCr[0];
//			RGB_2_YCbCr[1] = YCbCr[1];
//			RGB_2_YCbCr[2] = YCbCr[2];
//
//		}
//	return RGB_2_YCbCr;
//}

char *readImage(const char *file_name, unsigned long *length, struct image_header *ih){
    FILE *pgmFile;
    int i, j;
    int pixel;
    char tmp[3], tmp2[6];

    char *image_data;
    char *pix;

    pgmFile = fopen(file_name, "rb");
    if (pgmFile == NULL) {
        fprintf(stderr, "Cannot open file to read!\n");
        fclose(pgmFile);
        return NULL;
    }

    fgets(ih->format, sizeof(ih->format), pgmFile);
    if (strcmp(ih->format, "P5") != 0 && strcmp(ih->format, "P6") != 0) {
        fprintf(stderr, "Wrong file type!\n");
        fclose(pgmFile);
        return NULL;
    }
    //read header
    fscanf(pgmFile, "%d", &ih->cols);
    fscanf(pgmFile, "%d", &ih->rows);
    fscanf(pgmFile, "%d", &ih->levels);
    fgetc(pgmFile);

    if (strcmp(ih->format, "P5") == 0){
        image_data = (char *)malloc(ih->rows * ih->cols);
    }else{
        image_data = (char *)malloc(3 * ih->rows * ih->cols);
    }

    if (strcmp(ih->format, "P5") == 0){
        for (j = 0; j < ih->rows; ++j)
            for (i = 0; i < ih->cols; ++i) {
                pixel = (char)fgetc(pgmFile);
                image_data[j*ih->cols + i] = pixel;
            }
    }else{
//    	 rgb2ycbcr(pgmFile, &pix, &image_data, &ih);
        for (j = 0; j < ih->rows; j++)
            for (i = 0; i < ih->cols; i++) {
                pix = &image_data[j*(ih->cols * 3) + i * 3];
                pix[0] = (char)fgetc(pgmFile);
                pix[1] = (char)fgetc(pgmFile);
                pix[2] = (char)fgetc(pgmFile);

                tmp[0] = (19595*pix[0]+38470*pix[1]+7471*pix[2])>>16;
                tmp[1] = (36962*(pix[2]-tmp[0])>>16)+128;
                tmp[2] = (46727*(pix[0]-tmp[0])>>16)+128;

                pix[0] = tmp[0];
                pix[1] = tmp[1];
                pix[2] = tmp[2];

            }
    }

    if (strcmp(ih->format, "P5") == 0){
        *length = ih->rows * ih->cols;
    }else{
        *length = 3 * ih->rows * ih->cols;
    }

//    for (j = 0; j < ih->rows; j++)
//    	for (i = 0; i < ih->cols; i++) {
//    		pix = &image_data[j*(ih->cols * 6) + i * 6];
//    		tmp2[0]=pix[0];
//    		tmp2[1]=pix[3];
//    		tmp2[2]=pix[ih->cols];
//    		tmp2[3]=pix[ih->cols+1];
//    		tmp2[4]=pix[1];
//    		tmp2[5]=pix[2];
//
//    		for( i = 0; i<6;i++){
//    			pix[i] = tmp2[i];
//    		}
//    	}


    fclose(pgmFile);
    return image_data;
}
//---------------------------------------------------------------------------------------------
void writeImage(const char *filename, const char *image_data, const struct image_header ih){
    FILE *pgmFile;
    int i, j;
    const char *pix;

    pgmFile = fopen(filename, "wb");
    if (pgmFile == NULL) {
        perror("cannot open file to write");
        exit(EXIT_FAILURE);
    }

    fprintf(pgmFile, "%s ", ih.format);
    fprintf(pgmFile, "%d %d ", ih.cols, ih.rows);
    fprintf(pgmFile, "%d ", ih.levels);

    if(strcmp(ih.format, "P5") == 0){
        for (j = 0; j < ih.rows; ++j)
            for (i = 0; i < ih.cols; ++i) {
                pix = &image_data[j*ih.cols + i];
                fputc(pix[0], pgmFile);
            }
    }else{
        for (j = 0; j < ih.rows; ++j)
            for (i = 0; i < ih.cols; ++i) {
                pix = &image_data[j * ih.cols * 3 + i * 3];
                fputc(pix[0], pgmFile);
                fputc(pix[1], pgmFile);
                fputc(pix[2], pgmFile);
            }
    }

    fclose(pgmFile);
}
