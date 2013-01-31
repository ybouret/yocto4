//!\file

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
			t_char *next; //!< for list 
			t_char *prev; //!< for list 
			char    data; //!< actual char
			
			static t_char  *acquire();
			static void     release( t_char *ch ) throw();
			
			
		private:
			t_char(); ~t_char() throw();
			YOCTO_DISABLE_COPY_AND_ASSIGN(t_char);
		};
		
		
		
		//! token is a linked list of t_char
		class token : public object, public core::list_of<t_char>
		{
		public:
			explicit token() throw(); //!< construct an empty token
			virtual ~token() throw(); //!< destructor
            explicit token( const string &data); //!< construct and initialize token
			token( const token & );   //!< dynamic copy
			void release() throw();
			void move_at_head_of( token &other ) throw(); //!< for i/o caching
			void move_at_tail_of( token &other ) throw(); //!< 'tkncat'
			
			//! conversion to a string
			string to_string( size_t skip=0, size_t trim=0 ) const;
			
			//! for debugging
			friend std::ostream & operator<< ( std::ostream &, const token & );
			
		private:
			YOCTO_DISABLE_ASSIGN(token);
		};
		
		
	}
	
}

#endif
