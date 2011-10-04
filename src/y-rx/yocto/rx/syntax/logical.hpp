#ifndef YOCTO_RX_LOGICAL_INCLUDED
#define YOCTO_RX_LOGICAL_INCLUDED 1

#include "yocto/rx/syntax/rule.hpp"
#include "yocto/code/fourcc.hpp"
#include "yocto/sequence/vector.hpp"

namespace yocto
{
	
	namespace regex
	{
		
		namespace syntax
		{
				
			class logical : public rule
			{
			public:
				virtual ~logical() throw();
				
				inline logical & operator << (  rule *r )
				{
					const rule::ptr p(r);
					operands.push_back( p );
					return *this;
				}
				
				inline logical & operator << (  const rule::ptr &p )
				{
					operands.push_back( p );
					return *this;
				}
				
				
			protected:
				explicit logical( uint32_t t, const string &n );
				vector<rule::ptr,memory::pooled::allocator> operands;
				
			private:
				YOCTO_DISABLE_COPY_AND_ASSIGN(logical);
			};
			
			
			class aggregate : public logical
			{
			public:
				static const uint32_t ID = YOCTO_FOURCC( 'A','G','G','R' );
				virtual ~aggregate() throw();
				
				static aggregate *create( const string &n );
				
			private:
				aggregate( const string &n );
				YOCTO_DISABLE_COPY_AND_ASSIGN(aggregate);
				virtual syntax_result match( YOCTO_RX_SYNTAX_RULE_ARGS );
			};
		}
		
		inline syntax::aggregate * new_aggregate( const string &n ) { return syntax::aggregate:: create(n); }
		inline syntax::aggregate * new_aggregate( const char   *t ) { const string n(t); return syntax::aggregate::create(n); }
	}
}

#endif
