#ifndef YOCTO_TIFFPP_INCLUDED
#define YOCTO_TIFFPP_INCLUDED 1

#include "yocto/string.hpp"

namespace yocto
{
    namespace graphics
    {
        class I_TIFF
        {
        public:
          
                
            explicit I_TIFF(const string &filename);
            virtual ~I_TIFF() throw();

            bool   ReadDirectory();
            size_t CountDirectories();
            void   SetDirectory(const size_t n);
            int    GetWidth();
            int    GetHeight();

            class Raster
            {
            public:
                uint32_t    *data;
                const size_t size;
                Raster() throw();
                ~Raster() throw();
                void cleanup() throw();
                void startup(const size_t n);
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(Raster);
            };
            
            void ReadRBGAImage(Raster &raster);
            
        private:
            void     *handle;
            
            YOCTO_DISABLE_COPY_AND_ASSIGN(I_TIFF);
        };
    }
}

#endif
