#ifndef YOCTO_RX_SYMBOLS_INCLUDED
#define YOCTO_RX_SYMBOLS_INCLUDED 1


#include "yocto/ordered/catalog.hpp"
#include "yocto/memory/pooled.hpp"
#include <iosfwd>

namespace yocto
{
	namespace regex
	{
		//! catalog type
		template <typename T>
		struct catalog_of
		{
			typedef catalog<T,comparator<T>,memory::pooled::allocator> type;
		};
		typedef catalog_of<uint8_t>::type symbols;	
		
		//! detect first acceptable chars
		class first_chars : public symbols 
		{
		public:
			bool accept_empty;
			
			explicit first_chars() throw();
			virtual ~first_chars() throw();
			
			first_chars( const first_chars &other );
			
			//! leave accept_empty untouched
			void  merge( const symbols &other );
			
			friend std::ostream & operator<<( std::ostream &os, const first_chars &fch );
			
		private:
			YOCTO_DISABLE_ASSIGN(first_chars);
		};
		
		
		
		
	}
}

#endif

