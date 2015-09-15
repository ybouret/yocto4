#include <stdio.h>
#include "paulslib.h"
#include "bitmaplib.h"
#include "jpeglib.h"

int JPEG_Write(FILE *,BITMAP4 *,int,int,int);
int JPEG_Info(FILE *,int *,int *,int *);
int JPEG_Read(FILE *,BITMAP4 *,int *,int *);

