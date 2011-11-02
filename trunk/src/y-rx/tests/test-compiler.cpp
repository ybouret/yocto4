#include "yocto/utest/run.hpp"
#include "yocto/rx/compiler.hpp"
#include "yocto/rx/pattern/posix.hpp"
#include "yocto/auto-ptr.hpp"
#include "yocto/ios/ocstream.hpp"
#include "yocto/ios/osstream.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/ios/imstream.hpp"
#include "yocto/rx/source.hpp"

#include <cstring>
#include <cstdlib>

using namespace yocto;


YOCTO_UNIT_TEST_IMPL(compiler)
{
	if( argc > 1 )
	{
		regex::pattern_db dict;
		dict.record( "DIGIT", regex::posix::digit() );
		const string expr = argv[1];
		std::cerr << "-- compiling '" << expr << "'" << std::endl;
		auto_ptr<regex::pattern> p( regex::compile( expr, &dict ) );
		std::cerr << "-- done" << std::endl;
		dict.release();
		{
			ios::ocstream fp( "expr.dot", false );
			p->graphviz( fp, "G" );
		}
		
		system("dot -Tpng expr.dot -o expr.png");
		{
			regex::first_chars fch;
			p->gather(fch);
			std::cerr << "-- first chars" << std::endl;
			std::cerr << fch << std::endl;
			std::cerr << "-- done" << std::endl;
		}
		
		{
			ios::ocstream fp( "expr.bin", false );
			p->brx( fp );
		}
		
		{
			std::cerr << "-- pattern reloading..." << std::endl;
			ios::icstream fp( "expr.bin" );
			auto_ptr<regex::pattern> q( regex::pattern::load(fp) );
		
			string        bin_p;
			string        bin_q;
			ios::osstream out_p( bin_p );
			ios::osstream out_q( bin_q );
			p->brx( out_p );
			q->brx( out_q );
			if( bin_p != bin_q )
				throw exception("pattern mismatch!");
		}
		
		
		
		
		if( (argc<=2) || 0 != strcmp(argv[2],"no") )
		{
			ios::icstream input( ios::cstdin );
			regex::source src;
			string line;
			
			for(;;)
			{
				//-- read line
				(std::cerr << "> ").flush();
				line.clear();
				if( input.read_line(line) < 0 )
				{
					break;
				}
				
				//-- read input
				std::cerr << "# ";
				ios::imstream im( line );
				src.connect( im );
				while( src.peek() )
				{
					if( p->accept( src ) )
					{
						std::cerr << "<" << *p << ">";
						p->clear();
					}
					else {
						std::cerr << src.peek()->data;
						src.skip(1);
					}
				}
				std::cerr << std::endl;
				
			}
			
		}
		
	}
	
}
YOCTO_UNIT_TEST_DONE()
