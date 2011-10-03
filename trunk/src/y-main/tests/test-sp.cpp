#include "yocto/utest/run.hpp"
#include "yocto/shared-ptr.hpp"
#include "yocto/string.hpp"

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

#include "yocto/intrusive-ptr.hpp"

namespace 
{
	
	class dummy 
	{
	public:
		int value;
		dummy( int a ) : value(a), nref_(0)
		{
			std::cerr << "+[dummy " << value << "]" << std::endl;
		}
		
		~dummy() throw()
		{
			std::cerr << "-[dummy " << value << "]" << std::endl;
		}
	
		typedef intrusive_ptr<dummy> ptr;
		
		void withhold() throw() { ++nref_; }
		bool liberate() throw() { assert(nref_>0); return --nref_ <= 0; }
		
	private:
		size_t nref_;
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
