#include "yocto/random/uniform2.hpp"
#include "yocto/code/utils.hpp"

namespace yocto {
	
	namespace Random {
		
		
		static const int32_t IM1  = 2147483563;
		static const int32_t IM2  = 2147483399;
		static const int32_t IMM1 = (IM1-1);
		static const int32_t IA1 = 40014;
		static const int32_t IA2 = 40692;
		static const int32_t IQ1 = 53668;
		static const int32_t IQ2 = 52774;
		static const int32_t IR1 = 12211;
		static const int32_t IR2 = 3791;
#define              NDIV  (1+IMM1/32)
		
#if defined(_MSC_VER)
#pragma warning ( disable : 4351 )
#endif
		
		Uniform2:: Uniform2( Bits & s ) throw() :
		seed_( 0 ),
		saux_( 0 ),
		iy_(0),
		iv_( )
		{
			this->set( s );
		}
		
		
		Uniform2:: ~Uniform2() throw() {
			
		}
		
		
		void Uniform2:: set( Bits & s ) throw() {
			seed_ = s.full<int32_t>();
			
			if( seed_ <= 0 ) 
			{
				const int32_t ms = -seed_;
				seed_ = max_of<int32_t>( 1, ms );
			}
			
			saux_ = seed_;
			for(int32_t j=32+7;j>=0;j--)
			{
				const int32_t k=seed_/IQ1;
				seed_=IA1*(seed_-k*IQ1)-k*IR1;
				if (seed_ < 0)
					seed_ += IM1;
				if (j < 32)
					iv_[j] = seed_;
			}
			iy_=iv_[0];
			
		}
		
		void Uniform2:: seed( Bits &s ) throw() {
			this->set(s);
		}
		
		double Uniform2:: operator()(void) throw() {
			int32_t k=(seed_)/IQ1;
			
			seed_=IA1*(seed_-k*IQ1)-k*IR1;
			if (seed_ < 0)
				seed_ += IM1;
			
			k       = saux_/IQ2;
			saux_   = IA2*( saux_ - k*IQ2)-k*IR2;
			
			if ( saux_ < 0)
				saux_ += IM2;
			
			const int32_t j= (iy_)/NDIV;
			iy_     = iv_[j] - saux_;
			iv_[j]  = seed_;
			if (iy_ <= 0)
				iy_ += IMM1;
			/*-- each iv is in [0;IM1-1] */
			
			return ((double)(iy_)+0.5)/( (double)(IM1) );
		}
		
		
		
	}
	
}
