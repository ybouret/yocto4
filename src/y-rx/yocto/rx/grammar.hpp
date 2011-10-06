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
		typedef syntax::c_node cst_node;
		
		class grammar
		{
		public:
			
			
			explicit grammar( const string &  );
			virtual ~grammar() throw();
			
			syntax::rule  * operator[]( const string & );
			syntax::rule  * operator[]( const char   * );
			
			cst_node *parse( lexer &lxr, source &src, syntax_result &res );
			
			void                  terminal( const string &n, int p=0);
			syntax::aggregate   & aggregate( const string &n, int p=0);
			syntax::alternative & alternative( const string &n );
			void                  counting( const string &n, const string &m, char kind, int p=0);
			
			inline void                  terminal( const char *t, int p=0 )   { const string n(t); terminal(n,p); }
			inline syntax::aggregate   & aggregate( const char *t, int p=0)   { const string n(t); return aggregate(n,p); }
			inline syntax::alternative & alternative( const char *t ) { const string n(t); return alternative(n); }
			inline void                  counting( const char *n, const char *m, char kind , int p=0) 
			{ 
				const string _n(n), _m(m);
				counting(_n,m,kind,p);
			}
			
			void reset() throw();
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(grammar);
			typedef set<string,syntax::rule::ptr,key_hasher<string>,memory::pooled::allocator> rules_set;
			void record( syntax::rule *r );
			
		public:
			const string      name;
			
		private:
			cst_node         *tree_;
			rules_set         rset_;
			
			
		};
		
	}
	
}


#endif
