#include "yocto/utest/run.hpp"
#include "yocto/core/locate.hpp"
#include "yocto/memory/buffers.hpp"
#include "yocto/code/rand.hpp"

using namespace yocto;

static inline int compare( int a, int b ) throw()
{
	return a-b;
}

using namespace memory;

YOCTO_UNIT_TEST_IMPL(locate)
{
	{
		int          tab[] = { 1, 2, 4, 7, 8 , 9 , 11, 20 };
		const size_t num   = sizeof(tab)/sizeof(tab[0]); 
		
		for( size_t i=0; i < num; ++i )
		{
			size_t indx = 0;
			const int target = tab[i];
			std::cerr << "locate " << target << "..." << std::endl;
			if( !core::locate( &target, tab, num, indx, compare ) )
			{
				std::cerr << "Can't find " << target << " !!!" << std::endl;
			}
			else
			{
				if( indx != i )
					std::cerr << "Not the right position!" << std::endl;
				else 
				{
					std::cerr << "@" << indx << std::endl;
				}
				
			}		
		}
		
		{
			size_t indx = 0;
			int target = -1;
			if( !core::locate( &target, tab, num, indx, compare ) )
			{
				std::cerr << "should insert " << target << "@" << indx << std::endl;
			}
			
			target = 3;
			if( !core::locate( &target, tab, num, indx, compare ) )
			{
				std::cerr << "should insert " << target << "@" << indx << std::endl;
			}
			
			
			target = 10;
			if( !core::locate( &target, tab, num, indx, compare ) )
			{
				std::cerr << "should insert " << target << "@" << indx << std::endl;
			}
			
			target = 99;
			if( !core::locate( &target, tab, num, indx, compare ) )
			{
				std::cerr << "should insert " << target << "@" << indx << std::endl;
			}
			
		}
		
	}
	

	{
		size_t n = 20;
		size_t j = 0;
		buffer_of<int,global> tab( n );
		std::cerr << "raw=";
		for( size_t i=0; i < n; ++i )
		{
			const int x    = alea_leq(1000);
			size_t    indx = 0;
			if( !core::locate( &x, tab(), j, indx, compare ) )
			{
				core::insert( &x, tab(), j, indx ); ++j;
				std::cerr << " " << x;
			}
		}
		std::cerr << std::endl;
		
		std::cerr << "ord=";
		for( size_t i=0; i < j; ++i )
		{
			std::cerr << " " << *tab(i);
		}
		std::cerr << std::endl;
		
	}
	
	
}
YOCTO_UNIT_TEST_DONE()



