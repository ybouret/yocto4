#include "yocto/utest/run.hpp"
#include "yocto/ptr/shared.hpp"
#include "yocto/string.hpp"
#include "yocto/ptr/arc.hpp"

using namespace yocto;


YOCTO_UNIT_TEST_IMPL(shared_ptr)
{
	shared_ptr<string> pS1( new string("Hello") );
	shared_ptr<string> pS2( pS1 ); 

	std::cerr << *pS1 << std::endl;
	std::cerr << *pS2 << std::endl;
	
	*pS1 = "World";

	std::cerr << *pS1 << std::endl;
	std::cerr << *pS2 << std::endl;


}
YOCTO_UNIT_TEST_DONE()

#include "yocto/ptr/intr.hpp"
#include "yocto/counted.hpp"

namespace 
{
	static uint32_t dummy_idx = 0;
	
	class dummy : public counted
	{
	public:
		int value;
		const uint32_t id;
		dummy( int a ) : value(a), id( ++dummy_idx)
		{
			std::cerr << "+[dummy " << value << "]" << std::endl;
		}
		
		~dummy() throw()
		{
			std::cerr << "-[dummy " << value << "]" << std::endl;
		}
	
		typedef intr_ptr<uint32_t,dummy> ptr;
		
	
		
	private:
		YOCTO_DISABLE_COPY_AND_ASSIGN(dummy);
	};
	
}


YOCTO_UNIT_TEST_IMPL(intrusive_ptr)
{
	dummy::ptr pD1( new dummy( 7 ) );
	dummy::ptr pD2( pD1 );
	
	std::cerr << pD1->value   << std::endl;
	std::cerr << pD2->value++ << std::endl;
	std::cerr << pD1->value   << std::endl;
}
YOCTO_UNIT_TEST_DONE()