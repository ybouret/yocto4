#ifndef YOCTO_TIFF_IO_INCLUDED
#define YOCTO_TIFF_IO_INCLUDED 1

#include "yocto/string.hpp"
#include <tiffio.h>

namespace yocto
{
    
    namespace gfx
    {
        
        class tiff_reader
        {
        public:
            explicit tiff_reader(const string &the_path);
            virtual ~tiff_reader() throw();
            
            const string path;
            
        private:
            TIFF        *tif;
            YOCTO_DISABLE_COPY_AND_ASSIGN(tiff_reader);
        public:
            const size_t num;
        };
        
    }
    
}

#endif
