#include "yocto/graphics/image/png.hpp"
#include "yocto/exceptions.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/ios/ocstream.hpp"

#include "yocto/png/png.h"
#include "yocto/code/round.hpp"
#include "yocto/ptr/auto.hpp"

namespace yocto
{
    namespace graphics
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
        
        const char ** png_format:: extensions() const throw()
        {
            static const char *__ext[] = { "png", 0 };
            return __ext;
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
                
                void allocate( unit_t width, unit_t height, unit_t num_channels )
                {
                    const size_t rows_offset = 0;
                    const size_t rows_length = sizeof(png_bytep) * height;
                    
                    const size_t data_offset = memory::align(rows_length+rows_offset);
                    const unit_t row_bytes   = num_channels * width;
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
                                   rgba2data             &proc,
                                   const void            *) const
        {
            static const char fn[] = "png::load";
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
            png_structp png_ptr  = 0;
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
            //const png_byte   color_type       = png_get_color_type(png_ptr, info_ptr);
            png_byte         bit_depth        = png_get_bit_depth(png_ptr, info_ptr);
            const int        number_of_passes = png_set_interlace_handling(png_ptr);
            
            
            png_set_strip_16(png_ptr); //!< strip 16->8 bits
            png_set_packing(png_ptr);  //!< expand channels
            png_set_expand(png_ptr);   //!< expand all to RGBA...
            png_read_update_info(png_ptr, info_ptr);
            const unit_t num_channels = png_get_channels(png_ptr, info_ptr);
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
            
            switch(num_channels)
            {
                case 1:
                case 3:
                case 4:
                    break;
                    
                default:
                    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
                    throw exception("%s(unhandled #channels=%d)",fn, int(num_channels));
            }
            
            
            auto_ptr<bitmap> bmp;
            PNG_Mem          mem;
            try
            {
                bmp.reset(new bitmap(depth,width,height));
                mem.allocate(width,height,num_channels);
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
            unit_t y=height;
            for(unit_t j=0;j<height;++j)
            {
                const png_byte *q = mem.rows[j];
                uint8_t        *p = static_cast<uint8_t*>(bmp->get_line(--y));
                for(unit_t i=0;i<width;++i,q += num_channels, p += depth)
                {
                    switch(num_channels)
                    {
                        case 1: {
                            const RGBA C(q[0],q[0],q[0],0xff);
                            proc(p,C); }
                            break;
                        case 3: {
                            const RGBA C( q[0], q[1], q[2], 0xff);
                            proc(p,C); }
                            break;
                            
                        case 4: {
                            const RGBA C( q[0], q[1], q[2], q[3]);
                            proc(p,C); }
                            break;
                            
                        default:
                            // never get here
                            ;
                    }
                }
            }
            
            //__________________________________________________________________
            //
            // cleanup
            //__________________________________________________________________
            png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
            return bmp.yield();
        }
        
        
        void png_format:: save(const string        &filename,
                               const bitmap        &bmp,
                               data2rgba           &proc,
                               const void          *options) const
        {
            static const char fn[] = "png::save";
            
            //__________________________________________________________________
            //
            // open file
            //__________________________________________________________________
            ios::ocstream fp(filename,false);
            
            //__________________________________________________________________
            //
            // parse options
            //__________________________________________________________________
            const string opt((const char*)options);
            bool use_alpha = false;
            if( options )
            {
                if(opt=="alpha")
                {
                    use_alpha = true;
                }
            }
            const unit_t num_channels = use_alpha ? 4 : 3;
            
            
            //__________________________________________________________________
            //
            // prepare PNG
            //__________________________________________________________________
            png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
            
            if (!png_ptr)
            {
                throw exception("%s(can't init PNG)",fn);
            }
            
            png_infop   info_ptr = png_create_info_struct(png_ptr);
            if (!info_ptr)
            {
                png_destroy_write_struct(&png_ptr,NULL);
                throw exception("%s(can't init PNG info)",fn);
            }
            
            if (setjmp(png_jmpbuf(png_ptr)))
            {
                png_destroy_write_struct(&png_ptr,&info_ptr);
                throw exception("%s(init_io error)",fn);
            }
            
            png_init_io(png_ptr, fp.__get());
            
            //__________________________________________________________________
            //
            // write header
            //__________________________________________________________________
            if (setjmp(png_jmpbuf(png_ptr)))
            {
                png_destroy_write_struct(&png_ptr,&info_ptr);
                throw exception("%s(header error)",fn);
            }
            
            const unit_t width  = bmp.w;
            const unit_t height = bmp.h;
            png_set_IHDR(png_ptr, info_ptr,
                         width,
                         height,
                         8,
                         use_alpha ? PNG_COLOR_TYPE_RGB_ALPHA : PNG_COLOR_TYPE_RGB,
                         PNG_INTERLACE_NONE,
                         PNG_COMPRESSION_TYPE_BASE,
                         PNG_FILTER_TYPE_BASE);
            
            png_write_info(png_ptr, info_ptr);

            png_set_compression_level(png_ptr, 6);
            

            PNG_Mem mem;
            try
            {
                mem.allocate(width, height, num_channels);
            }
            catch(...)
            {
                png_destroy_write_struct(&png_ptr,&info_ptr);
                throw;
            }
            //__________________________________________________________________
            //
            // write bytes
            //__________________________________________________________________
            if (setjmp(png_jmpbuf(png_ptr)))
            {
                png_destroy_write_struct(&png_ptr,&info_ptr);
                throw exception("%s(write_image error)", fn);
            }
            
            unit_t       y     = height;
            const unit_t depth = bmp.depth;
            
            for(unit_t j=0;j<height;++j)
            {
                const uint8_t *p = (const uint8_t *)(bmp.get_line(--y));
                png_byte      *q = mem.rows[j];
                for(unit_t i=0;i<width;++i, p+=depth, q += num_channels)
                {
                    const RGBA C = proc(p);
                    q[0] = C.r;
                    q[1] = C.g;
                    q[2] = C.b;
                    if(use_alpha)
                        q[3] = C.a;
                }
            }
            
            png_write_image(png_ptr, mem.rows);
            
            
            //__________________________________________________________________
            //
            // end write
            //__________________________________________________________________
            if (setjmp(png_jmpbuf(png_ptr)))
            {
                png_destroy_write_struct(&png_ptr,&info_ptr);
                throw exception("%s(write_end error)",fn);
            }
            
            png_write_end(png_ptr, NULL);
            
            //__________________________________________________________________
            //
            // normal return
            //__________________________________________________________________
            png_destroy_write_struct(&png_ptr,&info_ptr);
            
        }
        
    }
    
}
