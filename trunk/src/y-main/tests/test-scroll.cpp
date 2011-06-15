#include "yocto/utest/run.hpp"
#include "yocto/sequence/scroll.hpp"
#include "yocto/string.hpp"

#include "support.hpp"

using namespace yocto;

namespace
{



	template <typename T>
	static inline
	void perform_scroll( scroll<T> &S )
	{
		std::cerr << "Filling Scroll" << std::endl;
		while( S.size() < 1000 )
		{
			S.push_back( gen<T>::get() );
		}

		const size_t s0 = S.size();
		if( s0 <= 0 ) throw exception("Invalid initial size");

		std::cerr << "\tat_next" << std::endl;
		for( size_t k=0; k < 1024; ++k )
		{
			S.at_next( gen<T>::get() );
			assert( S.size() == s0 );
		}

		std::cerr << "\tat_prev" << std::endl;
		for( size_t k=0; k < 1024; ++k )
		{
			S.at_prev( gen<T>::get() );
			assert( S.size() == s0 );
		}

		std::cerr << std::endl;
	}

}

YOCTO_UNIT_TEST_IMPL(scroll)
{
	std::cerr << "Scrolling..." << std::endl;
	{
		scroll<int> S;
		perform_scroll( S  );
	}

	{
		scroll<string> S;
		perform_scroll( S  );
	}

	{
		scroll<double> S;
		perform_scroll( S );
	}


}
YOCTO_UNIT_TEST_DONE()

namespace
{
	template <typename T>
	static inline
	void perform_autoscroll( autoscroll<T> &S )
	{
		for( size_t w=1; w <= 128; ++w )
		{
			S.set_width( w );
			for( size_t k=0; k < 256; ++k )
				S.record( gen<T>::get() );
		}

		for( size_t w=127; w > 0; --w )
		{
			S.set_width( w );
			for( size_t k=0; k < 256; ++k )
				S.record( gen<T>::get()  );
		}
	}
}



YOCTO_UNIT_TEST_IMPL(auto_scroll)
{
	std::cerr  << "AutoScrolling..." << std::endl;

	{

		autoscroll<int> S(8);
		perform_autoscroll( S );
	}

	{

		autoscroll<string> S(5);
		perform_autoscroll( S  );
	}
	{

		autoscroll<double> S(90);
		perform_autoscroll( S  );
	}


}
YOCTO_UNIT_TEST_DONE()

