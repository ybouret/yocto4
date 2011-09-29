#ifndef YOCTO_RX_LEXICAL_ATOM_INCLUDED
#define YOCTO_RX_LEXICAL_ATOM_INCLUDED 1

#include "yocto/rx/lexical/rule.hpp"

namespace yocto
{
	namespace regex
	{
		namespace lexical
		{
			
			//! a token with its label
			class atom : public token
			{
			public:
				
								
				atom         *next; //!< for core::list binary layout
				atom         *prev; //!< for core::list binary layout
				const string &label; //!< from the lexer database: no duplicate memory
				
				static atom *create( rule &r );
				
				//! destroy with caching used t_char
				static void  destroy( atom *a, t_char::pool &tp) throw();
				
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
				void     to( t_char::pool &tp ) throw(); //!< all atoms back_to(tp)
				
			private:
				YOCTO_DISABLE_COPY_AND_ASSIGN(atoms);
			};
			
		}
	}
}

#endif
