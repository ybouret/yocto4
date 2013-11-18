#include "yocto/utest/run.hpp"
#include "yocto/object.hpp"
#include "yocto/sys/wtime.hpp"
#include "yocto/code/rand.hpp"

using namespace yocto;

namespace 
{
	
	class nothing
	{
	public:
		explicit nothing() throw() {}
		virtual ~nothing() throw() {}

		static threading::mutex access;

		static void * operator new( size_t block_size )
		{
			YOCTO_LOCK(access);
			return :: operator new( block_size );
		}

		static void operator delete( void *p, size_t block_size ) throw()
		{
			YOCTO_LOCK(access);
			:: operator delete( p );
		}


		
	private:
		YOCTO_DISABLE_COPY_AND_ASSIGN(nothing);
	};
	

	threading::mutex nothing::access( "nothing" );

	template <size_t N, typename BASE>
	class dummy : public BASE
	{
	public:
		inline    dummy() throw() {}
		virtual  ~dummy() throw()  {}
		
	private:
		YOCTO_DISABLE_COPY_AND_ASSIGN(dummy);
	};
	
	
	
	
	template <size_t N, typename T>
	static inline
	double do_test( size_t count )
	{
		const size_t iter_max     = 16;
		const size_t scramble_max = 8;
		
		typedef dummy<N,T> dum_t;
		
		dum_t **pDum = memory::kind<memory::global>::acquire_as<dum_t *>( count );
		
		try 
		{
			double ell = 0;
			wtime chrono;
			for( size_t iter = 0; iter < iter_max; ++iter )
			{
				chrono.start();
				for( size_t i=0; i < count; ++i ) pDum[i] = new dum_t();
				ell += chrono.query();
				const size_t nfree = count/2;
				for( size_t scramble = 0; scramble < scramble_max; ++scramble )
				{
					c_shuffle(pDum, count);
					chrono.start();
					for( size_t i=0; i < nfree; ++i )
					{
						delete pDum[i];
					}
					for( size_t i=0; i < nfree; ++i )
					{
						pDum[i] = new dum_t();
					}
					ell += chrono.query();
				}
				
				
				
				c_shuffle(pDum, count);
				chrono.start();
				for( size_t i=0; i < count; ++i ) delete pDum[i];
				ell += chrono.query();
			}
			
			ell /= (iter_max * count );
			ell *= 1.0e6;
			
			memory::kind<memory::global>::release_as<dum_t *>( pDum, count );
			return ell;
		}
		catch (...) 
		{
			memory::kind<memory::global>::release_as<dum_t *>( pDum, count );
			throw;
		}
		
		return 0;
	}
}

#include <cstdlib>

#define DO_TEST(N,TYPE) do { const double res = do_test<N,TYPE>(count); std::cerr << "do_test<"  << N << "," #TYPE ">(" << count << "): " << res << std::endl; } while( false )
YOCTO_UNIT_TEST_IMPL(object)
{
	size_t count = 0;
	
	if( argc > 1 )
		count = strtol( argv[1], NULL, 10 );
	
	if( count <= 0 )
		count = 100;
	
	DO_TEST(2,nothing);
	DO_TEST(2,object);
	
	DO_TEST(YOCTO_LIMIT_SIZE/2,nothing);
	DO_TEST(YOCTO_LIMIT_SIZE/2,object);
	
	DO_TEST(2*YOCTO_LIMIT_SIZE,nothing);
	DO_TEST(2*YOCTO_LIMIT_SIZE,object);
	
}
YOCTO_UNIT_TEST_DONE()

