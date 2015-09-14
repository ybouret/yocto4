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
            typedef list<char> list_type;
            typedef list_type::const_iterator const_iterator;

            virtual ~q_codec() throw();
            
            virtual bool query( char &C );
            virtual void store( char  C );

            const_iterator begin() const throw();
            const_iterator end()   const throw();

            void clear() throw();

        protected:
            explicit q_codec() throw();
            list_type Q;

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(q_codec);
        };
    }
    
}


#endif
