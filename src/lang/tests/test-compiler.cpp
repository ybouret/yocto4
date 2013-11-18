#include "yocto/utest/run.hpp"
#include "yocto/rx/compiler.hpp"
#include "yocto/rx/pattern/posix.hpp"
#include "yocto/ptr/auto.hpp"
#include "yocto/ios/ocstream.hpp"
#include "yocto/ios/osstream.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/ios/imstream.hpp"
#include "yocto/rx/source.hpp"

#include <cstring>
#include <cstdlib>

using namespace yocto;


YOCTO_UNIT_TEST_IMPL(comp_rx)
{
	if( argc > 1 )
	{
		int res=0;
		regex::pattern_dict dict;
		dict.record( "DIGIT", regex::posix::digit() );
	        dict.record( "DIGITS", "[1-9]{DIGIT}*" );
		const string expr = argv[1];
		std::cerr << "-- compiling '" << expr << "'" << std::endl;
		auto_ptr<regex::pattern> p( regex::compile( expr, &dict ) );
		std::cerr << "-- done" << std::endl;
		dict.release();
		{
			ios::ocstream fp( "expr.dot", false );
			p->graphviz( fp, "G" );
		}
		
		res = system("dot -Tpng expr.dot -o expr.png");
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
				regex::source src(im);

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
		(void)res;	
	}
	
}
YOCTO_UNIT_TEST_DONE()
