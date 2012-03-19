#ifndef YOCTO_RX_SOURCE_INCLUDED
#define YOCTO_RX_SOURCE_INCLUDED 1

#include "yocto/rx/token.hpp"
#include "yocto/ios/istream.hpp"

namespace yocto
{
	namespace regex
	{
		
		//! char stream to t_char stream
		class source 
		{
		public:
			explicit source( ios::istream &input ) throw();
			virtual ~source() throw();
						
						
			t_char       *get(); 
			void          unget( t_char * ) throw();
			void          unget( token &  ) throw();
			void          uncpy( const token & );
			const t_char *peek(); //! try to fill cache and return cache head.
			bool          is_active();
			void          prefetch(size_t n);
			size_t        in_cache() const throw();
			void          skip(size_t n) throw();//!< n <= in_cache()
			
		private:
			token         cache_;
			ios::istream &input_;
			
			bool cache1();
			
			YOCTO_DISABLE_COPY_AND_ASSIGN(source);
		};
		
	}
}

#endif
