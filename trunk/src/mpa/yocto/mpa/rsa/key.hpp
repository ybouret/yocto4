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
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(Key);
            };
            
            struct as_primes_t {};
            extern const as_primes_t as_primes;
            
            class PublicKey : public Key
            {
            public:
                const natural publicExponent;
                
                virtual ~PublicKey() throw();
                explicit PublicKey( const natural &Modulus, const natural &PublicExponent);
                explicit PublicKey( const natural &prime1, const natural &prime2, const as_primes_t &);
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(PublicKey);
            };
            
        }
        
    }
    
}


#endif
