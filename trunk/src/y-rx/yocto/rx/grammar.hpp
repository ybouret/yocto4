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
		
		typedef functor<void,TL2(const string,const token &)> production;
		
		class grammar : public object
		{
		public:
			
			
			explicit grammar( const string &  );
			virtual ~grammar() throw();
			
			syntax::rule  * operator[]( const string & );
			syntax::rule  * operator[]( const char   * );
			
			syntax::c_node *parse( lexer &lxr, source &src );
			
			//------------------------------------------------------------------
			// rules creation API
			//------------------------------------------------------------------
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
			
			//------------------------------------------------------------------
			// action API
			//------------------------------------------------------------------
#if 0
			void operator()( const string &rule_name, const production &do_something );
			inline void operator()( const char *rule_name, const production &do_something) { const string rn(rule_name); (*this)(rn,do_something); }
		
			template <typename HOST>
			inline void operator()( const string &rule_name, HOST &host, bool (HOST:: *method)( const string&,const token &) )
			{
				assert( method );
				const production a( &host, method );
				(*this)(rule_name,a);
			}
			
			template <typename HOST>
			inline void operator()( const char *rule_name, HOST &host, bool (HOST:: *method)(const string&,const token & ) )
			{
				assert( method );
				const  production a( &host, method );
				(*this)(rule_name,a);
			}
			
			void apply( const string &rule_name, const token &tkn);
#endif
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(grammar);
			typedef key_hasher<string>        keyHasher;
			typedef memory::pooled::allocator memAlloc;
			
			typedef set<string,syntax::rule::ptr,keyHasher,memAlloc> rule_set;
			//typedef map<string,production,keyHasher,memAlloc>   make_set;
			void record( syntax::rule *r );
			
		public:
			const string      name;
			
		private:
			syntax::rule     *root_;
			syntax::c_node   *tree_;
			rule_set          rset_;
			//make_set          mset_;
			
		};
		
	}
	
}


#endif
