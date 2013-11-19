#ifndef YOCTO_MPK_NATURAL_INCLUDED
#define YOCTO_MPK_NATURAL_INCLUDED 1

#include "yocto/type/ints.hpp"
#include "yocto/string.hpp"

#include <iosfwd>

namespace yocto
{
    
	namespace ios { class bitio; class ostream; class istream; }
    
	namespace mpk
	{
        
		class    memmgr;
		size_t   mem_round( size_t n );
		uint8_t *mem_acquire( size_t &n );
		void     mem_release( uint8_t * &, size_t &n ) throw();
		size_t   mem_limit_size() throw();
        
        
#		define YMPN_CMP_DECL(FUNC) \
/*		*/ friend bool operator FUNC ( const natural &lhs, const natural &rhs ) throw()
        
		class natural : public memory::ro_buffer
		{
		public:
			explicit natural();
			virtual ~natural() throw();
            
			natural( const natural & );
			natural & operator=( const natural & );
            
			void ldz() throw(); //!< set to 0, fast
            void ldZ() throw(); //!< set to 0, slow
			void xch( natural &other ) throw();
			bool is_zero() const throw();
			bool is_positive() const throw();
			bool is_one() const throw();
			bool is_two() const throw();
            
			size_t bits() const throw();
            
			//==================================================================
			// I/O
			//==================================================================
			void out( std::ostream & ) const;
			friend inline std::ostream & operator<<( std::ostream &os, const natural &mp )
			{
				mp.out(os);
				return os;
			}
            
			bool bit( size_t index ) const throw();
			void store( ios::bitio &, size_t nbits ) const;
			static natural query( ios::bitio &, size_t nbits );
			void           save( ios::ostream &fp ) const;
			static natural load( ios::istream &fp );
            
			//==================================================================
			// ro_buffer interface
			//==================================================================
			virtual size_t length() const throw();
            
			//==================================================================
			// special constructor
			//==================================================================
			natural( const ro_buffer & ); //!< binary + update
			natural( const void *buffer, const size_t buflen ); //!< binary + update
			natural( const uint64_t ); //!< default cast
            
			//==================================================================
			// fast conversion
			//==================================================================
			template <typename T>
			inline T to() const throw()
			{
				T ans(0);
#if defined(__ICC)
#pragma warning(push)
#pragma warning(disable:2259)
#endif
				for( size_t i=0; i <sizeof(T); ++i)
				{
					ans |= ( T(get_byte(i)) << ( i << 3 ) );
				}
				return ans;
#if defined(__ICC)
#pragma warning(pop)
#endif
			}
            
			//==================================================================
			// addition
			//==================================================================
			static natural add_( const natural &lhs, const natural &rhs );
			natural inc_( uint8_t x ) const;
			friend natural operator+( const natural &lhs, const natural &rhs );
			natural & operator+=( const natural &rhs );
			natural & operator++();
			natural & inc( uint8_t x );
            
			//==================================================================
			// subtraction
			//==================================================================
			static natural sub_( const natural &lhs, const natural &rhs );
			friend natural operator-(const natural &lhs, const natural &rhs );
			natural & operator -=( const natural &rhs );
			natural dec_( uint8_t x ) const;
			natural & operator--();
			natural & dec( uint8_t x );
            
			//==================================================================
			// multiplication
			//==================================================================
			static natural mul_( const natural &lhs, const natural &rhs );
			static natural fft_( const natural &lhs, const natural &rhs );
			static natural sqr_( const natural &lhs);
			friend natural operator*( const natural &lhs, const natural &rhs );
			natural & operator *= ( const natural &rhs );
            
			//==================================================================
			// bitwise
			//==================================================================
			static natural shl_( const natural &lhs, size_t n  ); //!< shift left
			natural operator<<( size_t n) const;
			natural & shl();         //!< shift left by one
			natural & operator<<=( size_t n );
            
			static natural shr_( const natural &lhs, size_t n); //!< shift right
			natural & shr() throw(); //!< shift right by one, special
			natural operator>>( size_t n) const;
			natural & operator>>=( size_t n );
            
			static natural exp2( size_t n ); //!< \return 1 << n
            
			//==================================================================
			//comparison
			//==================================================================
			static int compare( const natural &lhs, const natural &rhs ) throw();
			enum comparison
			{
				is_less,
				is_equal,
				is_greater
			};
			static inline comparison comparison_of( const natural &lhs, const natural &rhs ) throw()
			{
				const int cmp = natural::compare( lhs, rhs );
				return cmp < 0 ?  is_less : (cmp > 0  ? is_greater : is_equal);
			}
			YMPN_CMP_DECL(==);
			YMPN_CMP_DECL(!=);
			YMPN_CMP_DECL(<);
			YMPN_CMP_DECL(>);
			YMPN_CMP_DECL(<=);
			YMPN_CMP_DECL(>=);
            
			//==================================================================
			// division
			//==================================================================
			static natural  div_( const natural &num, const natural &den );
			friend natural operator/( const natural &num, const natural &den );
			natural & operator/=( const natural &den );
            
			//==================================================================
			// mod
			//==================================================================
			static natural  mod__( const natural &num, const natural &den ); // assume den > 0
			static natural  mod_( const natural &num, const natural &den );
			friend natural  operator%(  const natural &num, const natural &den );
			natural & operator %= ( const natural &den );
            
			//==================================================================
			// modular arithmetic
			//==================================================================
			static natural  mod_inv( const natural &b, const natural &n );                     //!< modular inverse
			static natural  mod_exp( const natural &b, const natural &e, const natural &n );   //!< modular exponentiation (b^e)[n]
            
			//==================================================================
			// arithmetic
			//==================================================================
			static natural gcd( const natural &lhs, const natural &rhs );           //!< greatest common divisor
			static bool    are_coprime( const natural &lhs, const natural &rhs );   //!< gcd(lhs,rhs) == 1
            
			static natural one();
			static natural two();
            
			bool    is_even()     const throw();
			bool    is_odd()      const throw();
            
			bool    is_prime_()   const; //!< sieve based algorithm
			natural next_prime_() const;
            
			//==================================================================
			// parsing
			//==================================================================
			static natural dec( const string & );
			static natural dec( const char *txt );
			static natural hex( const string & );
			static natural hex( const char *txt );
            
            
		private:
			size_t   size_;
			size_t   maxi_;
			uint8_t *byte_;
			void     update() throw(); //!< update predicted bits_
			void     rescan() throw(); //!< set bits_ to max and update
            
			virtual const void * get_address() const throw();
            
			natural( size_t n, const as_capacity_t & );
            
            
		public:
			static  const uint8_t  _bit[8];
			static  const uint8_t  _msk[8];
		};
        
		template <typename T>
		class array_of
		{
		public:
			explicit array_of( size_t n ) :
			size(   n <= 0 ? 1 : n   ),
            buflen( size * sizeof(T) ),
            buffer( mem_acquire( buflen ) ),
            data(  (T*) &buffer[0] )
			{
			}
            
			virtual ~array_of() throw()
			{
				mem_release( buffer, buflen );
				data = 0;
				(size_t &)size = 0;
			}
            
			const size_t size;
            
			inline T &       operator[](size_t i) throw()       { assert(i<size); return data[i]; }
			inline const T & operator[](size_t i) const throw() { assert(i<size); return data[i]; }
            
		private:
			size_t       buflen;
			uint8_t     *buffer;
			T           *data;
			YOCTO_DISABLE_COPY_AND_ASSIGN(array_of);
		};
        
	}
    
	typedef mpk::natural mpn;
    
}

#endif
