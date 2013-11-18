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
                
                void operator()( rule &r );
                
                compound & operator<<( rule & );
                
			protected:
				explicit compound( const string &id);
                compound( const compound &other );
                vector<rule::ptr> operands;
                
			private:
				YOCTO_DISABLE_ASSIGN(compound);
			};
		}
	}
    


}

#endif
