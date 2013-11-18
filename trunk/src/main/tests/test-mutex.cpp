#include "yocto/threading/mutex.hpp"
#include "yocto/utest/run.hpp"

using namespace yocto;

//using namespace threading;

YOCTO_UNIT_TEST_IMPL(mutex)
{
	threading::mutex m1( "First Mutex" );
	threading::mutex m2(NULL);


	{
		YOCTO_LOCK(m1);
		YOCTO_LOCK(m2);
	}
	

	{
		if( m1.try_lock() )
		{
			std::cerr << "lock(" << m1.name() << ") success !" << std::endl;
			m1.unlock();
		}
		
		if( m2.try_lock() )
		{
			std::cerr << "lock(" << m2.name() << ") success !" << std::endl;
			m2.unlock();
		}
	}


}
YOCTO_UNIT_TEST_DONE()

