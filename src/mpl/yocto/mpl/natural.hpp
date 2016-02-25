#ifndef YOCTO_MPL_NATURAL_INCLUDED
#define YOCTO_MPL_NATURAL_INCLUDED 1

#include "yocto/mpl/types.hpp"
#include "yocto/memory/buffers.hpp"
#include "yocto/code/endian.hpp"
#include "yocto/code/bswap.hpp"
#include "yocto/xnumeric.hpp"
#include "yocto/ios/bitio.hpp"
#include "yocto/ios/ostream.hpp"
#include "yocto/ios/istream.hpp"

#include <iosfwd>

namespace yocto
{
    namespace mpl
    {

#define YOCTO_CHECK_MPN(X)     \
assert( (X).byte != 0 );       \
assert( (X).maxi >  0 );       \
assert( (X).size <= (X).maxi );\
assert( ! ( ((X).size>0) && (X).byte[(X).size-1] <= 0 ) )

#define YOCTO_MPN_DECL(RET,OP,CALL) \
inline friend RET operator OP (const natural &lhs, const natural &rhs) \
{ return CALL(lhs.byte,lhs.size,rhs.byte,rhs.size); } \
inline friend RET operator OP (const natural &lhs, word_t rhs ) \
{ const size_t n = prepare(rhs); return CALL(lhs.byte,lhs.size,&rhs,n); } \
inline friend RET operator OP (word_t lhs, const natural &rhs) \
{ const size_t n = prepare(lhs); return CALL(&lhs,n,rhs.byte,rhs.size); }

#define YOCTO_MPN_DECL_SELF(OP1,OP) \
inline natural & operator OP1 ( const natural &rhs ) { natural tmp = (*this) + rhs; xch(tmp); return *this; }\
inline natural & operator OP1 ( const word_t   rhs ) { natural tmp = (*this) + rhs; xch(tmp); return *this; }

        class integer;

        class natural : public memory::ro_buffer
        {
        public:
            //__________________________________________________________________
            //
            //
            // management functions
            //
            //__________________________________________________________________
            natural();                   //!< this is zero
            natural(const word_t);       //!< any word
            natural(const natural &);    //!< copy constructor
            natural(const void *buf, const size_t len);

            //!< default n bytes, MUST be updated
            natural(size_t n, const as_capacity_t &);

            //! no throw swaps
            inline void xch( natural &other) throw()
            {
                cswap(maxi,other.maxi);
                cswap(size,other.size);
                cswap(byte,other.byte);
            }

            //! assign operator
            inline natural & operator=(const natural &other)
            {
                natural tmp(other); xch(tmp); return *this;
            }


            //! assign a word, optimized, no throw
            inline natural & operator=(const word_t n) throw()
            {
                assert(maxi>=sizeof(word_t));
                size = sizeof(word_t);
                *(word_t *)byte = swap_le(n);
                update();
                YOCTO_CHECK_MPN(*this);
                return *this;
            }

            //! destructor
            virtual ~natural() throw();


            static inline void update(const uint8_t *b, size_t &n)
            {
                assert(b!=0);
                while(n>0)
                {
                    const size_t imsb = n-1;
                    if(b[imsb]>0) break;
                    n=imsb;
                }
            }

            static inline size_t prepare(word_t &w)
            {
                size_t n = sizeof(word_t);
                w        = swap_le(w);
                update( (uint8_t *)&w, n );
                return n;
            }

            //! try to decrease size
            inline void update() throw() {  update(byte,size);  }

            //! size=maxi and update
            inline void rescan() throw() {  update(byte,(size=maxi)); }

            //! ro_buffer interface
            virtual size_t length() const throw() { return size; }

            //__________________________________________________________________
            //
            //
            // output
            //
            //__________________________________________________________________
            void display( std::ostream & ) const;
            
            inline friend std::ostream & operator<<( std::ostream &os, const natural &n )
            {
                n.display(os);
                return os;
            }

            //! least significant word
            inline word_t lsw() const throw()
            {
                word_t         ans = 0;
                const uint8_t *b   = byte+size;
                for(size_t i=min_of<size_t>(sizeof(word_t),size);i>0;--i)
                {
                    (ans <<= 8) |= unsigned( *(--b) );
                }
                return ans;
            }

            inline bool is_zero() const throw() { return (size<=0); }
            inline bool is_byte(const uint8_t x) const throw() { assert(x>0); return (1==size) && (x==byte[0]); }

            void           put(ios::bitio &Q) const;
            static natural get(ios::bitio &Q, const size_t n);
            void   save( ios::ostream &) const;
            static natural load( ios::istream &);

