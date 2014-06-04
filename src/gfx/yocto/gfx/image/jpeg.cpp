#include "yocto/gfx/image/jpeg.hpp"
extern "C"
{
#include "yocto/jpeg/jpeglib.h"
}

#include <cstring>
#include "yocto/ios/icstream.hpp"
#include "yocto/exception.hpp"
#include "yocto/memory/buffers.hpp"

#include <setjmp.h>

namespace yocto
{
    namespace gfx
    {
        jpeg_format:: jpeg_format() : image::format("JPEG")
        {
        }
        
        jpeg_format::~jpeg_format() throw()
        {
            
        }
        
        bool jpeg_format::lossless() const throw()
        {
            return false;
        }
        
        namespace
        {
            struct my_error_mgr
            {
                struct jpeg_error_mgr pub;	         /* "public" fields */
                jmp_buf               setjmp_buffer; /* for return to caller */
            };
            
            typedef struct my_error_mgr * my_error_ptr;
            
            
            METHODDEF(void)
            my_error_exit (j_common_ptr cinfo)
            {
                /* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
                my_error_ptr myerr = (my_error_ptr) cinfo->err;
                
                /* Always display the message. */
                /* We could postpone this until after returning, if we chose. */
                (*cinfo->err->output_message) (cinfo);
                
                /* Return control to the setjmp point */
                longjmp(myerr->setjmp_buffer, 1);
            }
            
            
            
            
        }
        
        
        
        
        void jpeg_format:: load(const string &filename) const
        {
            static const char fn[] = "jpeg::load";
            YOCTO_GIANT_LOCK();
            
            ios::icstream                 fp(filename);
            struct jpeg_decompress_struct cinfo;
            struct my_error_mgr           jerr;
            size_t   buflen = 0;
            JSAMPLE *buffer = 0;
            memset(&cinfo,0,sizeof(cinfo));
            memset(&jerr,0,sizeof(jerr));
            
            /* We set up the normal JPEG error routines, then override error_exit. */
            cinfo.err = jpeg_std_error(&jerr.pub);
            jerr.pub.error_exit = my_error_exit;
            
            /* Establish the setjmp return context for my_error_exit to use. */
            if (setjmp(jerr.setjmp_buffer)) {
                /* If we get here, the JPEG code has signaled an error.
                 * We need to clean up the JPEG object, close the input file, and return.
                 */
                jpeg_destroy_decompress(&cinfo);
                memory::kind<memory::global>::release_as<JSAMPLE>(buffer, buflen);
                throw exception("%s(failure)",fn);
            }
            
            /* Now we can initialize the JPEG decompression object. */
            jpeg_create_decompress(&cinfo);
            
            /* Step 2: specify data source (eg, a file) */
            jpeg_stdio_src(&cinfo, fp.__get());
            
            
            /* Step 3: read file parameters with jpeg_read_header() */
            
            (void) jpeg_read_header(&cinfo, TRUE);
            /* We can ignore the return value from jpeg_read_header since
             *   (a) suspension is not possible with the stdio data source, and
             *   (b) we passed TRUE to reject a tables-only JPEG file as an error.
             * See libjpeg.doc for more info.
             */
            
            /* Step 4: set parameters for decompression */
            
            /* In this example, we don't need to change any of the defaults set by
             * jpeg_read_header(), so we do nothing here.
             */
            
            /* Step 5: Start decompressor */
            
            (void) jpeg_start_decompress(&cinfo);
            /* We can ignore the return value since suspension is not possible
             * with the stdio data source.
             */
            
            const unit_t width  = cinfo.output_width;
            const unit_t height = cinfo.output_height;
            std::cerr << "width=" << width << ", height=" << height << std::endl;
            
            /* JSAMPLEs per row in output buffer */
            buflen = cinfo.output_width * cinfo.output_components;
            buffer = memory::kind<memory::global>::acquire_as<JSAMPLE>(buflen);
            
            /* Step 6: while (scan lines remain to be read) */
            /*           jpeg_read_scanlines(...); */
            
            /* Here we use the library's state variable cinfo.output_scanline as the
             * loop counter, so that we don't have to keep track ourselves.
             */
            unit_t j = height-1;
            while (cinfo.output_scanline < cinfo.output_height) {
                /* jpeg_read_scanlines expects an array of pointers to scanlines.
                 * Here the array is only one element long, but you could ask for
                 * more than one scanline at a time if that's more convenient.
                 */
                (void) jpeg_read_scanlines(&cinfo, &buffer, 1);
                /* Assume put_scanline_someplace wants a pointer and sample count. */
                for(unit_t i=0;i<width;++i)
                {
                    const unit_t   i3 = 3*i;
                    const JSAMPLE *b  = &buffer[i3];
                    const rgba_t C(b[0],b[1],b[2],0xff);
                }
                --j;
            }
            
            /* Step 7: Finish decompression */
            
            (void) jpeg_finish_decompress(&cinfo);
            /* We can ignore the return value since suspension is not possible
             * with the stdio data source.
             */
            
            /* Step 8: Release JPEG decompression object */
            
            /* This is an important step since it will release a good deal of memory. */
            memory::kind<memory::global>::release_as<JSAMPLE>(buffer, buflen);
            jpeg_destroy_decompress(&cinfo);
            
        }
    }
}