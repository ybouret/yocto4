#ifndef YOCTO_PACK_Q_CODEC_INCLUDED
#define YOCTO_PACK_Q_CODEC_INCLUDED

#include "yocto/ios/codec.hpp"
#include "yocto/sequence/list.hpp"

namespace yocto
{
    
    namespace pack
    {
        //! implement the istream part
        class q_codec : public ios::codec
        {
        public:
            virtual ~q_codec() throw();
            
            virtual bool query( char &C );
            virtual void store( char  C );
            
            
        protected:
            explicit q_codec() throw();
            list<char> Q;

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(q_codec);
        };
    }
    
}


#endif
