#ifndef YOCTO_RX_LEXER_INCLUDED
#define YOCTO_RX_LEXER_INCLUDED 1



#include "yocto/rx/pattern.hpp"
#include "yocto/rx/lexical/engine.hpp"
#include "yocto/associative/set.hpp"

namespace yocto
{
	namespace regex
	{
		
		class lexer : public object
		{
		public:
			explicit lexer( label_type init );
			virtual ~lexer() throw();
			
			//! create a new engine with label=id
			void declare( label_type id );
			
			
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(lexer);
			lexical::engine *current_; //!< current processing engine
			lexical::engines engines_; //!< engines database
		};
	}
	
}



#endif
