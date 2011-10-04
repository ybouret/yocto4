#ifndef YOCTO_RX_LOGICAL_INCLUDED
#define YOCTO_RX_LOGICAL_INCLUDED 1

#include "yocto/rx/syntax/rule.hpp"
#include "yocto/code/fourcc.hpp"
#include "yocto/sequence/vector.hpp"

namespace yocto
{
	
	namespace regex
	{
		
		class grammar;
		
		namespace syntax
		{
			
			
			class logical : public rule
			{
			public:
				virtual ~logical() throw();
				
				logical & operator <<( const string &p );
				logical & operator <<( const char   *p );
				
			protected:
				explicit logical( uint32_t t, const string &n, grammar &G );
				vector<rule *,memory::pooled::allocator> operands;
				
			private:
				YOCTO_DISABLE_COPY_AND_ASSIGN(logical);
			};
			
			
			class aggregate : public logical
			{
			public:
				static const uint32_t ID = YOCTO_FOURCC( 'A','G','G','R' );
				virtual ~aggregate() throw();
				explicit aggregate( const string &n, grammar &g);
				
			private:
				
				YOCTO_DISABLE_COPY_AND_ASSIGN(aggregate);
				virtual syntax_result match( YOCTO_RX_SYNTAX_RULE_ARGS );
			};
			
			class alternative : public logical
			{
			public:
				static const uint32_t ID = YOCTO_FOURCC( 'A','L','T','R' );
				virtual ~alternative() throw();
				explicit alternative( const string &n, grammar &g);
				
				
			private:
				YOCTO_DISABLE_COPY_AND_ASSIGN(alternative);
				virtual syntax_result match( YOCTO_RX_SYNTAX_RULE_ARGS );
			};
			
			
			
		}
		
	
		
	}
}

#endif
