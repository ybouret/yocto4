#include "yocto/random/uniform3.hpp"
#include "yocto/code/utils.hpp"

namespace yocto {
	
	namespace Random {
		
		static const int32_t MBIG  = 1000000000;
		static const int32_t MSEED = 161803398;
		static const int32_t MZ    = 0;
		
		Uniform3:: Uniform3( Bits & s ) throw() :
		inext_(0),
		inextp_(0)
		{
			this->set( s );
		}
		
		Uniform3:: ~Uniform3() throw() {
			
		}
		
		void Uniform3:: seed( Bits &s ) throw() {
			this->set( s );
		}
		
		
		void Uniform3:: set( Bits & s ) throw() 
		{
			const int32_t _s = s.full<int32_t>();
			const int32_t ms = _s < 0 ? - _s  : _s ;
			int32_t mj = MSEED - ( max_of<int32_t>(1,ms) );
			if(mj<0)
				mj = -mj;
			mj %= MBIG;
			ma_[55]=mj;
			
			int32_t mk=1;
			for(int i=1;i<=54;++i)
			{
				const int ii = (21*i) % 55;
				ma_[ii]=mk;
				mk=mj-mk;
				if (mk < MZ)
					mk += MBIG;
				mj=ma_[ii];
			}
			for(int k=1;k<=4;k++)
				for(int i=1;i<=55;i++)
				{
					ma_[i] -= ma_[1+(i+30) % 55];
					if (ma_[i] < MZ)
						ma_[i] += MBIG;
				}
			
			inext_  = 0;
			inextp_ = 31;
		}
		
		double Uniform3:: operator()( void ) throw() {
			if (++(inext_) == 56)
				(inext_) =1;
			if (++(inextp_) == 56)
				inextp_=1;
			
			int32_t mj = ma_[inext_]- ma_[inextp_];
			if (mj < MZ)
				mj += MBIG;
			ma_[inext_]=mj;
			/* mj is in [0:MBIG-1] */
			return ((double)(mj)+0.5) / ( (double)(MBIG) );
		}
		
	}
	
}
