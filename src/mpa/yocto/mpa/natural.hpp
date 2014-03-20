#ifndef YOCTO_MPA_NATURAL_INCLUDED
#define YOCTO_MPA_NATURAL_INCLUDED 1

#include "yocto/mpa/types.hpp"
#include "yocto/string.hpp"
#include <iosfwd>

namespace yocto
{
    namespace ios
    {
        class bitio;
        class ostream;
        class istream;
    }
    
    namespace mpa
    {
        class word2mpn;
        
        //! a little-endian muliple precision natural number
        class natural : public memory::ro_buffer
        {
        public:
            static const uint8_t _bit[8];
            //__________________________________________________________________
            //
            // class operations
            //__________________________________________________________________
            natural();
            natural( const size_t n, const as_capacity_t &);
            virtual ~natural() throw();
            natural(const natural &);
            natural & operator=( const natural &);
            natural & operator=( uint64_t ) throw();
            virtual size_t length() const throw(); //!< ro_buffer API
            
            //__________________________________________________________________
            //
            // basic API
            //__________________________________________________________________
            bool   bit( const size_t index ) const throw();
            void   xch( natural &other ) throw();
            void   ldz() throw();   //!< size to 0
            void   clear() throw(); //!< memory clear
            void   __clr() const throw(); //!< force 0
            size_t bits() const throw();
            bool   is_zero() const throw();
            bool   is_one()  const throw();
            bool   is_two()  const throw();
            bool   is_odd()  const throw();
            bool   is_even() const throw();
            static natural one();
            static natural two();
            static natural three();
            
#define YOCTO_MPA_TO ans <<= 8; ans |= get_byte(i-1)
			template <typename T>
			inline T to() const throw()
			{
				T ans(0);
                for( size_t i=sizeof(T);i>0;--i)
				{
                    YOCTO_MPA_TO;
                }
                return ans;
            }
            
            
            //__________________________________________________________________
            //
            // setting values
            //__________________________________________________________________
            natural(uint64_t x);
            natural(const memory::ro_buffer &buf);
            natural(const void *buf, size_t len);
            
            //__________________________________________________________________
            //
            // output
            //__________________________________________________________________
            void   out( std::ostream &) const;
            friend std::ostream & operator<<( std::ostream &, const natural &);
            void           store( ios::bitio &bio, size_t nbits ) const;
            void           store( ios::bitio &bio) const;
            static natural query( ios::bitio &, size_t nbits );
            void           save( ios::ostream &out ) const;
            static natural load( ios::istream &in  );
            
            //__________________________________________________________________
            //
            // comparison
            //__________________________________________________________________
            static int compare( const natural  &, const natural &) throw();
            static int compare( const natural  &, const uint64_t ) throw();
            static int compare( const uint64_t ,  const natural &) throw();
            
            //! for arguments disambiguation
            static inline int cmp( const natural  &lhs, const natural &rhs) throw()
            {
                return compare(lhs,rhs);
            }
#define YOCTO_COMPARE(OP) \
inline friend bool operator OP (const natural &lhs, const natural &rhs) throw() { return compare(lhs,rhs) OP 0; } \
inline friend bool operator OP (const natural &lhs, const uint64_t rhs) throw() { return compare(lhs,rhs) OP 0; } \
inline friend bool operator OP (const uint64_t lhs, const natural &rhs) throw() { return compare(lhs,rhs) OP 0; }
            
            YOCTO_COMPARE(==)
            YOCTO_COMPARE(!=)
            YOCTO_COMPARE(<)
            YOCTO_COMPARE(<=)
            YOCTO_COMPARE(>)
            YOCTO_COMPARE(>=)
            
            
            //__________________________________________________________________
            //
            // addition
            //__________________________________________________________________
            static natural add( const natural &lhs, const natural &rhs );
            static natural add( const natural &lhs, const uint64_t x);
            friend natural operator+( const natural &lhs, const natural &rhs );
            friend natural operator+( const natural &lhs, const uint64_t x   );
            friend natural operator+( const uint64_t lhs, const natural &rhs );
            
            natural & operator+=( const natural &rhs );
            natural & operator+=( const uint64_t rhs );
            void      inc(uint8_t);
            natural & operator++();     //!< prefix
            natural   operator++ (int); //!< postfix
            
            //__________________________________________________________________
            //
            // subtraction
            //__________________________________________________________________
            static natural sub( const natural &lhs, const natural &rhs );
            static natural sub( const natural &lhs, const uint64_t x);
            static natural sub( const uint64_t x,   const natural &rhs);
            friend natural operator-( const natural &lhs, const natural &rhs );
            friend natural operator-( const natural &lhs, const uint64_t rhs );
            natural & operator-=( const natural &rhs );
            natural & operator-=( const uint64_t rhs );
            void      dec(uint8_t);
            natural & operator--();     //!< prefix
            natural   operator-- (int); //!< postfix
            
