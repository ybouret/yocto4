#ifndef YOCTO_RX_SOURCE_INCLUDED
#define YOCTO_RX_SOURCE_INCLUDED 1

#include "yocto/rx/token.hpp"
#include "yocto/ios/istream.hpp"

namespace yocto
{
	namespace regex
	{
		
		
		class source 
		{
		public:
			explicit source() throw();
			virtual ~source() throw();
						
			void connect( ios::istream &input ) throw();
			void disconnect() throw();
			
			t_char *get();
			void    unget( t_char * ) throw();
			void    unget( token &  ) throw();
			void    uncpy( const token & );
			const t_char *peek(); //! try to fill cache and return cache head.
			inline  bool is_active() { return NULL != peek(); }
			
			void   prefetch(size_t n);
			size_t in_cache() const throw();
			
			void   skip(size_t n) throw();       //!< n <= in_cache()
			
		private:
			token         cache_;
			ios::istream *input_;
			
			void reset() throw();
			bool cache1();
			
			YOCTO_DISABLE_COPY_AND_ASSIGN(source);
		};
		
	}
}

#endif
