#include "jpeg.h"

/*
 Write a JPEG file
 Quality is 0 to 100
 */
int JPEG_Write(FILE *fptr, BITMAP4 *image,int width,int height,int quality)
{
	int i,j;
	struct jpeg_compress_struct cinfo;
	JSAMPROW row_pointer[1];
	JSAMPLE *jimage = NULL;
	int row_stride;
	struct jpeg_error_mgr jerr;
    
	if ((jimage = malloc(width*3)) == NULL)
		return(1);
    
	// Error handler
    cinfo.err = jpeg_std_error(&jerr);
    
	// Initialize JPEG compression object.
	jpeg_create_compress(&cinfo);
	
	// Associate with output stream
	jpeg_stdio_dest(&cinfo,fptr);
    
	// Fill out values
    cinfo.image_width = width;
    cinfo.image_height = height;
    cinfo.input_components = 3;
    cinfo.in_color_space = JCS_RGB;
    
	// Default compression settings
	jpeg_set_defaults(&cinfo);
	jpeg_set_quality(&cinfo, quality, TRUE); // limit to baseline-JPEG values
    
	// Start cmpressor
	jpeg_start_compress(&cinfo, TRUE);
    
	row_stride = width * 3;
	row_pointer[0] = jimage;
	j = cinfo.image_height - 1;
	while (cinfo.next_scanline < cinfo.image_height) {
		for (i=0;i<width;i++) {
			jimage[3*i  ] = image[j*width+i].r;
            jimage[3*i+1] = image[j*width+i].g;
            jimage[3*i+2] = image[j*width+i].b;
		}
        jpeg_write_scanlines(&cinfo,row_pointer,1);
		j--;
    }
    
	jpeg_finish_compress(&cinfo);
	jpeg_destroy_compress(&cinfo);
    
	free(jimage);
	return(TRUE);
}

/*
 Get dimensions of a JPEG image
 */
int JPEG_Info(FILE *fptr,int *width,int *height,int *depth)
{
	int row_stride;
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;
	JSAMPARRAY buffer;
    
    // Error handler
    cinfo.err = jpeg_std_error(&jerr);
    
	jpeg_create_decompress(&cinfo);
	jpeg_stdio_src(&cinfo, fptr);
    
	// Read header
	jpeg_read_header(&cinfo, TRUE);
    
	// Read one scan line
	jpeg_start_decompress(&cinfo);
    row_stride = cinfo.output_width * cinfo.output_components;
    buffer = (*cinfo.mem->alloc_sarray)
    ((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);
    while (cinfo.output_scanline < cinfo.output_height)
        jpeg_read_scanlines(&cinfo,(JSAMPARRAY)buffer,1);
    
	*width = cinfo.output_width;
	*height = cinfo.output_height;
	*depth = 8*cinfo.output_components;
    
	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
    
	rewind(fptr);
    
	return(TRUE);
}

/*
 Read a JPEG image
 */
int JPEG_Read(FILE *fptr,BITMAP4 *image,int *width,int *height)
{
	int i,j,n;
	int row_stride;
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;
	JSAMPLE *buffer;
    
    // Error handler
    cinfo.err = jpeg_std_error(&jerr);
    
    jpeg_create_decompress(&cinfo);
    jpeg_stdio_src(&cinfo, fptr);
    
    // Read header
    jpeg_read_header(&cinfo, TRUE);
	jpeg_start_decompress(&cinfo);
    
    *width = cinfo.output_width;
    *height = cinfo.output_height;
    
	// Can only handle RGB JPEG images at this stage
	if (cinfo.output_components != 3)
		return(1);
    
	// buffer for one scan line
	row_stride = cinfo.output_width * cinfo.output_components;
	if ((buffer = malloc(row_stride * sizeof(JSAMPLE))) == NULL)
		return(2);
    
	j = cinfo.output_height-1;
    while (cinfo.output_scanline < cinfo.output_height) {
        n = jpeg_read_scanlines(&cinfo,&buffer,1);
		for (i=0;i<cinfo.output_width;i++) {
			image[j*cinfo.output_width+i].r = buffer[3*i];
            image[j*cinfo.output_width+i].g = buffer[3*i+1];
            image[j*cinfo.output_width+i].b = buffer[3*i+2];
			image[j*cinfo.output_width+i].a = 255;
		}
		j--;
    }
    
	// Finish
	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
	free(buffer);
    
    return(0);
}


