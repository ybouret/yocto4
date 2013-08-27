#include "yocto/utest/run.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/threading/mutex.hpp"

using namespace yocto;

template <typename T>
static inline void show( const T tab[], size_t num )
{
	for( size_t i=0; i < num; ++i) std::cerr << tab[i] << ' ';
	std::cerr << std::endl;
}

YOCTO_UNIT_TEST_IMPL(rand)
{
	_rand.wseed();
	{
		YOCTO_GIANT_LOCK();
		static const size_t N = 1000;
		double              r[N];
		double              sum = 0;
		for( size_t i=0; i < N; ++i )
		{
			r[i] = alea<double>();
			sum += r[i];
		}
		const double ave = sum /= N;
		std::cerr << "<r>= " << ave << std::endl;
	}
	
	{
		YOCTO_GIANT_LOCK();
		int tab[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
		const size_t num = sizeof(tab)/sizeof(tab[0]);
		show( tab, num );
		_rand.shuffle<int>(tab,num);
		show( tab, num );
	}
    
    {
        uniform_generator<double,rand32_kiss> ran;
        static const  size_t num = 10;
        double               tab[num];
        for(size_t i=0; i < num; ++i)
        {
            tab[i] = ran();
        }
        show(tab,num);
        for(size_t i=0; i < num; ++i)
        {
            tab[i] = ran.lt(num);
        }
        show(tab,num);
    }
	
}
YOCTO_UNIT_TEST_DONE()
