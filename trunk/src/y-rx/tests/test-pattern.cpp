#include "yocto/utest/run.hpp"
#include "yocto/rx/pattern/basic.hpp"
#include "yocto/rx/pattern/logic.hpp"

#include "yocto/rx/source.hpp"

#include "yocto/ios/icstream.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/code/endian.hpp"
#include "yocto/ios/ocstream.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(pattern)
{
	ios::icstream input( ios::cstdin );
	regex::source src;
	
	src.connect( input );
	src.prefetch( 1 + alea_less_than<size_t>(16) );
	
	regex::p_list patterns;
	patterns.push_back( regex::basic::any1::create()         );
	patterns.push_back( regex::basic::single::create('A')    );
	patterns.push_back( regex::basic::range::create('a','z') );
	{
		regex::basic::within *w = regex::basic::within::create();
		patterns.push_back(w);
		w->add('h');
		w->add('e');
		w->add('l');
		w->add('l');
		w->add('o');
	}
	patterns.push_back( regex::logical::EQUAL( "world" ) );
	patterns.push_back( regex::logical::AMONG( "hello" ) );
	
	{
		size_t i=0;
		for( const regex::pattern *p = patterns.head; p; p=p->next)
		{
			char buffer[32]; snprintf( buffer, sizeof(buffer)-1, "g%04x.dot", unsigned(i++) );
			ios::ocstream fp( buffer, false );
			p->graphviz( fp, "G" );
		}
	}
	
	regex::p_list pcpy( patterns );
	assert( pcpy.size == patterns.size );
	
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
	
	
}
YOCTO_UNIT_TEST_DONE()
