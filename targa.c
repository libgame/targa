/*
 * MIT License
 *
 * TARGA Copyright (c) 2016 Sebastien Serre <ssbx@sysmo.io>.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "targa.h"
#include <stdio.h>
#include <inttypes.h>

#define CMT_TRUE_COLOR   0
#define CMT_COLOR_MAPPED 1

#define IMG_TYPE_NO_IMAGE_DATA                0
#define IMG_TYPE_UNCOMPRESSED_COLOR_MAPPED    1
#define IMG_TYPE_UNCOMPRESSED_TRUE_COLOR      2
#define IMG_TYPE_UNCOMPRESSED_BLACK_AND_WHITE 3
#define IMG_TYPE_RLE_COLOR_MAPPED             9
#define IMG_TYPE_RLE_TRUE_COLOR               10
#define IMG_TYPE_RLE_BLACK_AND_WHITE          11 


typedef struct {
    uint16_t firstEntryIndex;
    uint16_t mapLenght;
    uint8_t  mapEntrySize;
} TGA_COLOR_MAP_SPEC;


typedef struct {
    uint16_t xOriginOfImage;
    uint16_t yOriginOfImage;
    uint16_t imageWidth;
    uint16_t imageHeight;
    uint8_t  pixelDepth;
    uint8_t  imageDescriptor;
} TGA_H_IMAGE_SPEC;


typedef struct {
    uint8_t idLength;
    uint8_t colorMapType;      // CMT_*
    uint8_t imageType;         // IMG_TYPE_*
    TGA_COLOR_MAP_SPEC colorMapSpec;
    TGA_H_IMAGE_SPEC imageSpec;
} TGA_FILE_HEADER;


typedef struct {
    uint8_t r,g,b;
} PIXEL;

void targaLoad(const char* fileName, int* status)
{

    FILE* TGA_file = fopen(fileName, "rb");

    if (!TGA_file) {

        printf("openfile failed\n");
        return;

    }

    /*
     * Read TGA header
     */
    TGA_FILE_HEADER TGA_header;
    if (fread(&TGA_header.idLength, sizeof(uint8_t), 1, TGA_file) < 1)
        return;
    if (fread(&TGA_header.colorMapType, sizeof(uint8_t), 1, TGA_file) < 1)
        return;
    if (fread(&TGA_header.imageType, sizeof(uint8_t), 1, TGA_file) < 1)
        return;
    if (fread(&TGA_header.colorMapSpec.firstEntryIndex, sizeof(uint16_t), 1, TGA_file) < 1)
        return;
    if (fread(&TGA_header.colorMapSpec.mapLenght, sizeof(uint16_t), 1, TGA_file) < 1)
        return;
    if (fread(&TGA_header.colorMapSpec.mapEntrySize, sizeof(uint8_t), 1, TGA_file) < 1)
        return;
    if (fread(&TGA_header.imageSpec.xOriginOfImage, sizeof(uint16_t), 1, TGA_file) < 1)
        return;
    if (fread(&TGA_header.imageSpec.yOriginOfImage, sizeof(uint16_t), 1, TGA_file) < 1)
        return;
    if (fread(&TGA_header.imageSpec.imageWidth, sizeof(uint16_t), 1, TGA_file) < 1)
        return;
    if (fread(&TGA_header.imageSpec.imageHeight, sizeof(uint16_t), 1, TGA_file) < 1)
        return;
    if (fread(&TGA_header.imageSpec.pixelDepth, sizeof(uint8_t), 1, TGA_file) < 1)
        return;
    if (fread(&TGA_header.imageSpec.imageDescriptor, sizeof(uint8_t), 1, TGA_file) < 1)
        return;


    /*
     * Format supported?
     */
    switch (TGA_header.imageType)
    {
        case IMG_TYPE_NO_IMAGE_DATA:
            printf("no data\n");
            return;
        case IMG_TYPE_UNCOMPRESSED_COLOR_MAPPED:
            printf("unsuported format color mapped\n");
            return;
        case IMG_TYPE_UNCOMPRESSED_TRUE_COLOR:
            break;
        case IMG_TYPE_UNCOMPRESSED_BLACK_AND_WHITE:
            printf("unsuported format black and white\n");
            return;
        case IMG_TYPE_RLE_COLOR_MAPPED:
            printf("unsuported format RLE color mapped\n");
            return;
        case IMG_TYPE_RLE_TRUE_COLOR:
            printf("unsuported format RLE true color\n");
            return;
        case IMG_TYPE_RLE_BLACK_AND_WHITE:
            printf("unsuported format RLE black and white\n");
            return;
    }



    /*
     * Skipp image id
     */
    fseek(TGA_file, TGA_header.idLength, SEEK_CUR);



    /*
     * No color map data
     */



    /*
     * Image data
     *
     *  Bits 3-0 - number of attribute bits associated with each pixel. 
     */
    uint8_t TGA_imgAttribNum = TGA_header.imageSpec.imageDescriptor & 0x0F;

    /*  Bit 4    - reserved.  Must be set to 0. */

    /*  Bit 5    - screen origin bit.
     *      0 = Origin in lower left-hand corner.
     *      1 = Origin in upper left-hand corner.
     */
    uint8_t TGA_imgScrOriginBit = TGA_header.imageSpec.imageDescriptor & 0x20 >> 5;

    /*  Bits 7-6 - Data storage interleaving flag.
     *      00 = non-interleaved.
     *      01 = two-way (even/odd) interleaving.
     *      10 = four way interleaving.
     *      11 = reserved.
     */
    uint8_t TGA_imgInterleaving = TGA_header.imageSpec.imageDescriptor & 0xC0 >> 6;


    int TGA_pixsize;
    if (TGA_imgAttribNum % 8 != 0)
        TGA_pixsize = TGA_imgAttribNum / 8 + 3 + 1;
    else
        TGA_pixsize = TGA_imgAttribNum / 8 + 3;

    printf("will have %d bits attributes per pixels\n", TGA_imgAttribNum);
    printf("will have %d screen origin bit\n", TGA_imgScrOriginBit);
    printf("will have %d interleaving\n", TGA_imgInterleaving);
    printf("will have %d pix size\n", TGA_pixsize);
    printf("will have %d pix depth\n", TGA_header.imageSpec.pixelDepth);
    printf("sizeof map spec %zu \n",sizeof(TGA_COLOR_MAP_SPEC));
    printf("sizeof image spec %zu \n",sizeof(TGA_H_IMAGE_SPEC));
    printf("sizeof file header spec %zu \n",sizeof(TGA_FILE_HEADER));

    unsigned char* TGA_imageData = NULL;
    int   TGA_numberOfPixels = 
          TGA_header.imageSpec.imageWidth 
        * TGA_header.imageSpec.imageHeight;

    TGA_imageData = malloc(sizeof(char) * TGA_numberOfPixels * TGA_pixsize);
    int i = 0;
    for (i; i < TGA_numberOfPixels; i++)
    {

        fread(&TGA_imageData[i * TGA_pixsize], sizeof(char), TGA_pixsize, TGA_file);
        /*
        printf("%d\tattrib: %d, red: %d, green: %d, blue: %d \n",
                i,
                TGA_imageData[i * TGA_pixsize],
                TGA_imageData[i * TGA_pixsize + 1],
                TGA_imageData[i * TGA_pixsize + 2],
                TGA_imageData[i * TGA_pixsize + 3]);
                */
        
    }






    if (TGA_imageData != NULL)
        free(TGA_imageData);

    fclose(TGA_file);

}

