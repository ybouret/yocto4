#ifndef YOCTO_MPA_RSA_KEY_INCLUDED 
#define YOCTO_MPA_RSA_KEY_INCLUDED 1

#include "yocto/mpa/natural.hpp"


namespace yocto
{
    namespace mpa
    {
        
        namespace RSA
        {
            
            class Key : public object
            {
            public:
                const natural modulus;
                
                virtual ~Key() throw();
                
            protected:
                explicit Key( const natural &Modulus);
                Key(const Key &);
                
            private:
                YOCTO_DISABLE_ASSIGN(Key);
            };
            
         
            
            class PublicKey : public Key
            {
            public:
                const natural publicExponent;
                
                virtual ~PublicKey() throw();
                explicit PublicKey( const natural &Modulus, const natural &PublicExponent);
                PublicKey( const PublicKey &);
                
                static PublicKey GenerateFrom( const natural &prime1, const natural &prime2, const natural &exponent );
                
            private:
                YOCTO_DISABLE_ASSIGN(PublicKey);
            };
            
        }
        
    }
    
}


#endif
