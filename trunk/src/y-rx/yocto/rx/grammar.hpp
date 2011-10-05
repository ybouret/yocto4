#ifndef YOCTO_RX_GRAMMAR_INCLUDED
#define YOCTO_RX_GRAMMAR_INCLUDED 1

#include "yocto/rx/syntax/c-node.hpp"

#include "yocto/rx/syntax/terminal.hpp"
#include "yocto/rx/syntax/logical.hpp"
#include "yocto/rx/syntax/counting.hpp"

#include "yocto/associative/set.hpp"

namespace yocto
{
	
	namespace regex
	{
		
		class grammar
		{
		public:
			
			
			explicit grammar( const string &  );
			virtual ~grammar() throw();
			
			syntax::rule  * operator[]( const string & );
			syntax::rule  * operator[]( const char   * );
			
			syntax_result parse( lexer &lxr, source &src );
			
			void                  terminal( const string &n );
			syntax::aggregate   & aggregate( const string &n );
			syntax::alternative & alternative( const string &n );
			void                  counting( const string &n, const string &p, char kind );
			
			inline void                  terminal( const char *t )    { const string n(t); terminal(n); }
			inline syntax::aggregate   & aggregate( const char *t )   { const string n(t); return aggregate(n); }
			inline syntax::alternative & alternative( const char *t ) { const string n(t); return alternative(n); }
			inline void                  counting( const char *n, const char *p, char kind ) 
			{ 
				const string _n(n), _p(p);
				counting(_n,_p,kind);
			}
			
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(grammar);
			typedef set<string,syntax::rule::ptr,key_hasher<string>,memory::pooled::allocator> rules_set;
			void record( syntax::rule *r );
			
		public:
			const string      name;
			
		private:
			rules_set         rset_;
			
			
		};
		
	}
	
}


#endif
