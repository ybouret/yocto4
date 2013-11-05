#ifndef YOCTO_GFX_FORMAT_INCLUDED
#define YOCTO_GFX_FORMAT_INCLUDED 1

#include "yocto/gfx/bitmap.hpp"

namespace yocto
{
    
    namespace gfx
    {
        class format : public object, public counted
        {
        public:
            const string name;
            
            virtual ~format() throw();
            const string &key() const throw();
            
            virtual void save(const string &filename,
                              const bitmap &bmp,
                              addr2rgba    &proc,
                              const char   *options) const = 0;
            
        protected:
            explicit format( const char *id );
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(format);
        };
    }
    
}

#endif