            //__________________________________________________________________
            //
            // multiplication
            //__________________________________________________________________
            static natural mul( const natural &lhs, const natural &rhs );
            static natural mul( const natural &lhs, const uint64_t x);
            friend natural operator*( const natural &lhs, const natural &rhs);
            friend natural operator*( const natural &lhs, const uint64_t rhs);
            friend natural operator*( const uint64_t lhs, const natural &rhs);
            natural & operator*=( const natural &rhs);
            natural & operator*=( const uint64_t rhs);
            static natural sqr( const natural &lhs);
            
            //__________________________________________________________________
            //
            // 2^n
            //__________________________________________________________________
            static natural exp2(size_t n); //!< \return 1 << n
            
            //__________________________________________________________________
            //
            // SHR
            //__________________________________________________________________
            natural  &     shr() throw(); //!< optimized right shift
            static natural shr( const natural &, size_t n );
            natural operator>>( size_t n) const;
			natural & operator>>=( size_t n );
            
            //__________________________________________________________________
            //
            // SHL
            //__________________________________________________________________
            static natural shl( const natural &lhs, size_t n  ); //!< shift left
			natural operator<<( size_t n) const;
			natural & shl();         //!< shift left by one
			natural & operator<<=( size_t n );
            
            //__________________________________________________________________
            //
			// division
            //__________________________________________________________________
			static natural  div( const natural &num, const natural &den );
            static natural  div( const natural &num, const uint64_t x   );
            static natural  div( const uint64_t x,   const natural &den );
			friend natural operator/( const natural &num, const natural &den );
            friend natural operator/( const natural &num, const uint64_t den );
            friend natural operator/( const uint64_t num, const natural &den );
			natural & operator/=( const natural &den );
            natural & operator/=( const uint64_t den );
            
            //__________________________________________________________________
            //
			// modulo
            //__________________________________________________________________
			static natural  __mod(  const natural &num, const natural &den ); // assume den > 0
			static natural  modulo( const natural &num, const natural &den );
            static natural  modulo( const natural &num, const uint64_t x   );
            static natural  modulo( const uint64_t x,   const natural &den );
			friend natural  operator%(  const natural &num, const natural &den );
            friend natural  operator%(  const natural &num, const uint64_t den );
			friend natural  operator%(  const uint64_t num, const natural &den );
            
			natural & operator %= ( const natural &den );
            natural & operator %= ( const uint64_t den );
            bool is_divisible_by( const natural &den ) const;
            
            //__________________________________________________________________
            //
			// modular arithmetic
            //__________________________________________________________________
			static natural  mod_inv( const natural &b, const natural &n );                     //!< modular inverse
			static natural  mod_exp( const natural &b, const natural &e, const natural &n );   //!< modular exponentiation (b^e)[n]
            
            //__________________________________________________________________
            //
			// arithmetic
            //__________________________________________________________________
			static natural gcd( const natural &lhs, const natural &rhs );           //!< greatest common divisor
			static bool    are_coprime( const natural &lhs, const natural &rhs );   //!< gcd(lhs,rhs) == 1
            
            
            //__________________________________________________________________
            //
			// parsing
            //__________________________________________________________________
            static natural hex( const string & ); // without 0x
            static natural hex( const char   * ); // without 0x
            
            static natural dec( const string & );
            static natural dec( const char   * );
            
            static natural parse( const string & );
            
            //__________________________________________________________________
            //
			// primality
            //__________________________________________________________________
            bool     is_prime_() const;
            natural  next_prime_() const;
            
            //__________________________________________________________________
            //
			// Random
            //__________________________________________________________________
            static natural rand(size_t nbits);
            
        private:
            size_t   maxi;
            size_t   size;
            uint8_t *byte;
            
            void update() throw(); //!< start from size and adjust predicted size
            void rescan() throw(); //!< start from maxi and adjust
            virtual const void *get_address() const throw(); //!< ro_buffer API
            friend class word2mpn;
        };
#define YOCTO_CHECK_MPN(X)    \
assert((X)->byte);            \
assert((X)->maxi>0);          \
assert((X)->size<=(X)->maxi); \
assert( ! ( (X)->size > 0 && (X)->byte[ (X)->size-1 ] == 0 ) )
        
        
    }
    
    typedef mpa::natural mpn;
    
}

#endif

