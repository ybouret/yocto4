#include "yocto/utest/run.hpp"
#include "yocto/string/env.hpp"

using namespace yocto;

static inline
void display( const string &name, const string &value, void * )
{
	std::cerr << name << " => " << value << std::endl;
}

YOCTO_UNIT_TEST_IMPL(env)
{
	//-- display all
	environment::get( display, NULL );
	
	for( int i=1; i < argc; ++i )
	{
		const string name = argv[i];
		string value;
		if( environment::get(value,name) )
			std::cerr << "$" << name << " = '" << value << "'" << std::endl;
		else 
			std::cerr << "no $" << name << std::endl;
		
	}
}
YOCTO_UNIT_TEST_DONE()
