
#include "tiffio.h"

int main(int argc, char *argv[])
{
    TIFF* tif = TIFFOpen("foo.tif", "r");
    TIFFClose(tif);
}

