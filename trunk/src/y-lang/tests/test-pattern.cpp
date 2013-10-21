#define _CRT_SECURE_NO_WARNINGS 1
#include "yocto/utest/run.hpp"
#include "yocto/rx/pattern/basic.hpp"
#include "yocto/rx/pattern/logic.hpp"
#include "yocto/rx/pattern/joker.hpp"

#include "yocto/rx/source.hpp"

#include "yocto/ios/icstream.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/code/endian.hpp"
#include "yocto/ios/ocstream.hpp"
#include <cstdlib>
#include <cstdio>
#if defined(_MSC_VER)
#define snprintf _snprintf
#endif
using namespace yocto;

YOCTO_UNIT_TEST_IMPL(pattern)
{
		
	regex::p_list patterns;
	patterns.push_back( regex::basic::any1::create()         );
	patterns.push_back( regex::basic::single::create('A')    );
	patterns.push_back( regex::basic::range::create('a','z') );
	{
		regex::basic::within *w = regex::basic::within::create();
		patterns.push_back(w);
		w->insert('h');
		w->insert('e');
		w->insert('l');
		w->insert('l');
		w->insert('o');
	}
	patterns.push_back( regex::logical::EQUAL( "world" ) );
	patterns.push_back( regex::logical::AMONG( "hello" ) );
	
	{
		regex::basic::single   *s  = regex::basic::single::create('B');
		regex::pattern         *j1 = regex::joker::at_least(s,1);
		patterns.push_back( j1 );
	}
	
	{
		regex::basic::single   *s  = regex::basic::single::create('C');
		regex::pattern         *j1 = regex::joker::counting(s,0,1);
		patterns.push_back( j1 );
	}
	

	int res=0;	
	{
		std::cerr << "-- saving patterns..." << std::endl;
		size_t i=0;
		for( const regex::pattern *p = patterns.head; p; p=p->next)
		{
			char buffer[128]; snprintf( buffer, sizeof(buffer)-1, "g%02x.dot", unsigned(i) );
			ios::ocstream fp( buffer, false );
			p->graphviz( fp, "G" );
			snprintf( buffer, sizeof(buffer)-1, "dot -Tpng g%02x.dot -o g%02x.png", unsigned(i), unsigned(i));
			std::cerr << "" << buffer << "" << std::endl;
			const string cmd = buffer;
			res=system( cmd.c_str() );
			++i;
		}
		std::cerr << "-- done" << std::endl;
	}
	
	regex::p_list pcpy( patterns );
	assert( pcpy.size == patterns.size );
	
	ios::icstream input( ios::cstdin );
	regex::source src(input);
	
	src.prefetch( 1 + alea_lt(16) );
	
	while( src.peek() )
	{
		assert( src.in_cache() > 0 );
		size_t len = 0;
		for( regex::pattern *p = patterns.head; p; p=p->next )
		{
			if( p->accept( src ) )
			{
				const uint32_t tag = swap_be(p->type);
				std::cerr.write( (const char *)&(tag), sizeof(uint32_t) );
				std::cerr << ":'" << *p << "'" << std::endl;
				if( p->size > len ) len = p->size;
				src.unget(*p);
			}
		}
		assert( src.in_cache() >= len );
		src.skip( len );
	}
	
	(void)res;	
}
YOCTO_UNIT_TEST_DONE()
