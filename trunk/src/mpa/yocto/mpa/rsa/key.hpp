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
                static const uint64_t MINIMUM_PRIME   = 3;
                static const uint64_t MINIMUM_MODULUS = MINIMUM_PRIME * MINIMUM_PRIME;
                
                const natural modulus;
                const size_t  obits; //!< (modulus-1).bits() :
                const size_t  ibits; //!< obits-1
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
                explicit PublicKey(const natural &Modulus,
                                   const natural &PublicExponent);
                PublicKey( const PublicKey &);
                
                //! generate a public key
                static PublicKey GenerateFrom( const natural &prime1, const natural &prime2, const natural &exponent );
                
                //! M.bits() <= ibits : (M^publicExponent) % modulus
                natural encode_with_pub( const natural &M ) const;
                
                //! C < modulus: (C^publicExponent) % modulus
                natural decode_with_pub( const natural &C ) const;
                
                
            private:
                YOCTO_DISABLE_ASSIGN(PublicKey);
            };
            
            class PrivateKey : public PublicKey
            {
            public:
                explicit PrivateKey(const natural &Modulus,
                                    const natural &PublicExponent,
                                    const natural &PrivateExponent,
                                    const natural &Prime1,
                                    const natural &Prime2,
                                    const natural &Exponent1,
                                    const natural &Exponent2,
                                    const natural &Coefficient
                                    );
                PrivateKey(const PrivateKey &other );
                
                virtual ~PrivateKey() throw();
                
                //! (C^privateExponent) % modulus: C < modulus
                natural decode_with_prv_( const natural &C ) const;
                
                //! M.bits() <= ibits : (M^privateExponent) % modulus
                natural encode_with_prv_( const natural &M ) const;
                
                
                
                
                //! Generate a private key
                static PrivateKey GenerateFrom(const natural &prime1, const natural &prime2, const natural &exponent );
                
                const natural privateExponent; //!< (1/publicExponent) %( (prime1-1) * (prime2-1) )
                const natural prime1;          //!< bigger  prime
                const natural prime2;          //!< smaller prime
                const natural exponent1;       //!< Chinese Remainder Theoreme exponent1   = d % (prime1-1)
                const natural exponent2;       //!< Chinese Remainder Theoreme exponent2   = d % (prime2-1)
                const natural coefficient;     //!< Chinese Remainder Theoreme coefficient = (1/prime2)  % prime1
                
            private:
                YOCTO_DISABLE_ASSIGN(PrivateKey);
            };
            
        }
        
    }
    
}


#endif
