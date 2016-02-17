#ifndef YOCTO_MPK_NATURAL_INCLUDED
#define YOCTO_MPK_NATURAL_INCLUDED 1

#include "yocto/mpk/types.hpp"
#include "yocto/memory/buffer.hpp"
#include "yocto/code/endian.hpp"
#include "yocto/xnumeric.hpp"

#include <iosfwd>

namespace yocto
{
    namespace mpk
    {
#define YOCTO_CHECK_MPN(X)    \
assert( (X).maxi>0 );         \
assert( (X).size<=(X).maxi ); \
assert( (X).byte!=NULL     ); \
assert( !( ((X).size>0) && ( (X).byte[ (X).size-1 ] == 0) ) )
        
        
#define YOCTO_MPN_DECL2_NOTHROW(RETURN_TYPE,OP,CALL) \
inline friend RETURN_TYPE OP(const natural &lhs,const natural &rhs) throw() { return CALL(lhs,rhs); } \
friend        RETURN_TYPE OP(const natural &lhs,const word_t   rhs) throw(); \
friend        RETURN_TYPE OP(const word_t   lhs,const natural &rhs) throw()
        
#define YOCTO_MPN_IMPL2_NOTHROW(RETURN_TYPE,OP,CALL)          \
RETURN_TYPE OP(const natural &lhs,const word_t   rhs) throw() \
{ const word2mpn RHS(rhs); return CALL(lhs,RHS.n); }          \
RETURN_TYPE OP(const word_t lhs, const natural &rhs) throw()  \
{ const word2mpn LHS(lhs); return CALL(LHS.n,rhs); }
        
#define YOCTO_MPN_DECL2(RETURN_TYPE,OP,CALL) \
inline friend RETURN_TYPE OP(const natural &lhs,const natural &rhs) { return CALL(lhs,rhs); } \
friend        RETURN_TYPE OP(const natural &lhs,const word_t   rhs); \
friend        RETURN_TYPE OP(const word_t   lhs,const natural &rhs)
        
#define YOCTO_MPN_IMPL2(RETURN_TYPE,OP,CALL)            \
RETURN_TYPE OP(const natural &lhs,const word_t   rhs)   \
{ const word2mpn RHS(rhs); return CALL(lhs,RHS.n); }    \
RETURN_TYPE OP(const word_t lhs, const natural &rhs)    \
{ const word2mpn LHS(lhs); return CALL(LHS.n,rhs); }
        
        
#define YOCTO_MPN_OP(OP) \
inline friend bool operator OP (const natural &lhs, const natural &rhs) throw() \
{ return natural::compare(lhs,rhs) OP 0; }                                      \
inline friend bool operator OP (const natural &lhs, const word_t   rhs) throw() \
{ return natural::compare(lhs,rhs) OP 0; }                                      \
inline friend bool operator OP (const word_t   lhs, const natural &rhs) throw() \
{ return natural::compare(lhs,rhs) OP 0; }
        
        class natural : public memory::ro_buffer
        {
        public:
            static const uint8_t _bit[8];
            
            virtual ~natural() throw();
            virtual size_t length() const throw();
            
            //__________________________________________________________________
            //
            // canonic functions
            //__________________________________________________________________
            natural(); //!< zero
            //! size set to num_bytes, need to update after computation!
            natural(const size_t num_bytes, const as_capacity_t &);
            natural(const natural &other);
            natural & operator=( const natural &other);
            
            //__________________________________________________________________
            //
            // helpers
            //__________________________________________________________________
            natural(word_t w);
            natural & operator=(const word_t w);
            void xch(natural &other) throw();
            void ldz() throw(); //!< size to 0
            void clr() throw(); //!< size to 0, clean memory
            void __clr() const throw(); //!< force clr()
            
            friend std::ostream & operator<<( std::ostream &os, const natural &n);
            
            //__________________________________________________________________
            //
            //
            // bit ops
            //
            //__________________________________________________________________
            size_t bits() const throw();
            word_t to_word() const throw();     //!< least significant bytes
            
            //!  highest bit is always 1
            static natural rand(size_t nbits);
            
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
            // comparison: are equal
            //__________________________________________________________________
            static bool are_equal(const natural &lhs, const natural &rhs) throw();
            YOCTO_MPN_DECL2_NOTHROW(bool,operator==,are_equal);
            
            //__________________________________________________________________
            //
            // comparison: are different
            //__________________________________________________________________
            static bool are_different(const natural &lhs, const natural &rhs) throw();
            YOCTO_MPN_DECL2_NOTHROW(bool,operator!=,are_different);
            
            //__________________________________________________________________
            //
            // comparison: generic case
            //__________________________________________________________________
            static int compare(const natural &lhs, const natural &rhs) throw();
            static int compare(const natural &lhs, const word_t   rhs) throw();
            static int compare(const word_t   lhs, const natural &rhs) throw();
            
            //__________________________________________________________________
            //
            // comparison: operators
            //__________________________________________________________________
            YOCTO_MPN_OP(>)
            YOCTO_MPN_OP(>=)
            YOCTO_MPN_OP(<)
            YOCTO_MPN_OP(<=)
            
            //__________________________________________________________________
            //
            // addition
            //__________________________________________________________________
            
            //! addition function
            static natural add(const natural &lhs, const natural &rhs);
            
            //! unary + operator
            inline natural operator+() { return natural(*this); }
            
