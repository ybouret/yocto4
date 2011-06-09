#ifndef YOCTO_STRING_BASIC_INCLUDED
#define YOCTO_STRING_BASIC_INCLUDED

#include "yocto/memory/buffer.hpp"
#include "yocto/chars.hpp"

namespace yocto
{
	
	namespace basic
	{
		
		template <typename T>
		class string : public memory::rw_buffer
		{
		public:
			//------------------------------------------------------------------
			//
			// canonical form
			//
			//------------------------------------------------------------------
			virtual ~string() throw();
			
			string();
			string( const string & );
			string &operator=( const string & );
			
			//------------------------------------------------------------------
			//
			// information
			//
			//------------------------------------------------------------------
			size_t         size() const throw();
			size_t         capacity() const throw();
			virtual size_t length() const throw();
			
			void swap_with( string &other ) throw();
			
			//------------------------------------------------------------------
			//
			// constructors
			//
			//------------------------------------------------------------------
			string( size_t n, as_capacity_t & );
			string( const T C );
			string( const T *s );

			//------------------------------------------------------------------
			//
			// assign
			//
			//------------------------------------------------------------------
			void clear() throw();
			void assign( YCHAR C ) throw();
			void assign( const T      *s );
			void assign( const string &s );

			
		
			
		private:
			size_t size_; //!< #chars
			size_t full_; //!< memory units
			T     *char_; //!< memory location
			size_t maxi_; //!< in units: full-1
			
			string( const T *a, size_t a_len, const T *b, size_t b_len );

			
			virtual const void *get_address() const throw();
		};
		
	}
	
}

#endif
