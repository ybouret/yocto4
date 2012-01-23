#include "yocto/utest/run.hpp"

#include "yocto/cliff/components.hpp"

using namespace yocto;
using namespace cliff;


namespace  {
	
	static inline void display( const components &C )
	{
		for( size_t i=1; i <= C.number; ++i )
		{
			std::cerr << "component #" << i << " = " << C.name(i) << std::endl;
		}
		std::cerr << std::endl;
	}
	
}

YOCTO_UNIT_TEST_IMPL(comp)
{

	{
		components C(10,NULL);
		display(C);
		C.name(3,"toto");
		display(C);
	}
	
	{
		const char *varnames[] = { "u", "v" };
		components C(sizeof(varnames)/sizeof(varnames[0]),varnames);
		display(C);
	}
	
}
YOCTO_UNIT_TEST_DONE()
