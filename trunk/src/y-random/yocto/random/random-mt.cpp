#include "yocto/random/uniform-mt.hpp"

namespace yocto {
	
	namespace Random {
		
		static const int          N  = 624;
		static const int          M  = 397;
		
#define               MATRIX_A      0x9908b0dfUL  /* constant vector a */
		static const uint32_t UPPER_MASK  = 0x80000000UL; /* most significant w-r bits */
		static const uint32_t LOWER_MASK  = 0x7fffffffUL; /* least significant r bits */
		
		
		UniformMT:: UniformMT( Bits &s ) throw() :
		mti(0)
		{
			this->set(s);
		}
		
		UniformMT:: ~UniformMT() throw() {
		}
		
		void UniformMT:: seed( Bits &s ) throw() 
		{
			this->set( s );
		}
		
		void UniformMT:: set( Bits & s ) throw() 
		{
			mt[0]= s.full<unsigned long>() & 0xffffffffUL;
			for(mti=1; mti<N; mti++)
			{
				mt[mti] =
				(1812433253UL * (mt[mti-1] ^ (mt[mti-1] >> 30)) + mti);
				/* See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier. */
				/* In the previous versions, MSBs of the seed affect   */
				/* only MSBs of the array mt[].                        */
				/* 2002/01/09 modified by Makoto Matsumoto             */
				mt[mti] &= 0xffffffffUL;
				/* for >32 bit machines */
			}
		}
		
		static uint32_t  genrand_uint32( int &mti, unsigned long mt[] ) throw() {
			static unsigned long mag01[2]={0x0UL, MATRIX_A};
			/* mag01[x] = x * MATRIX_A  for x=0,1 */
			unsigned long y;
			
			if (mti >= N)
			{
				/* generate N words at one time */
				int kk;
				
				
				
				for (kk=0;kk<N-M;kk++)
				{
					y = (mt[kk]&UPPER_MASK)|(mt[kk+1]&LOWER_MASK);
					mt[kk] = mt[kk+M] ^ (y >> 1) ^ mag01[y & 0x1UL];
				}
				for (;kk<N-1;kk++)
				{
					y = (mt[kk]&UPPER_MASK)|(mt[kk+1]&LOWER_MASK);
					mt[kk] = mt[kk+(M-N)] ^ (y >> 1) ^ mag01[y & 0x1UL];
				}
				y = (mt[N-1]&UPPER_MASK)|(mt[0]&LOWER_MASK);
				mt[N-1] = mt[M-1] ^ (y >> 1) ^ mag01[y & 0x1UL];
				
				mti = 0;
			}
			
			y = mt[mti++];
			
			/* Tempering */
			y ^= (y >> 11);
			y ^= (y << 7)  & 0x9d2c5680UL;
			y ^= (y << 15) & 0xefc60000UL;
			y ^= (y >> 18);
			
			return uint32_t(y);
		}
		
		double UniformMT:: operator()(void) throw() 
		{
			return ( 0.5 + (double)(genrand_uint32(mti,mt))) / ( 4294967296.0 );
		}
		
		
		
	}
	
}


