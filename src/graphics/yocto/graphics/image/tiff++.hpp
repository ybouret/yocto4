#ifndef YOCTO_TIFFPP_INCLUDED
#define YOCTO_TIFFPP_INCLUDED 1

#include "yocto/graphics/types.hpp"
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

            //! load rgba for current TIFF directory...
            uint32_t *load_rgba();

            void cleanup() throw();
            
        private:
            void     *handle;
            uint32_t *raster;
            size_t    dwords;

            YOCTO_DISABLE_COPY_AND_ASSIGN(I_TIFF);
        };
    }
}

#endif
