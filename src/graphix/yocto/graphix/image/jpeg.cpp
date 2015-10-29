#include "yocto/graphix/image/jpeg.hpp"
extern "C"
{
#include "yocto/jpeg/jpeglib.h"
}

#include <cstring>
#include "yocto/ios/icstream.hpp"
#include "yocto/ios/ocstream.hpp"
#include "yocto/exception.hpp"
#include "yocto/ptr/auto.hpp"
#include "yocto/fs/vfs.hpp"

#include <setjmp.h>

namespace yocto
{
    namespace graphix
    {
        jpeg_format:: jpeg_format() : image::format("JPEG")
        {
        }
        
        jpeg_format::~jpeg_format() throw()
        {
            
        }
        
        const char ** jpeg_format:: extensions() const throw()
        {
            static const char *__ext[] = { "jpg", "jpeg", 0 };
            return __ext;
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
        
        
        
        bitmap  * jpeg_format:: load(const string         &filename,
                                     unit_t                depth,
                                     image::put_rgba_proc  proc,
                                     void                  *args,
                                     const void           *) const
        {
            static const char fn[] = "jpeg::load";
            assert(proc);
            YOCTO_GIANT_LOCK();
            
            ios::icstream                 fp(filename);
            struct jpeg_decompress_struct cinfo;
            struct my_error_mgr           jerr;
            size_t   buflen = 0;
            JSAMPLE *buffer = 0;
            memset(&cinfo,0,sizeof(cinfo));
            memset(&jerr,0,sizeof(jerr));
            
            //__________________________________________________________________
            //
            // We set up the normal JPEG error routines,
            // then override error_exit
            //__________________________________________________________________
            cinfo.err = jpeg_std_error(&jerr.pub);
            jerr.pub.error_exit = my_error_exit;
            
            //__________________________________________________________________
            //
            // Establish the setjmp return context for my_error_exit to use.
            //__________________________________________________________________
            if (setjmp(jerr.setjmp_buffer)) {
                /* If we get here, the JPEG code has signaled an error.
                 * We need to clean up the JPEG object, close the input file, and return.
                 */
                jpeg_destroy_decompress(&cinfo);
                memory::kind<memory::global>::release_as<JSAMPLE>(buffer, buflen);
                throw exception("%s(failure)",fn);
            }
            
            //__________________________________________________________________
            //
            // Now we can initialize the JPEG decompression object
            //__________________________________________________________________
            jpeg_create_decompress(&cinfo);
            
            //__________________________________________________________________
            //
            // Step 2: specify data source (eg, a file)
            //__________________________________________________________________
            jpeg_stdio_src(&cinfo, fp.__get());
            
            
            //__________________________________________________________________
            //
            // Read file parameters with jpeg_read_header()
            //__________________________________________________________________
            
            
            (void) jpeg_read_header(&cinfo, TRUE);
            /* We can ignore the return value from jpeg_read_header since
             *   (a) suspension is not possible with the stdio data source, and
             *   (b) we passed TRUE to reject a tables-only JPEG file as an error.
             * See libjpeg.doc for more info.
             */
            
            //__________________________________________________________________
            //
            // Step 5: Start decompressor
            //__________________________________________________________________
            
            (void) jpeg_start_decompress(&cinfo);
            /* We can ignore the return value since suspension is not possible
             * with the stdio data source.
             */
            
            try
            {
                const unit_t width  = cinfo.output_width;
                const unit_t height = cinfo.output_height;
                //std::cerr << "width=" << width << ", height=" << height << std::endl;
                if(width<=0||height<=0)
                    throw exception("%s(invalid witdh/height)", fn);
                
                if(cinfo.output_components!=3)
                    throw exception("%s(unsupported image depth=%d)",fn,int(cinfo.output_components));
                
                auto_ptr<bitmap> bmp( new bitmap(depth,width,height) );
                
                buflen = cinfo.output_width * cinfo.output_components;
                buffer = memory::kind<memory::global>::acquire_as<JSAMPLE>(buflen);
                
                unit_t j = height-1;
                while (cinfo.output_scanline < cinfo.output_height)
                {
                    (void) jpeg_read_scanlines(&cinfo, &buffer, 1);
                    //surface::row &r = (*surf)[j];
                    uint8_t *p = static_cast<uint8_t*>(bmp->get_line(j));
                    for(unit_t i=0;i<width;++i)
                    {
                        const unit_t   i3 = 3*i;
                        const JSAMPLE *b  = &buffer[i3];
                        const RGBA    C(b[0],b[1],b[2],0xff);
                        proc(p,C,args);
                        p += depth;
                    }
                    --j;
                }
                
                /* Step 7: Finish decompression */
                
                (void) jpeg_finish_decompress(&cinfo);
                /* We can ignore the return value since suspension is not possible
                 * with the stdio data source.
                 */
                
                memory::kind<memory::global>::release_as<JSAMPLE>(buffer, buflen);
                jpeg_destroy_decompress(&cinfo);
                return bmp.yield();
            }
            catch(...)
            {
                memory::kind<memory::global>::release_as<JSAMPLE>(buffer, buflen);
                jpeg_destroy_decompress(&cinfo);
                throw;
            }
            
        }
        
        void jpeg_format:: save(const string        &filename,
                                const bitmap        &bmp,
                                image::get_rgba_proc proc,
                                void                  *args,
                                const void          *options) const
        {
            static const char fn[] = "jpeg::save";
            assert(proc);
            YOCTO_GIANT_LOCK();
            
            ios::ocstream fp(filename,false);
            
            struct jpeg_compress_struct cinfo;
            struct my_error_mgr         jerr;
            memset(&cinfo,0,sizeof(cinfo));
            memset(&jerr,0,sizeof(jerr));
            size_t   buflen = 0;
            JSAMPLE *buffer = 0;
            //__________________________________________________________________
            //
            // We set up the normal JPEG error routines,
            // then override error_exit
            //__________________________________________________________________
            cinfo.err = jpeg_std_error(&jerr.pub);
            jerr.pub.error_exit = my_error_exit;
            
            //__________________________________________________________________
            //
            // We create the compression setup
            //__________________________________________________________________
            jpeg_create_compress(&cinfo);
            
            
            
            //__________________________________________________________________
            //
            // Establish the setjmp return context for my_error_exit to use.
            //__________________________________________________________________
            if (setjmp(jerr.setjmp_buffer)) {
                /* If we get here, the JPEG code has signaled an error.
                 * We need to clean up the JPEG object, close the input file, and return.
                 */
                jpeg_destroy_compress(&cinfo);
                memory::kind<memory::global>::release_as<JSAMPLE>(buffer, buflen);
                throw exception("%s(failure)",fn);
            }
            
            //__________________________________________________________________
            //
            // set the output
            //__________________________________________________________________
            jpeg_stdio_dest(&cinfo, fp.__get());
            
            
            cinfo.image_width  = bmp.w; 	/* image width and height, in pixels */
            cinfo.image_height = bmp.h;
            cinfo.input_components = 3;		/* # of color components per pixel */
            cinfo.in_color_space = JCS_RGB; 	/* colorspace of input image */
            
            /* Now use the library's routine to set default compression parameters.
             * (You must set at least cinfo.in_color_space before calling this,
             * since the defaults depend on the source color space.)
             */
            jpeg_set_defaults(&cinfo);
            
            int quality = 70;
            /* Now you can set any non-default parameters you wish to.
             * Here we just illustrate the use of quality (quantization table) scaling:
             */
            jpeg_set_quality(&cinfo, quality, TRUE /* limit to baseline-JPEG values */);
            
            /* Step 4: Start compressor */
            
            /* TRUE ensures that we will write a complete interchange-JPEG file.
             * Pass TRUE unless you are very sure of what you're doing.
             */
            jpeg_start_compress(&cinfo, TRUE);
            /* Here we use the library's state variable cinfo.next_scanline as the
             * loop counter, so that we don't have to keep track ourselves.
             * To keep things simple, we pass one scanline per call; you can pass
             * more if you wish, though.
             */
            
            try
            {
                
                buflen = bmp.w * 3;
                buffer = memory::kind<memory::global>::acquire_as<JSAMPLE>(buflen);
                JSAMPROW row_pointer[1] = { buffer };

                unit_t       j     = bmp.h - 1;
                const unit_t depth = bmp.depth;
                const unit_t width = bmp.w;
                while (cinfo.next_scanline < cinfo.image_height) {
                    /* jpeg_write_scanlines expects an array of pointers to scanlines.
                     * Here the array is only one element long, but you could pass
                     * more than one scanline at a time if that's more convenient.
                     */
                    const uint8_t *p = static_cast<const uint8_t*>(bmp.get_line(j));
                    for(unit_t i=0;i<width;++i)
                    {
                        JSAMPLE       *b  = &buffer[3*i];
                        const RGBA     C  = proc(p,args);
                        b[0] = C.r;
                        b[1] = C.g;
                        b[2] = C.b;
                        p += depth;
                    }
                    (void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
                    --j;
                }
                
                /* Step 6: Finish compression */
                
                jpeg_finish_compress(&cinfo);
                /* After finish_compress, we can close the output file. */
                
                /* Step 7: release JPEG compression object */
                
                /* This is an important step since it will release a good deal of memory. */
                jpeg_destroy_compress(&cinfo);
                memory::kind<memory::global>::release_as<JSAMPLE>(buffer, buflen);
                
                // success
            }
            catch(...)
            {
                
                jpeg_destroy_compress(&cinfo);
                memory::kind<memory::global>::release_as<JSAMPLE>(buffer, buflen);
                throw;
            }
            
        }
    }
}

