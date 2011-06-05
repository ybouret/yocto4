#include "yocto/utest/run.hpp"
#include "yocto/code/unroll.hpp"

using namespace yocto;

static inline void show( size_t i )
{
	std::cerr << i << std::endl;
}

YOCTO_UNIT_TEST_IMPL(unroll)
{
	int i=0;
	YOCTO_LOOP( 5, std::cerr << i++ << std::endl );
	
	i=0;
	YOCTO_LOOP_(5, std::cerr << i++ << std::endl );
	
	i=0;
	YOCTO_LOOP(17, std::cerr << i++ << std::endl );
	
	i=0;
	YOCTO_LOOP_(18, std::cerr << i++ << std::endl );
	
	YOCTO_LOOP_FUNC(5,show,0);
	
	YOCTO_LOOP_FUNC(5,show,1);
	
	YOCTO_LOOP_FUNC(17,show,0);
	
	YOCTO_LOOP_FUNC(18,show,1);
}
YOCTO_UNIT_TEST_DONE()
