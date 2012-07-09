#ifndef YOCTO_CODE_RAND_INCLUDED
#define YOCTO_CODE_RAND_INCLUDED 1

#include "yocto/code/rand32.hpp"

namespace yocto
{
	
#if 0
	class urand
	{
	public:
		uint32_t w;
		uint32_t z;
		uint32_t jsr;
		uint32_t jcong;
		
		explicit urand() throw();
		explicit urand( uint32_t seed ) throw();
		virtual ~urand() throw();
		void     reset( uint32_t seed ) throw();
		void     wseed();
		
		uint32_t MWC() throw();
		uint32_t SHR3() throw();
		uint32_t CONG() throw();
		uint32_t KISS() throw();
		
		template <typename T> T get() throw(); //!< uint32_t, float, double
		template <typename T>
		inline T full()
		{
			static const T one(1);
			T ans(0);
			for( size_t i=sizeof(T)<<3;i>0;--i)
			{
				ans <<= 1;
				if( get<float>() > 0.5f ) ans |= one;
			}
			return ans;
		}
		
		inline size_t less_than( const size_t some_number )
		{
			assert(some_number>0);
			return full<size_t>() % some_number;
		}
		
		template <typename T>
		void c_shuffle( T *base, size_t n ) throw()
		{
			assert( !(base==NULL && n>0) );
			if( n > 1 )
			{
				const size_t m=n-1;
				for( size_t i=0; i < m; ++i, ++base )
				{
					mswap<T>(base[0],base[ less_than(n-i) ]);
				}
			}
		}
		
	private:
		YOCTO_DISABLE_COPY_AND_ASSIGN(urand);
	};
#endif
    
	extern urand32 &_rand;
	
	//! thread unsafe: float, double uint32_t
	template <typename T> inline T    alea(void)    throw() { return _rand.get<T>(); }	

	
	//! thread unsafe: integer type
	template <typename T> inline T    alea_of(void) throw() { return _rand.full<T>(); }
	
	//! thread unsafe: 0..some_number
	inline size_t                     alea_less_than( const size_t some_number )  throw() { return _rand.less_than(some_number); }
	
	//! thread unsafe
	template <typename T> inline void alea_shuffle( T *base, size_t n ) throw() { _rand.shuffle(base,n);    }
}

#endif
