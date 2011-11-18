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
			/**
			 Created by a lexer from a lexical::rule.
			 */
			class atom : public token
			{
			public:
				atom         *next;  //!< for core::list  
				atom         *prev;  //!< for core::list  
				const string &label; //!< from a rule in a lexer database: no duplicate memory
				
				static atom *create( rule &r );
				
				
				//! destroy atom
				static void  destroy( atom *a ) throw();
				
			private:
				//! steal token from rule->motif
				explicit atom( rule &from ) throw();
				virtual ~atom() throw();
				
				YOCTO_DISABLE_COPY_AND_ASSIGN(atom);
			};
			
			//! a list of atoms
			class atoms : public core::list_of<atom>
			{
			public:
				explicit atoms() throw();
				virtual ~atoms() throw();
				void     clear() throw(); //!< clear all atoms
				
			private:
				YOCTO_DISABLE_COPY_AND_ASSIGN(atoms);
			};
			
		}
	}
}

#endif
