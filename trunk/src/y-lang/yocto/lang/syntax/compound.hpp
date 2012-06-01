#ifndef YOCTO_LANG_SYNTAX_COMPOUND_INCLUDED
#define YOCTO_LANG_SYNTAX_COMPOUND_INCLUDED 1

#include "yocto/lang/syntax/rule.hpp"

namespace yocto 
{
	namespace lang
	{

		namespace syntax
		{
			class compound : public rule
			{
			public:
				virtual ~compound() throw();
                void add( rule *r ) throw();
                
			protected:
				explicit compound( const string &id);
				rules operands;

			private:
				YOCTO_DISABLE_COPY_AND_ASSIGN(compound);
			};
		}
	}
    


}

#endif
