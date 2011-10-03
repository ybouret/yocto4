#ifndef YOCTO_RX_SYN_LOGICAL_INCLUDED
#define YOCTO_RX_SYN_LOGICAL_INCLUDED 1

#include "yocto/rx/syntactic/rule.hpp"
#include "yocto/code/fourcc.hpp"

namespace yocto
{
	
	namespace regex
	{
		
		namespace syntactic
		{

			class logical : public rule
			{
			public:
				virtual ~logical() throw();
				
				logical & operator<<( rule *r ) throw();
				
				
			protected:
				rules operands;
				explicit logical( uint32_t t, const string &id);
				logical( const logical & );
				
			private:
				YOCTO_DISABLE_ASSIGN(logical);
			};
			
			class AND : public logical
			{
			public:
				static const uint32_t ID = YOCTO_FOURCC('[','&','&', ']');
				virtual ~AND() throw();
				
				static AND * create( const string &id);
				static AND * create( const char   *id);
				
				virtual  syntax::result match( YOCTO_RX_SYNTAX_RULE_MATCH_ARGS );
				virtual  rule *clone() const;
				
			private:
				explicit AND( const string &id);

				YOCTO_DISABLE_COPY_AND_ASSIGN(AND);
				
			};
			
			class OR : public logical
			{
			public:
				static const uint32_t ID = YOCTO_FOURCC('[','|','|', ']');
				virtual ~OR() throw();
				
				static OR * create( const string &id);
				static OR * create( const char   *id);
				
				virtual  syntax::result match( YOCTO_RX_SYNTAX_RULE_MATCH_ARGS );
				virtual  rule *clone() const;
				
			private:
				explicit OR( const string &id);
				
				YOCTO_DISABLE_COPY_AND_ASSIGN(OR);
				
			};
			
			
			
		}
		
		typedef syntactic::logical s_logical;
		typedef syntactic::AND     s_and;
		typedef syntactic::OR      s_or;
	}

}
#endif
