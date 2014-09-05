#ifndef YOCTO_LANG_SYNTAX_LOGIC_INCLUDED
#define YOCTO_LANG_SYNTAX_LOGIX_INCLUDED 1

#include "yocto/lang/syntax/rule.hpp"


namespace yocto
{
	namespace lang
	{

		namespace syntax
		{


			class logical : public rule
			{
			public:
				class operand : public object
				{
				public:
					rule    &sub;
					operand *next;
					operand *prev;
					explicit operand( rule & ) throw();
					virtual ~operand() throw();

				private:
					YOCTO_DISABLE_COPY_AND_ASSIGN(operand);
				};

				core::list_of_cpp<operand> operands;

				void append( rule &sub );
				virtual ~logical() throw();

				logical & operator<<( rule &sub ); 

			protected:
				explicit logical(const string &id);


			private:
				YOCTO_DISABLE_COPY_AND_ASSIGN(logical);
			};


			class aggregate : public logical
			{
			public:
                const syntax::xnode_ppty behavior;
				explicit aggregate(const string &id,const syntax::xnode_ppty ppty);
				virtual ~aggregate() throw();

				aggregate & operator+=( rule & );

				YOCTO_LANG_SYNTAX_RULE_MATCH_DECL();

			private:
				YOCTO_DISABLE_COPY_AND_ASSIGN(aggregate);
			};


			class alternate : public logical
			{
			public:
				explicit alternate(const string &id);
				virtual ~alternate() throw();

				alternate & operator|=( rule & );
				YOCTO_LANG_SYNTAX_RULE_MATCH_DECL();

			private:
				YOCTO_DISABLE_COPY_AND_ASSIGN(alternate);
			};


		}

	}

}


#endif

