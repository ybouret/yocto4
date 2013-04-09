#ifndef YOCTO_BZ2STREAM_INCLUDED
#define YOCTO_BZ2STREAM_INCLUDED 1


#include "yocto/ios/icstream.hpp"
#include "yocto/ios/ocstream.hpp"
#include "yocto/bz2/bzlib.h"

namespace yocto
{
    
    namespace bzlib
    {
        class bz2stream
        {
        public:
            
            int      bzerror;
            virtual ~bz2stream() throw();
            
        protected:
            explicit bz2stream() throw(); //!< bzfile to NULL
            BZFILE *bzfile;
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(bz2stream);
            
        };
     
        class ibz2stream : public bz2stream, public ios::istream
        {
        public:
            explicit ibz2stream( const string &filename );
            virtual ~ibz2stream() throw();
            
        private:
            ios::icstream fp;
            YOCTO_DISABLE_COPY_AND_ASSIGN(ibz2stream);
            
        public:
            int *last_close;
        };
        
        
    }
    
}

#endif
