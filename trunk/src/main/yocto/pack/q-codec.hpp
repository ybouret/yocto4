#ifndef YOCTO_PACK_Q_CODEC_INCLUDED
#define YOCTO_PACK_Q_CODEC_INCLUDED

#include "yocto/ios/codec.hpp"
#include "yocto/sequence/list.hpp"

namespace yocto
{
    
    namespace pack
    {
        //! implement the istream part
        class Qcodec : public ios::codec
        {
        public:
            virtual ~Qcodec() throw();
            
            virtual bool query( char &C );
            virtual void store( char  C );
            
        protected:
            list<char> Q;
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(Qcodec);
        };
    }
    
}


#endif
