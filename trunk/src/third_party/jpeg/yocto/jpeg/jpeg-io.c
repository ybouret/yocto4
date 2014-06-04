
#include "jpeglib.h"

JSAMPLE * image_buffer = 0;
int image_height;	/* Number of rows in image */
int image_width;		/* Number of columns in image */

GLOBAL(int)
read_JPEG_file (char * filename);
GLOBAL(void)
write_JPEG_file (char * filename, int quality);

int main(int argc, char *argv[])
{
    
    if(argc>1)
    {
        read_JPEG_file(argv[1]);
        //write_JPEG_file("toto.jpg", 70);
    }
    
    return 0;
    
}