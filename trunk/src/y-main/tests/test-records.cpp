#include "yocto/utest/run.hpp"
#include "yocto/memory/records.hpp"
#include "yocto/memory/buffers.hpp"

#include "yocto/code/round.hpp"

#include "yocto/memory/pooled.hpp"
#include "yocto/memory/global.hpp"

using namespace yocto;

namespace {
	
	class dummy 
	{
	public:
		explicit dummy( int a ) : value(a)
		{
			std::cerr << "+[" << value << "]" << std::endl;
		}
		
		int value;
		
		~dummy() throw() { std::cerr << "-[" << value << "]" << std::endl; }
		
		dummy( const dummy &d ) : value( d.value ) { 	std::cerr << "*[" << value << "]" << std::endl; }
		
		
		
	private:
		YOCTO_DISABLE_ASSIGN(dummy);
	};
	
}

YOCTO_UNIT_TEST_IMPL(records)
{

	static const size_t N = 10;
	memory::buffer_of<dummy,memory::pooled> pbuf(N);
	memory::buffer_of<dummy,memory::global> gbuf(N);
	memory::records_of<dummy> D( pbuf(), pbuf.size() );
	
	std::cerr << "-- inserting" << std::endl;
	for( size_t i=0; i < N; ++i )
	{
		const dummy tmp( i );
		D.append( tmp );
	}
	std::cerr << "-- done" << std::endl;
	
	std::cerr << "-- rebase" << std::endl;
	D.rebase( gbuf(), gbuf.size() );
	
	std::cerr << "-- inserting" << std::endl;
	for( size_t i=0; i < N; ++i )
	{
		const dummy tmp( i );
		D.append( tmp );
	}
	std::cerr << "-- done" << std::endl;
}
YOCTO_UNIT_TEST_DONE()
