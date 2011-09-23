#ifndef YOCTO_RX_PARSER_INCLUDED
#define YOCTO_RX_PARSER_INCLUDED 1

#include "yocto/rx/syntactic/rule.hpp"
#include "yocto/auto-ptr.hpp"

namespace yocto
{
	
	namespace regex
	{
	
		
		class parser
		{
		public:
			explicit parser( syntactic::rule *root ) throw();
			virtual ~parser() throw();
			
		private:
			auto_ptr<syntactic::rule> root_;
			YOCTO_DISABLE_COPY_AND_ASSIGN(parser);
		};
		
	}
	
}

#endif
