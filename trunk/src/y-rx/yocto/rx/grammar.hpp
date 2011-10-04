#ifndef YOCTO_RX_GRAMMAR_INCLUDED
#define YOCTO_RX_GRAMMAR_INCLUDED 1

#include "yocto/rx/syntax/rule.hpp"
#include "yocto/associative/set.hpp"

namespace yocto
{
	
	namespace regex
	{
		
		class grammar
		{
		public:
			
			
			explicit grammar( syntax::rule *root_, const string &  );
			virtual ~grammar() throw();
			
			grammar & operator<<( syntax::rule *r );
			syntax::rule  * operator[]( const string & );
			
			syntax_result parse( lexer &lxr, source &src );
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(grammar);
			typedef set<string,syntax::rule::ptr,key_hasher<string>,memory::pooled::allocator> rules_set;
			syntax::rule::ptr root_;
		public:
			const string      name;
		private:
			rules_set         rset_;
			
		};
		
	}
	
}


#endif
