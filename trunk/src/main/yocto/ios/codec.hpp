#ifndef YOCTO_IOS_CODEC_INCLUDED
#define YOCTO_IOS_CODEC_INCLUDED 1

#include "yocto/ios/istream.hpp"
#include "yocto/ios/ostream.hpp"

namespace yocto
{
    namespace ios
    {
        class codec : public istream, public ostream
        {
        public:
            virtual ~codec() throw();
            
            //! reset codec state
            virtual void reset() throw() = 0;
            
            //! must be called after a flush()
            string to_string();
            
            //! block-wise behavior: reset, write, flush
            string to_string( const void *data, size_t size );
            string to_string( const memory::ro_buffer &data );
            
        protected:
            explicit codec() throw();
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(codec);
        };
        
    }
    
}

#endif
