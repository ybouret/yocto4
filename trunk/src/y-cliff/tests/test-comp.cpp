#include "yocto/utest/run.hpp"

#include "yocto/cliff/components.hpp"

using namespace yocto;
using namespace cliff;


namespace  {
	
	static inline void display( const components &C )
	{
		for( size_t i=C.cmin; i <= C.cmax; ++i )
		{
			std::cerr << "component #" << i << " = " << C.get_name(i) << std::endl;
		}
		std::cerr << std::endl;
	}
	
}

YOCTO_UNIT_TEST_IMPL(comp)
{

	{
		components C(1,10,NULL);
		display(C);
		C.set_name(3,"toto");
		display(C);
	}
	
	{
		const char *varnames[] = { "u", "v" };
		components C(0,sizeof(varnames)/sizeof(varnames[0]),varnames);
		display(C);
	}
	
}
YOCTO_UNIT_TEST_DONE()
