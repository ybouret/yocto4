#ifndef YOCTO_RX_LEXICAL_ATOM_INCLUDED
#define YOCTO_RX_LEXICAL_ATOM_INCLUDED 1

#include "yocto/rx/lexical/rule.hpp"

namespace yocto
{
	namespace regex
	{
		namespace lexical
		{
			
			class atom : public token
			{
			public:
				atom         *next;
				atom         *prev;
				const string &label;
				
				static atom *create( rule &r );
				
				//! destroy with caching
				static void  destroy( atom *a, source &src ) throw();
				
				//! destroy without caching
				static void  destroy( atom *a ) throw();
				
			private:
				//! steal token from rule->motif
				explicit atom( rule &from ) throw();
				virtual ~atom() throw();
				
				YOCTO_DISABLE_COPY_AND_ASSIGN(atom);
			};
			
			
			class atoms : public core::list_of<atom>
			{
			public:
				explicit atoms() throw();
				virtual ~atoms() throw();
				void     to( source &src ) throw(); //!< return all tokens to source
				
			private:
				YOCTO_DISABLE_COPY_AND_ASSIGN(atoms);
			};
			
		}
	}
}

#endif
