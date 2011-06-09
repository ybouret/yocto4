#ifndef YOCTO_STRING_BASIC_INCLUDED
#define YOCTO_STRING_BASIC_INCLUDED

#include "yocto/memory/buffer.hpp"

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
			string( const char s );
			
			
		private:
			size_t full_; //!< memory units
			T     *char_; //!< memory location
			size_t maxi_; //!< in units: full-1
			size_t size_; //!< #chars

			
			virtual const void *get_address() const throw();
		};
		
	}
	
}

#endif