            //__________________________________________________________________
            //
            //
            // comparison
            //
            //__________________________________________________________________
            static inline bool are_same(const void *lhs, const size_t nl,
                                        const void *rhs, const size_t nr) throw()
            {
                if(nl!=nr)
                {
                    return false;
                }
                else
                {
                    const uint8_t *l = (const uint8_t *)lhs;
                    const uint8_t *r = (const uint8_t *)rhs;
                    YOCTO_LOOP(nl, if( *(l++) != *(r++) ) return false);
                    return true;
                }
            }
            YOCTO_MPN_DECL(bool,==,are_same)

            static inline bool are_different(const void *lhs, const size_t nl,
                                             const void *rhs, const size_t nr) throw()
            {
                if(nl!=nr)
                {
                    return true;
                }
                else
                {
                    const uint8_t *l = (const uint8_t *)lhs;
                    const uint8_t *r = (const uint8_t *)rhs;
                    YOCTO_LOOP(nl, if( *(l++) != *(r++) ) return true);
                    return false;
                }
            }
            YOCTO_MPN_DECL(bool,!=,are_different)

            static
            int compare(const void *lhs, const size_t nl,
                        const void *rhs, const size_t nr) throw();

            static inline
            int compare(const natural &lhs, const natural &rhs) throw()
            {
                return compare(lhs.byte,lhs.size,rhs.byte,rhs.size);
            }

            static inline
            int compare(const natural &lhs, word_t rhs ) throw()
            {
                const size_t n = prepare(rhs);
                return compare(lhs.byte,lhs.size,&rhs,n);
            }

            static inline
            int compare(word_t lhs, const natural &rhs) throw()
            {
                const size_t n = prepare(lhs);
                return compare(&lhs,n,rhs.byte,rhs.size);
            }
#define YOCTO_MPN_CMP(OP) \
inline friend bool operator OP (const natural &lhs, const natural &rhs) throw() { return natural::compare(lhs,rhs) OP 0; } \
inline friend bool operator OP (const natural &lhs, const word_t   rhs) throw() { return natural::compare(lhs,rhs) OP 0; } \
inline friend bool operator OP (const word_t   lhs, const natural &rhs) throw() { return natural::compare(lhs,rhs) OP 0; }

            YOCTO_MPN_CMP(<)
            YOCTO_MPN_CMP(<=)
            YOCTO_MPN_CMP(>)
            YOCTO_MPN_CMP(>=)

            //__________________________________________________________________
            //
            //
            // bit ops
            //
            //__________________________________________________________________
            static const uint8_t  _bit[8];
            
            //! get the number of bits
            size_t bits() const throw();

            //!  highest bit is always 1
            static natural rand(size_t nbits);

            inline bool is_odd() const throw()
            {
                return (size>0) ? ( 0 != (byte[0]&0x01) ) : false;
            }

            inline bool is_even() const throw()
            {
                return (size>0) ? ( 0 == (byte[0]&0x01) ) : true;
            }

            //__________________________________________________________________
            //
            //! 2^n
            //__________________________________________________________________
            static natural exp2(const size_t n);

            //__________________________________________________________________
            //
            // SHR
            //__________________________________________________________________
            natural & shr() throw(); //!< optimized one bit right shift
            static natural shr(const natural &lhs, const size_t n); //!< right shift n bits
            inline friend
            natural operator>>(const natural &lhs, const size_t n) { return natural::shr(lhs,n); }
            inline
            natural & operator>>=(const size_t n)
            {
                natural ans = natural::shr(*this,n);
                xch(ans);
                return *this;
            }

            //__________________________________________________________________
            //
            // SHL
            //__________________________________________________________________
            static natural shl( const natural &lhs, const size_t n  ); //!< shift left n bits
            inline
            natural & shl()
            {
                natural ans = shl(*this,1);
                xch(ans);
                return *this;
            }
            inline friend
            natural operator<<(const natural &lhs, const size_t n) { return natural::shl(lhs,n); }
            inline
            natural & operator<<=(const size_t n)
            {
                natural ans = natural::shl(*this,n);
                xch(ans);
                return *this;
            }


            //__________________________________________________________________
            //
            //
            // addition
            //
            //__________________________________________________________________
            //! helper
            inline void    ldz() throw() { size=0; }

#define Y_MPL_CLR(I) ( (uint8_t &)byte[I] ) = 0
            inline void    __clr() const throw()
            {
                (size_t &)size = 0;
                YOCTO_LOOP_FUNC_(maxi,Y_MPL_CLR,0);
            }

            //! unversal add function
            static natural add(const void *lhs, size_t nl,
                               const void *rhs, size_t nr);

            //! unary operator
            natural  operator+() const { return natural(*this); }
            YOCTO_MPN_DECL(natural,+,add)
            YOCTO_MPN_DECL_SELF(+=,+)
            
            
            inline natural &inc()
            {
                word_t       __one = 1;
                const size_t __len = prepare(__one);
                natural      __ans = add(byte,size,&__one,__len);
                xch(__ans);
                return *this;
            }

            //! prefix increment
            inline natural & operator++()
            {
                return inc();
            }

