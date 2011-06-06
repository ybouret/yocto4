#include "yocto/threading/singleton.hpp"
#include "yocto/utest/run.hpp"


using namespace yocto;

namespace  {
	
	class dummy : public singleton<dummy>
	{
	public:
		
		const char * get_name() const throw() { return name; }
		
	private:
		inline dummy() throw() {}
		inline virtual ~dummy() throw() {}
		
		friend class singleton<dummy>;
		static const char name[];
		static const threading::longevity life_time = 100;
	};
	
	const char dummy::name[] = "dummy";
}

YOCTO_UNIT_TEST_IMPL(singleton)
{

	dummy &d = dummy::instance();
	
	std::cerr << d.get_name() << std::endl;
	
	
}
YOCTO_UNIT_TEST_DONE()
