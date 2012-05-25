#include "yocto/utest/run.hpp"
#include "yocto/functor.hpp"
#include "yocto/string.hpp"
using namespace yocto;

namespace  {
	
	class dummy 
	{
	public:
		explicit dummy(  int a ) :
		factor(a)
		{
			std::cerr << "[+dummy " << factor << "]" << std::endl;
		}
		
		dummy( const dummy &d ) :
		factor( d.factor )
		{
			std::cerr << "[*dummy " << factor << "]" << std::endl;
		}
		
		~dummy() throw()
		{
			std::cerr << "[-dummy " << factor << "]" << std::endl;

		}
		
		int factor;
		
		double operator()( double x )
		{
			return x * factor;
		}
		
		uint32_t h32( const string &s )
		{
			return hash32( s.ro(), s.length());
		}
		
	private:
		YOCTO_DISABLE_ASSIGN(dummy);
	};
	
	static inline double func( double x, double y )
	{
		return x * y;
	}
	
}

YOCTO_UNIT_TEST_IMPL(functor)
{
	dummy d( 2 );
	
	functor<double,TL1(double)> F1( d );
	std::cerr << "F1(3)=" << F1(3) << std::endl;
	
	d.factor = 3;
	functor<int,TL1(float)> F2( d );
	std::cerr << "F2(2.5)=" << F2(2.5) << std::endl;
	std::cerr << "F1(2.5)=" << F1(2.5) << std::endl;

	functor<unsigned,TL1(string)> Fh( &d, & dummy::h32 );
	std::cerr << "F(Hello)=" << Fh( "Hello" ) << std::endl;
	
	functor<double,TL2(double,double)> Fq( cfunctor2(func) );
	
	for( int i=-1; i <= 1; ++i )
	{
		for( int j=-1; j <= 1; ++j )
			std::cerr << " Fq(" << i << "," << j << ")=" << Fq(i,j);
		std::cerr << std::endl;
	}
	
	functor<double, TL1(double)> Fb = bind_first( Fq, 2);
	for( int i=-3; i <= 3; ++i) std::cerr << "Fq(2," << i << ") = " << Fb(i) << std::endl;
	
	
}
YOCTO_UNIT_TEST_DONE()
