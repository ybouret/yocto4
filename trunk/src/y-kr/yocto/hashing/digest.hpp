#ifndef YOCTO_HASHING_DIGEST_INCLUDED
#define YOCTO_HASHING_DIGEST_INCLUDED 1

#include "yocto/string.hpp"
#include "yocto/code/endian.hpp"

namespace yocto
{
	
	namespace hashing
	{
		class function;
	}
	
	//! fixed size digest
	class digest : public memory::rw_buffer
		{
		public:
			explicit digest( size_t n, uint8_t fill = 0 ); //!< n bytes, set to fill
			virtual ~digest() throw();
			digest( const digest            &other ); //!< copy length and content
			digest( const memory::ro_buffer &buff  ); //!< copy length and content
			
			digest &operator=( const digest            & ) throw(); //!< copy TRUNCATED to this->length
			digest &operator=( const memory::ro_buffer & ) throw(); //!< copy TRUNCATED to this->length
			
			virtual size_t length() const throw(); //!< rw_buffer
			
			uint8_t       &operator[]( size_t ) throw();
			const uint8_t &operator[]( size_t ) const throw();
			friend std::ostream &operator<<( std::ostream &, const digest & );
			
			void _set( const uint8_t x     ) throw(); //!< fill
			void _set( const digest &other ) throw(); //!< for same length
			void _xor( const digest &other ) throw(); //!< for same length
			void _xor( const digest &lhs, const digest &rhs ) throw(); //!< for same length
			void _swp( digest &other ) throw();   //!< for same length
			void _inc( const uint8_t x ) throw(); //!< increase with overflow
			
			template <typename T>
			T get() const throw()
			{
				T res = 0;
				this->get_lsb( &res, sizeof(T) );
				return swap_be_as<T>(res);
			}
			
			template <typename T>
			void set( T value ) throw()
			{
				const T x( swap_be_as<T>(value) );
				this->set_lsb( &x, sizeof(T) );
			}
			
			static digest hex( const string &s );
			static digest hex( const char   *s );
			
			friend bool operator==( const digest &lhs, const digest &rhs ) throw();
			friend bool operator!=( const digest &rhs, const digest &lsh ) throw();
			
			static digest get_from( hashing::function &h ); //!< length == h.length
			static digest checksum( hashing::function &h, const memory::ro_buffer & );
			
			
		private:
			const size_t  length_;
			uint8_t      *buffer_;
			virtual const void *get_address() const throw();
			void assign( const ro_buffer &buff ) throw();
			void get_lsb( void *p, size_t n ) const throw(); //!< last bytes
			void set_lsb( const void *p, size_t n ) throw(); //!< last bytes
		};
	
	template <typename HFUNC>
	inline digest checksum( const memory::ro_buffer &B )
	{
		HFUNC H;
		return digest::checksum(H,B);
	}
	
}

#endif
