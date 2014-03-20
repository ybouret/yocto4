#include "yocto/mpa/rsa/key.hpp"
#include "yocto/exception.hpp"


namespace yocto
{
    
    namespace mpa
    {
        
        namespace RSA
        {
            
            //__________________________________________________________________
            //
            // Key
            //__________________________________________________________________
            
            Key:: ~Key() throw()
            {
                modulus.__clr();
            }
            
            static inline size_t __obits( const natural &n )
            {
                if( n < Key::MINIMUM_MODULUS )
                    throw exception("invalid rsa modulus");
                const mpn m = n - 1;
                assert(m.bits()>1);
                return m.bits();
            }
            
            Key:: Key( const natural &Modulus ) :
            modulus( Modulus  ),
            obits( __obits(modulus) ),
            ibits( obits-1          )
            {
            }
            
            Key:: Key( const Key & other ) :
            modulus( other.modulus ),
            obits( other.obits ),
            ibits( other.ibits )
            {
            }
            
            
            //__________________________________________________________________
            //
            // PublicKey
            //__________________________________________________________________
            PublicKey:: ~PublicKey() throw()
            {
                publicExponent.__clr();
            }
            
            PublicKey:: PublicKey(const natural &Modulus,
                                  const natural &PublicExponent) :
            Key(Modulus),
            publicExponent(PublicExponent)
            {
                
            }
            
            PublicKey:: PublicKey( const PublicKey &other ) :
            Key( other ),
            publicExponent( other.publicExponent )
            {
            }
            
            
            
            static inline
            void __pubExp( const natural &phi, natural &e )
            {
                assert(phi>=4);
                if(e<3) e = 3;
                
                while( ! mpn::are_coprime(phi, e) )
                {
                    assert(e>=2);
                    --e;
                    if(e<2)
                    {
                        throw exception("RSA: Public Exponent Failure");
                    }
                }
                
            }
            
            PublicKey PublicKey:: GenerateFrom( const natural &prime1, const natural &prime2, const natural &exponent )
            {
                if( prime1 < MINIMUM_PRIME || prime2 < MINIMUM_PRIME )
                    throw exception("RSA::PublicKey::GenerateFrom(Invalid Primes)");
                
                const natural n   = prime1 * prime2;          assert(n>=MINIMUM_MODULUS);
                const natural phi = (prime1-1) * (prime2-1);
                assert(phi>=4);
                natural e = exponent;
                __pubExp(phi, e);
                
                return PublicKey(n,e);
            }
            
            
            natural PublicKey:: encode_with_pub( const natural &M ) const
            {
                // check message length
                if(M.bits()>ibits)
                    throw exception("RSA:PublicKey:: encode_with_pub(Message Too Long)");
                return mpn::mod_exp(M, publicExponent, modulus);
            }
            
            natural PublicKey::  decode_with_pub( const natural &C ) const
            {
                if( C >= modulus)
                    throw exception("RSA:PublicKey:: decode_with_pub(Code Too Long)");
                return mpn::mod_exp(C, publicExponent, modulus);
            }
            
            
            //__________________________________________________________________
            //
            // PrivateKey
            //__________________________________________________________________
            PrivateKey:: PrivateKey(const natural &Modulus,
                                    const natural &PublicExponent,
                                    const natural &PrivateExponent,
                                    const natural &Prime1,
                                    const natural &Prime2,
                                    const natural &Exponent1,
                                    const natural &Exponent2,
                                    const natural &Coefficient
                                    ) :
            PublicKey(Modulus,PublicExponent),
            privateExponent(PrivateExponent),
            prime1(Prime1),
            prime2(Prime2),
            exponent1(Exponent1),
            exponent2(Exponent2),
            coefficient(Coefficient)
            {
                // should check ?
                
            }
            
            
            PrivateKey:: ~PrivateKey() throw()
            {
                privateExponent.__clr();
                prime1.__clr();
                prime2.__clr();
                exponent1.__clr();
                exponent2.__clr();
                coefficient.__clr();
            }
            
            
            PrivateKey:: PrivateKey( const PrivateKey &other ) :
            PublicKey(other),
            privateExponent(other.privateExponent),
            prime1(other.prime1),
            prime2(other.prime2),
            exponent1(other.exponent1),
            exponent2(other.exponent2),
            coefficient(other.coefficient)
            {
            }
            
            
            natural PrivateKey:: decode_with_prv_( const natural &C ) const
            {
                if( C >= modulus )
                    throw exception("RSA::PivateKey:: decode_with_prv_(Code Too Long)");
                return mpn::mod_exp(C, privateExponent, modulus);
            }
            
            natural PrivateKey:: decode_with_prv( const natural &C ) const
            {
                if( C >= modulus )
                    throw exception("RSA::PivateKey:: decode_with_prv(Code Too Long)");
                return CRT(C);
            }

            
            natural PrivateKey::  encode_with_prv_( const natural &M ) const
            {
                if(M.bits()>ibits)
                    throw exception("RSA:PrivateKey:: encode_with_prv_(Message Too Long)");
                return mpn::mod_exp(M,privateExponent,modulus);
            }
            
            natural PrivateKey::  encode_with_prv( const natural &M ) const
            {
                if(M.bits()>ibits)
                    throw exception("RSA:PrivateKey:: encode_with_prv_(Message Too Long)");
                return CRT(M);
            }
            
            
            PrivateKey PrivateKey:: GenerateFrom(const natural &P1, const natural &P2, const natural &exponent )
            {
                
                mpn prime1 = P1;
                mpn prime2 = P2;
                if(prime1<prime2)
                    prime1.xch(prime2);
                
                if( prime1 < MINIMUM_PRIME || prime2 < MINIMUM_PRIME )
                    throw exception("RSA::PrivateKey::GenerateFrom(Invalid Primes)");
                
                const natural n   = prime1 * prime2;          assert(n>=MINIMUM_MODULUS);
                
                const natural p1m = prime1-1;
                const natural p2m = prime2-1;
                const natural phi = p1m * p2m;
                assert(phi>=4);
                natural e = exponent;
                __pubExp(phi, e);
                
                const natural d  = mpn::mod_inv(e,phi);
                const natural e1 = d % p1m;
                const natural e2 = d % p2m;
                const natural cf = mpn::mod_inv(prime2, prime1);
                
                return PrivateKey(n, e, d, prime1, prime2, e1, e2, cf);
            }
            
            natural PrivateKey:: CRT( const natural &C ) const
            {
                mpn       M1 = mpn::mod_exp(C,exponent1,prime1);
                const mpn M2 = mpn::mod_exp(C,exponent2,prime2);
                while(M1<M2) M1 += prime1;
                M1 -= M2;
                M1 *= coefficient;
                return M2 + (M1%prime1) * prime2;
            }

            
        }
        
        
    }
    
}
