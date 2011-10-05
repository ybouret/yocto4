#ifndef YOCTO_RX_GRAMMAR_INCLUDED
#define YOCTO_RX_GRAMMAR_INCLUDED 1

#include "yocto/rx/syntax/terminal.hpp"
#include "yocto/rx/syntax/logical.hpp"
#include "yocto/rx/syntax/counting.hpp"

#include "yocto/associative/set.hpp"

namespace yocto
{
	
	namespace regex
	{
		
		namespace syntax
		{
			class c_node
			{
			public:
				enum type_t
				{
					internal,
					terminal
				};
				typedef core::list_of<c_node> child_nodes;
				
				const type_t  type;
				c_node       *next;
				c_node       *prev;
				const rule   &link;
				
				static c_node *create( lexeme *lx, rule &r )
				{
					try 
					{
						return new( object::acquire1<c_node>() ) c_node(lx,r);
					}
					catch (...) {
						lexeme::destroy(lx);
						throw;
					}
				}
				
				static c_node *create( rule &r )
				{
					return new( object::acquire1<c_node>() ) c_node(r);
				}
				
				
				static void destroy( c_node *node ) throw()
				{
					assert(node);
					switch( node->type )
					{
						case terminal:
							lexeme::destroy( node->data.lx );
							break;
							
						case internal:
							
							break;
					}
					node->~c_node();
					object::release1<c_node>(node);
				}
				
			private:
				explicit c_node( lexeme *lx, rule &r ) throw() :
				type(terminal),
				next(NULL),
				prev(NULL),
				link(r),
				data()
				{
					data.lx = lx;
				}
				
				explicit c_node( rule &r ) throw() :
				type(internal),
				next(NULL),
				prev(NULL),
				link(r),
				data()
				{
					_children().reset();
				}
				
				~c_node() throw() { assert( type == terminal || _children().size <= 0 ); }
				
				
				union 
				{
					lexeme  *lx;
					uint32_t wksp[ YOCTO_U32_FOR_ITEM(child_nodes) ];
				} data;
				
				inline lexeme      * _lexeme()   throw() { return data.lx; }
				inline child_nodes & _children() throw() { return *(child_nodes *) & data.wksp[0]; }
				
				YOCTO_DISABLE_COPY_AND_ASSIGN(c_node);
			};
		}
		
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
