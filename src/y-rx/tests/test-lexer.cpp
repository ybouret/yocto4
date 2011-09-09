#include "yocto/utest/run.hpp"
#include "yocto/rx/lexical/engine.hpp"
#include "yocto/rx/pattern/posix.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/rx/source.hpp"

using namespace yocto;
using namespace regex;

namespace
{
	class dummy 
	{
	public:
		explicit dummy() 
		{
		}
		
		virtual ~dummy() throw()
		{
		}
		
		YLEX_CB(display,t)
		{
			std::cerr << '[' << t << ']';
		}
		
		YLEX_CB(newline,)
		{
			std::cerr << " [newline]" << std::endl;
		}
		
		YLEX_CB(discard,) throw()
		{
			
		}
		
	private:
		YOCTO_DISABLE_COPY_AND_ASSIGN(dummy);
	};
}

YOCTO_UNIT_TEST_IMPL(lexer)
{
	lexical::engine l;
	dummy           d;

	l.make( "[:endl:]",   d, & dummy:: newline );
	l.make( "[:word:]+",  d, & dummy:: display );
	l.make( "[:word:]+_", d, & dummy:: display );
	l.make( "[:blank:]+", d, & dummy:: discard );
	
	l.stop( "@", d, & dummy::display );
	l.make( ".", d, & dummy::display );
	
	ios::icstream inp( ios::cstdin );
	source        src;
	src.connect(inp);
	
	l.reset(src);
	if( ! l.run(src) )
	{
		std::cerr << std::endl << "unexpected char..." << std::endl;
	}
	else {
		std::cerr << std::endl << "normal exit" << std::endl;
	}

	
	
	
}
YOCTO_UNIT_TEST_DONE()