            //! binary + operators
            YOCTO_MPN_DECL2(natural,operator+,add);
            
            //! in place operator
            inline natural & operator+=( const natural &rhs)
            {
                natural ans = add(*this,rhs);
                xch(ans);
                return *this;
            }
            
            //! in place operator
            natural & operator+=(const word_t rhs);
            
            //! prefix increment
            natural & operator++();
            
            //! postfix increment
            natural   operator++ (int);
            
            //__________________________________________________________________
            //
            // subtraction
            //__________________________________________________________________
            
            //! subtraction function
            static natural sub(const natural &lhs, const natural &rhs);
            
            //! binary - operators (NO unary for natural!)
            YOCTO_MPN_DECL2(natural,operator-,sub);
            
            //! in place operator
            inline natural & operator-=( const natural &rhs )
            {
                natural ans = sub(*this,rhs);
                xch(ans);
                return *this;
            }
            
            //! in place operator
            natural & operator-=(const word_t rhs);
            
            //! prefix increment
            natural & operator--();
            
            //! postfix increment
            natural   operator-- (int);
            
            
            //__________________________________________________________________
            //
            //
            // multiplication
            //
            //__________________________________________________________________
            
            //! FFT multiplication
            static natural mul(const natural &lhs, const natural &rhs);
            
            //! binary * operator
            YOCTO_MPN_DECL2(natural,operator*,mul);
            
            //! in place operator
            inline natural & operator*=( const natural &rhs )
            {
                natural ans = mul(*this,rhs);
                xch(ans);
                return *this;
            }
            
            //! in place operator wrapper
            natural & operator*=(const word_t rhs);
            
            //! square
            static natural sqr(const natural &lhs);
            
            //! factorial
            static natural factorial(const natural &);
            
            //! factorial wrapper
            static natural factorial(const word_t);
            
            //! power
            static inline natural power(const natural &lhs, size_t n)
            {
                natural ans = 1;
                while(n-->0)
                {
                    ans *= lhs;
                }
                return ans;
            }
            
            static inline natural power(const natural &lhs,const natural &p)
            {
                natural ans = 1;
                for(natural n=p;n>0;--n)
                {
                    ans *= lhs;
                }
                return ans;
            }
            
            //__________________________________________________________________
            //
            //
            // division
            //
            //__________________________________________________________________
            static natural div(const natural &num, const natural &den);
            //! binary / operator
            YOCTO_MPN_DECL2(natural,operator/,div);
            
            //! in place operator
            inline natural & operator/=(const natural &den)
            {
                natural ans = div(*this,den);
                xch(ans);
                return *this;
            }
            
            //! in place operator wrapper
            natural & operator/=(const word_t den);
            
            
            //__________________________________________________________________
            //
            //
            // modulo
            //
            //__________________________________________________________________
            //! assuming den>0
            static natural __mod(const natural &num, const natural &den);
            
            static natural modulo(const natural &num, const natural &den);
            //! binary % operator
            YOCTO_MPN_DECL2(natural,operator%,modulo);
            
            //! in place operator
            inline natural &operator%=(const natural den)
            {
                natural ans = modulo(*this,den);
                xch(ans);
                return *this;
            }
            
            //! in place operator wrapper
            natural & operator%=(const word_t den);
            
            inline bool is_divisible_by(const natural &den) const { const natural ans = (*this) % den; return ans.size <= 0; }
            inline bool is_divisible_by(const word_t   den) const { const natural ans = (*this) % den; return ans.size <= 0; }
            
            static void split(natural &q, natural &r,const natural &num, const natural &den);
            
            
            //__________________________________________________________________
            //
            //
            // arithmetic
            //
            //__________________________________________________________________
            static natural gcd(const natural &x, const natural &y);
            static natural mod_inv( const natural &b, const natural &n );                     //!< modular inverse
            static natural mod_exp( const natural &b, const natural &e, const natural &n );   //!< modular exponentiation (b^e)[n]
            
            //__________________________________________________________________
            //
            //
            // bitwise ops
            //
            //__________________________________________________________________
            typedef unsigned (*bproc)(const unsigned,const unsigned);
            static  natural __apply(bproc          proc,
                                    const uint8_t *l,
                                    const size_t   nl,
                                    const uint8_t *r,
                                    const size_t   nr);
            
            static inline natural _apply(bproc          proc,
                                         const natural &lhs,
                                         const natural &rhs)
            {
                return __apply(proc, lhs.byte, lhs.size, rhs.byte, rhs.size);
            }
            
            static inline natural _apply(bproc          proc,
                                         const natural &lhs,
                                         word_t         rhs)
            {
                rhs = swap_le(rhs);
                return __apply(proc, lhs.byte, lhs.size, (uint8_t *)&rhs, sizeof(word_t) );
            }

            static inline natural _apply(bproc          proc,
                                         word_t         lhs,
                                         const natural &rhs)
            {
            }
            
            
        private:
            size_t   maxi; //!< capacity
            size_t   size; //!< significant bytes
            uint8_t *byte; //!< memory
            
            void update() throw(); //!< check size reduction
            void rescan() throw(); //!< size=maxi and update
            
            virtual const void * get_address() const throw();
            friend class word2mpn;
        };
        
    }
    
    
    typedef mpk::natural mpn;
    template <>
    struct xnumeric<mpn>
    {
        inline static mpn zero() { return mpn();  }
        inline static mpn one()  { return mpn(1); }
    };
    
}

#endif