#include <lcom/lcf.h>
#include "bitmap.h"
#include "game.h"
#include "stdio.h"

extern Bitmap* bitmaps_to_destroy[TOTAL_BITMAPS];

Bitmap* loadBitmap(char* filename) {
    // allocating necessary size
    Bitmap* bmp = (Bitmap*) malloc(sizeof(Bitmap));

    char * file;
    if((file = malloc(strlen(res_path)+strlen(filename)+1)) != NULL){
        file[0] = '\0';   // ensures the memory is an empty string
        strcat(file,res_path);
        strcat(file,filename);
    } else {
        printf("malloc failed!\n");
        return NULL;
    }
    // open filename in read binary mode
    FILE *filePtr;
    filePtr = fopen(file, "rb");
    if (filePtr == NULL){
        printf("File not found: %s", file);
        return NULL;
    }

    // read the bitmap file header
    BitmapFileHeader bitmapFileHeader;
    fread(&bitmapFileHeader, 2, 1, filePtr);

    // verify that this is a bmp file by check bitmap id
    if (bitmapFileHeader.type != 0x4D42) {
        fclose(filePtr);
        return NULL;
    }

    int rd;
    do {
        if ((rd = fread(&bitmapFileHeader.size, 4, 1, filePtr)) != 1)
            break;
        if ((rd = fread(&bitmapFileHeader.reserved, 4, 1, filePtr)) != 1)
            break;
        if ((rd = fread(&bitmapFileHeader.offset, 4, 1, filePtr)) != 1)
            break;
    } while (0);

    if (rd != 1) {
        fprintf(stderr, "Error reading file\n");
        exit(-1);
    }

    // read the bitmap info header
    BitmapInfoHeader bitmapInfoHeader;
    fread(&bitmapInfoHeader, sizeof(BitmapInfoHeader), 1, filePtr);

    // move file pointer to the begining of bitmap data
    fseek(filePtr, bitmapFileHeader.offset, SEEK_SET);

    // allocate enough memory for the bitmap image data
    unsigned int* bitmapImage = (unsigned int*) malloc(bitmapInfoHeader.imageSize);

    // verify memory allocation
    if (!bitmapImage) {
        free(bitmapImage);
        fclose(filePtr);
        return NULL;
    }

    // read in the bitmap image data
    int n_lines = bitmapInfoHeader.height;
    int line_size = bitmapInfoHeader.width;
    for(int i = (n_lines-1)*line_size; i >= 0; i -= line_size){  //ImageSize/height is number of lines
        fread(bitmapImage+i, bitmapInfoHeader.bits/8, bitmapInfoHeader.width, filePtr); //writing width nº of block of size of each pixel each
        //fseek(filePtr, bitmapInfoHeader.width, SEEK_CUR);
    }

    // make sure bitmap image data was read
    if (bitmapImage == NULL) {
        fclose(filePtr);
        return NULL;
    }

    // close file and return bitmap image data
    fclose(filePtr);

    bmp->bitmapData = bitmapImage;
    bmp->bitmapInfoHeader = bitmapInfoHeader;

    for(unsigned int i = 0; i < TOTAL_BITMAPS; i++)
    {
        if(bitmaps_to_destroy[i] == NULL)
        {
            bitmaps_to_destroy[i] = bmp;
            break;
        }
    }

    return bmp;
}

void deleteBitmap(Bitmap* bmp) {
    if (bmp == NULL)
        return;

    free(bmp->bitmapData);
    free(bmp);
}
