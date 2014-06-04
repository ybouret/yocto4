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
            
            
            
            class DecompressJPEG
            {
            public:
                inline DecompressJPEG( struct jpeg_decompress_struct *user_cinfo ) :
                cinfo(user_cinfo)
                {
                    assert(cinfo);
                    if(!jpeg_start_decompress(cinfo))
                        throw exception("jpeg_start_decompress failure");
                }
                
                inline ~DecompressJPEG() throw()
                {
                    jpeg_finish_decompress(cinfo);
                    jpeg_destroy_decompress(cinfo);
                }
                
                
            private:
                struct jpeg_decompress_struct *cinfo;
                YOCTO_DISABLE_COPY_AND_ASSIGN(DecompressJPEG);
            };
            
            class RowJPEG
            {
            public:
                size_t   buflen;
                JSAMPLE *buffer;
                
                inline RowJPEG(int row_stride) :
                buflen( row_stride ),
                buffer( memory::kind<memory::global>::acquire_as<JSAMPLE>(buflen) )
                {
                }
                
                inline ~RowJPEG() throw()
                {
                    memory::kind<memory::global>::release_as<JSAMPLE>(buffer, buflen);
                }
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(RowJPEG);
            };
            
        }
        
        
        
        
        void jpeg_format:: load(const string &filename) const
        {
            static const char fn[] = "jpeg::load";
            YOCTO_GIANT_LOCK();
            
            ios::icstream                 fp(filename);
            struct jpeg_decompress_struct cinfo;
            struct my_error_mgr           jerr;
            memset( &cinfo, 0, sizeof(cinfo) );
            memset( &jerr,  0, sizeof(jerr)  );
            
            //__________________________________________________________________
            //
            // Error handler
            //__________________________________________________________________
            cinfo.err           = jpeg_std_error(&jerr.pub);
            jerr.pub.error_exit = my_error_exit;
            
            /* Establish the setjmp return context for my_error_exit to use. */
            if( setjmp(jerr.setjmp_buffer) )
            {
                /* If we get here, the JPEG code has signaled an error.
                 * We need to clean up the JPEG object, close the input file, and return.
                 */
                jpeg_destroy_decompress(&cinfo);
                throw exception("libjpeg error");
            }
            
            
            jpeg_create_decompress(&cinfo);
            jpeg_stdio_src(&cinfo, fp.__get());
            
            //__________________________________________________________________
            //
            // Read header
            //__________________________________________________________________
            std::cerr << "reading header" << std::endl;
            (void)jpeg_read_header(&cinfo, TRUE);
            
            //__________________________________________________________________
            //
            // start decompression
            //__________________________________________________________________
            DecompressJPEG __start(&cinfo);
            
            const unit_t width  = cinfo.output_width;
            const unit_t height = cinfo.output_height;
            std::cerr << "w=" << width << ", h=" << height << std::endl;
            if(width<=0||height<=0)
            {
                throw exception("invalid width/height");
            }
            if (cinfo.output_components != 3)
                throw exception("%s(unsupported output_components=%d)", fn, cinfo.output_components);
            
            //__________________________________________________________________
            //
            // buffer for one scan line
            //__________________________________________________________________
            const size_t row_stride = width * cinfo.output_components;
            RowJPEG    __row(row_stride);
            JSAMPLE   *buffer = __row.buffer;
            
            
            //__________________________________________________________________
            //
            // read and process
            //__________________________________________________________________
            unit_t j = height-1;
            while (cinfo.output_scanline < cinfo.output_height)
            {
                if(jpeg_read_scanlines(&cinfo,&buffer,1)!=1)
                {
                    
                }
                for(unit_t i=0;i<cinfo.output_width;++i)
                {
                    const unit_t i3 = 3*i;
                    const rgba_t C(i3,i3+1,i3+2,0xff);
                    /*
                     image[j*cinfo.output_width+i].r = buffer[3*i];
                     image[j*cinfo.output_width+i].g = buffer[3*i+1];
                     image[j*cinfo.output_width+i].b = buffer[3*i+2];
                     image[j*cinfo.output_width+i].a = 255;
                     */
                }
                --j;
            }
            
            
        }
    }
}