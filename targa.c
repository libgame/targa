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

typedef enum {
    TRUE_COLOR     = 0,
    COLOR_MAPPED   = 1
} TGA_COLORMAP_TYPE;

typedef enum {
    NO_IMAGE_DATA                = 0,
    UNCOMPRESSED_COLOR_MAPPED    = 1,
    UNCOMPRESSED_TRUE_COLOR      = 2,
    UNCOMPRESSED_BLACK_AND_WHITE = 3,
    RLE_COLOR_MAPPED             = 9,
    RLE_TRUE_COLOR               = 10,
    RLE_BLACK_AND_WHITE          = 11 
} TGA_IMAGE_TYPE;

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
    uint8_t pixelDepth;
    char   imageDescriptor;
} TGA_H_IMAGE_SPEC;

typedef struct {
    uint16_t idLength;
    uint16_t colorMapType;      // enum TGA_COLORMAP_TYPE
    uint16_t imageType;         // enum TGA_IMAGE_TYPE
    TGA_COLOR_MAP_SPEC colorMapSpec;
    TGA_H_IMAGE_SPEC imageSpec;
} TGA_FILE_HEADER;

typedef struct {
    char extensionAreaOffset[4];
    char developerDirectoryOffset[4];
    char signature[16];                 // "TRUEVISION-XFILE"
    char dotCharacter;                  // "."
    char zeroStrTerminator;             // 0x00
} TGA_FILE_FOOTER;

void targaLoad(const char* fileName, int* status)
{
    FILE* targaFile = fopen(fileName, "rb");

    if (!targaFile) {

        printf("openfile failed\n");
        return;

    }

    if (fseek(targaFile, -26, SEEK_END) != 0) {

        printf("can not read end of file");
        return;

    }

    // footer
    TGA_FILE_FOOTER tgaFooter;
    if (fread(&tgaFooter, sizeof(TGA_FILE_FOOTER), 1, targaFile) < 1)
    {

        printf("can not read footer\n");
        return;

    }

    // define targa version 
    int isNewTgaFormat = 0;
    if (strncmp(tgaFooter.signature, "TRUEVISION-XFILE", 16) == 0)
        isNewTgaFormat = 1;

    // fields 1 - 5
    rewind(targaFile);
    TGA_FILE_HEADER tgaHeader;
    if (fread(&tgaHeader, sizeof(TGA_FILE_HEADER), 1, targaFile) < 1)
    {

        printf("can not read header\n");
        return;

    }

    // field 6 - Image ID
    char* imageId = NULL;
    if (tgaHeader.idLength != 0) 
    {

        imageId = malloc(sizeof(char) * tgaHeader.idLength);
        fread(imageId, sizeof(char), tgaHeader.idLength, targaFile);

    }

    // field 7 - Color Map Data
    char* mapData = NULL;
    int useColorMap = 0;
    if (tgaHeader.colorMapType != 0)
    {

        useColorMap = 1;
        int mapDataSize =
              (int) tgaHeader.colorMapSpec.mapLenght
            * (int) tgaHeader.colorMapSpec.mapEntrySize // in bits
            / 8;
        mapData = malloc(sizeof(char) * mapDataSize);
        fread(mapData, sizeof(char), mapDataSize, targaFile);
        printf("contains data %d\n", mapDataSize);

        // TODO
        // will contain tgaHeader.colorMapSpec.mapLength elements
        // of size tgaHeader.colorMapSpec.mapEntrySize
        // starting at index tgaHeader.colorMapSpec.firstEntryIndex
    }

    // field 8 - Image Data
    char* imageData = NULL;
    int numberOfPixels = 
          tgaHeader.imageSpec.imageWidth 
        * tgaHeader.imageSpec.imageHeight;

    if (!useColorMap)
    {
        // TODO use colormap indexes
    }
    else
    {
        int pixelSize = tgaHeader.imageSpec.pixelDepth;
        printf("should read %d pixels of %d size\n", numberOfPixels, pixelSize);
        printf("descriptor is %d\n", (uint8_t) tgaHeader.imageSpec.imageDescriptor);

        // TODO process imageDescriptor:
        /*
         *  Bits 3-0 - number of attribute bits associated with each pixel. 
         *  Bit 4    - reserved.  Must be set to 0. 
         *  Bit 5    - screen origin bit.
         *      0 = Origin in lower left-hand corner.
         *      1 = Origin in upper left-hand corner.
         *  Bits 7-6 - Data storage interleaving flag.
         *      00 = non-interleaved.
         *      01 = two-way (even/odd) interleaving.
         *      10 = four way interleaving.
         *      11 = reserved.
         */
    }
    




    printf("ok read %s\n", tgaFooter.signature);
    printf("ok read %d\n", isNewTgaFormat);

    if (imageId != NULL)
        free(imageId);

    if (mapData != NULL)
        free(mapData);

    if (imageData != NULL)
        free(imageData);

    fclose(targaFile);

}

