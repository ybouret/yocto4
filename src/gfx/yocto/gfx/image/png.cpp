#include "yocto/gfx/image/png.hpp"
#include "yocto/exceptions.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/ios/ocstream.hpp"

#include "yocto/png/png.h"
#include "yocto/code/round.hpp"
#include "yocto/ptr/auto.hpp"

namespace yocto
{
    namespace gfx
    {
        
        png_format:: ~png_format() throw()
        {
        }
        
        png_format:: png_format() : image::format("PNG")
        {
        }
        
        bool     png_format:: lossless() const throw()
        {
            return false;
        }
        
        
        
        namespace
        {
            
            class PNG_Mem
            {
            public:
                inline PNG_Mem() throw() :
                rows(0),
                wlen(0),
                wksp(0)
                {
                }
                
                inline ~PNG_Mem() throw()
                {
                    memory::kind<memory::global>::release(wksp, wlen);
                    rows=0;
                }
                
                png_bytep *rows;
                size_t     wlen;
                void      *wksp;
                
                void allocate( unit_t width, unit_t height )
                {
                    const size_t rows_offset = 0;
                    const size_t rows_length = sizeof(png_bytep) * height;
                    const unit_t row_bytes   = 4 * width;
                    const size_t data_offset = memory::align(rows_length+rows_offset);
                    const size_t data_length = height * row_bytes * sizeof(png_byte);
                    wlen = memory::align(data_offset+data_length);
                    wksp = memory::kind<memory::global>::acquire(wlen);
                    
                    uint8_t *addr = (uint8_t *)wksp;
                    rows = (png_bytep *)&addr[rows_offset];
                    uint8_t *data = &addr[data_offset];
                    for(unit_t i=0;i<height;++i)
                    {
                        rows[i] = (png_byte *)&data[i*row_bytes];
                    }
                    
                }
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(PNG_Mem);
            };
        }
        
        bitmap  *png_format:: load(const string          &filename,
                                   unit_t                 depth,
                                   image::put_rgba_proc   proc,
                                   const void            *args) const
        {
            static const char fn[] = "png::load";
            //assert(proc);
            YOCTO_GIANT_LOCK();
            
            //__________________________________________________________________
            //
            // open file
            //__________________________________________________________________
            
            ios::icstream fp(filename);
            
            //__________________________________________________________________
            //
            // read header
            //__________________________________________________________________
            {
                unsigned char header[8];
                size_t        done;
                fp.get(header, 8, done);
                if(done!=8)
                    throw exception("%s(missing header bytes)",fn);
                
                if (png_sig_cmp(header, 0, 8))
                    throw exception("%s(file is not recognized as PNG)", fn);
            }
            
            //__________________________________________________________________
            //
            // prepare to read
            //__________________________________________________________________
            png_structp png_ptr  = 0; png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
            png_infop   info_ptr = 0;
            
            png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
            if(!png_ptr)
                throw exception("%s(couldn't create PNG read struct)",fn);
            
            info_ptr = png_create_info_struct(png_ptr);
            if(!info_ptr)
            {
                png_destroy_read_struct(&png_ptr, NULL, NULL);
                throw exception("%s(could't create PNG info struct)",fn);
            }
            
            //__________________________________________________________________
            //
            //  Init I/O
            //__________________________________________________________________
            if (setjmp(png_jmpbuf(png_ptr)))
            {
                png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
                throw exception("%s(I/O failure)",fn);
            }
            
            png_init_io(png_ptr, fp.__get());
            png_set_sig_bytes(png_ptr, 8);
            
            png_read_info(png_ptr, info_ptr);
            
            const unit_t     width            = png_get_image_width(png_ptr, info_ptr);
            const unit_t     height           = png_get_image_height(png_ptr, info_ptr);
            const png_byte   color_type       = png_get_color_type(png_ptr, info_ptr);
            png_byte         bit_depth        = png_get_bit_depth(png_ptr, info_ptr);
            const int        number_of_passes = png_set_interlace_handling(png_ptr);
            
            std::cerr
            << "width=" << width
            << ", height=" << height
            << ", color_type=" << int(color_type)
            << ", bit_depth=" << int(bit_depth)
            << ", #passes=" << number_of_passes << std::endl;
            
            
            png_set_strip_16(png_ptr); //!< strip 16->8 bits
            png_set_expand(png_ptr);   //!< expand all to RGBA...
            png_read_update_info(png_ptr, info_ptr);
            bit_depth  = png_get_bit_depth(png_ptr, info_ptr);
            if(bit_depth!=8)
            {
                png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
                throw exception("%s(unstripped 16 bits!)",fn);
            }
            
            if(number_of_passes>1)
            {
                png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
                throw exception("%s(unhandled #passes=%d)",fn, number_of_passes);
            }
            
            
            auto_ptr<bitmap> bmp;
            PNG_Mem          mem;
            try
            {
                bmp.reset(new bitmap(depth,width,height));
                mem.allocate(width,height);
            }
            catch(...)
            {
                png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
                throw;
            }
            
            //__________________________________________________________________
            //
            // read file
            //__________________________________________________________________
            if (setjmp(png_jmpbuf(png_ptr)))
            {
                png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
                throw exception("%s(I/O failure)",fn);
            }
            
            png_read_image(png_ptr,mem.rows);
            
            //__________________________________________________________________
            //
            // processing...
            //__________________________________________________________________
            
            
            //__________________________________________________________________
            //
            // cleanup
            //__________________________________________________________________
            png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
            return bmp.yield();
        }
        
        
        void png_format:: save(const string        &filename,
                               const bitmap        &bmp,
                               image::get_rgba_proc proc,
                               const void          *args,
                               const char          *options) const
        {
        }
        
        
    }
    
}