            //! postfix increment
            natural   operator++ (int)
            {
                natural sav(*this);
                (void)inc();
                return sav;
            }

            //__________________________________________________________________
            //
            //
            // subtraction
            //
            //__________________________________________________________________

            //! universal sub function
            static natural sub(const void *lhs, const size_t nl,
                               const void *rhs, const size_t nr);
            // no unary operator !
            YOCTO_MPN_DECL(natural,-,sub)
            YOCTO_MPN_DECL_SELF(-=,-)
            
          
            inline natural &dec()
            {
                word_t       __one = 1;
                const size_t __len = prepare(__one);
                natural      __ans = sub(byte,size,&__one,__len);
                xch(__ans);
                return *this;
            }

            //! prefix increment
            inline natural & operator--()
            {
                return dec();
            }

            //! postfix increment
            natural   operator-- (int)
            {
                natural sav(*this);
                (void)dec();
                return sav;
            }

            //__________________________________________________________________
            //
            //
            // multiplication
            //
            //__________________________________________________________________
            //! universal FFT based mul function
            static natural mul(const void *lhs, const size_t nl,
                               const void *rhs, const size_t nr);
            YOCTO_MPN_DECL(natural,*,mul)
            YOCTO_MPN_DECL_SELF(*=,*)

            
            static natural sqr(const natural &);
            static inline
            natural power(const natural &rhs,  word_t p)
            {
                natural ans = 1;
                while(p-->0) { ans *= rhs; }
                return ans;
            }

            static inline
            natural power(const natural &rhs, const natural &p )
            {
                natural ans = 1;
                for(natural i=0;i<p;++i) { ans *= rhs; }
                return ans;
            }

            static inline
            natural factorial(const word_t n)
            {
                natural ans(1);
                for(word_t i=2;i<=n;++i)
                {
                    ans *= i;
                }
                return ans;
            }

            static inline
            natural factorial(const natural &n)
            {
                natural ans(1);
                for(natural i=2;i<=n;++i)
                {
                    ans *= i;
                }
                return ans;
            }


            //__________________________________________________________________
            //
            //
            // division
            //
            //__________________________________________________________________

            //!universal division algorithm
            static natural div(const void *num, const size_t nn,
                               const void *den, const size_t nd);
            YOCTO_MPN_DECL(natural,/,div)
            YOCTO_MPN_DECL_SELF(/=,/)
            
            
            static void split( natural &q, natural &r, const natural &num, const natural &den );

            //__________________________________________________________________
            //
            //
            // modulo
            //
            //__________________________________________________________________
            //! universal modulo
            static natural __mod(const void *num, const size_t nn,
                                 const void *den, const size_t nd);
            static natural modulo(const void  *num,
                                  const size_t nn,
                                  const void  *den,
                                  const size_t nd);
            YOCTO_MPN_DECL(natural,%,modulo)
            YOCTO_MPN_DECL_SELF(%=,%)
           
            //__________________________________________________________________
            //
            //
            // arithmetic
            //
            //__________________________________________________________________
            static natural gcd(const natural &lhs, const natural &rhs);
            static inline bool are_coprime(const natural &lhs, const natural &rhs)
            {
                const natural __gcd = natural::gcd(lhs,rhs);
                return __gcd.is_byte(1);
            }
            static void simplify(natural &lhs, natural &rhs);

            static inline
            natural comb(const word_t n, const word_t k)
            {
                if(k<=0)
                {
                    return natural(1);
                }
                else
                {
                    if(k>=n)
                    {
                        return natural( (k>n) ? 0 : 1 );
                    }
                    else
                    {
                        assert(k>0);
                        assert(k<n);
                        const natural fn   = factorial(n);
                        const natural fk   = factorial(k);
                        const natural fnmk = factorial(n-k);
                        const natural den  = fk * fnmk;
                        return fn/den;
                    }
                }

            }

            static natural mod_inv( const natural &b, const natural &n );                     //!< modular inverse
            static natural mod_exp( const natural &b, const natural &e, const natural &n );   //!< modular exponentiation (b^e)[n]


            //__________________________________________________________________
            //
            //
            // primality
            //
            //__________________________________________________________________
            static bool    __is_prime( const natural &n );
            static natural __next_prime(const natural &n);

            //__________________________________________________________________
            //
            //
            // conversion
            //
            //__________________________________________________________________
            static double to_real(const natural &x);
            
        private:
            size_t   maxi; //!< maximum #bytes
            size_t   size; //!< current #bytes
            uint8_t *byte; //!< little endian
            
            static  uint8_t *   build(size_t &n);
            virtual const void *get_address() const throw() { return byte; }
            friend class integer;
        };
        
    }
    
    typedef mpl::natural mpn;
    template <>
    struct xnumeric<mpn>
    {
        inline static mpn zero() { return mpn();  }
        inline static mpn one()  { return mpn(1); }
    };
}

#endif
