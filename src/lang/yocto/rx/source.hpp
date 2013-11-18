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
						
			//! read a new t_char, NULL on end of source.			
			t_char       *get(); 
            
            //! unread a previously read t_char.
			void          unget( t_char * ) throw();
            
            //! unread a previously read token.
			void          unget( token &  ) throw();
            
            //! unread a copy a a previouslyu read token.
			void          uncpy( const token & );
            
            //! try to fill cache and return cache head.
			const t_char *peek(); 
            
            //! true is more t_char available
			bool          is_active();
            
            //! try to fill cache with n t_char
			void          prefetch(size_t n);
            
            //! cache size
			size_t        in_cache() const throw();
            
            //! skip n >= in_cache
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
