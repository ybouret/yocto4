
#include "tiffio.h"

int main(int argc, char *argv[])
{
    TIFF* tif = TIFFOpen("foo.tif", "w");
    TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_JPEG);
    TIFFClose(tif);
}

