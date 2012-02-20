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

            string to_string();
            
        protected:
            explicit codec() throw();
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(codec);
        };
    }
    
}

#endif
