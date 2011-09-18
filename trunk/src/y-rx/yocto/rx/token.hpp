#ifndef YOCTO_RX_TOKEN_INCLUDED
#define YOCTO_RX_TOKEN_INCLUDED 1

#include "yocto/core/list.hpp"
#include "yocto/core/pool.hpp"
#include "yocto/string.hpp"

namespace yocto
{
	
	namespace regex
	{
		
		//! single dynamic char
		class t_char 
		{
		public:
			t_char *next;
			t_char *prev;
			char    data;
			
			static t_char  *acquire();
			static void     release( t_char *ch ) throw();
			
		private:
			t_char(); ~t_char() throw();
			YOCTO_DISABLE_COPY_AND_ASSIGN(t_char);
		};
		
		//! t_char pool
		class t_pool : public core::pool_of<t_char>
		{
		public:
			explicit t_pool() throw();
			virtual ~t_pool() throw();
			t_char  *create( char c );
			t_char  *acquire();
			void     release() throw();
			void     reserve(size_t n);
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(t_pool);
		};
		
		
		//! token is a list of t_char
		class token : public object, public core::list_of<t_char>
		{
		public:
			explicit token() throw();
			virtual ~token() throw();
			token( const token & );
			token( const token &, t_pool &pool );
			void release() throw();
			void back_to( t_pool &pool ) throw(); //!< all t_chars back to pool
			void move_at_head_of( token &other ) throw(); //!< for i/o caching
			void move_at_tail_of( token &other ) throw(); //!< 'tkncat'
			string to_string( size_t skip=0, size_t trim=0 ) const;
			friend std::ostream & operator<< ( std::ostream &, const token & );
			
		private:
			YOCTO_DISABLE_ASSIGN(token);
		};
		
		
	}
	
}

#endif
