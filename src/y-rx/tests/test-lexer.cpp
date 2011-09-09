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
	lexical::engine s;
	dummy           d;
	
	{
		const lexical::action on_newline( &d, & dummy::newline );
		const lexical::action on_display( &d, & dummy::display );
		const lexical::action on_discard( &d, & dummy::discard );

		l.make( "[:endl:]",   on_newline );
		l.make( "[:word:]+",  on_display );
		l.make( "[:word:]+_", on_display );
		l.make( "[:blank:]+", on_discard );
		
		l.stop( "@", on_display );
		l.make( ".", on_display );
	}
	
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
