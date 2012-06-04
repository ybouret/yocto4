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
                
                //! direct rule append
                void add( rule *r ) throw();
                
                //! cloning rule
                void add( const rule &r );
                
			protected:
				explicit compound( const string &id);
                compound( const compound &other );
				rules operands;

			private:
				YOCTO_DISABLE_ASSIGN(compound);
			};
		}
	}
    


}

#endif
