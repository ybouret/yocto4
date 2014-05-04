#include "yocto/random/uniform0.hpp"

namespace yocto {
	
	namespace Random {
		static const int32_t MASK  = 123459876;
		static const int32_t IA    = 16807;
		static const int32_t IM    = 2147483647; /* 0x7FFFFFFF = 2^31-1 */
		static const int32_t IQ    = 127773;
		static const int32_t IR    = 2836;
		
		Uniform0:: Uniform0( Bits &s ) throw() :
		seed_(0)
		{
			this->set(s);
		}
		
		Uniform0:: ~Uniform0() throw() {
			
		}
		
		void Uniform0:: set( Bits &b ) throw()
		{
			seed_ = b.full<int32_t>();
		}
		
		void Uniform0:: seed( Bits & s ) throw()
		{
			this->set(s);
		}
		
		double Uniform0:: operator()(void) throw() {
			seed_ ^= MASK;
			const int32_t k = seed_ / IQ;
			seed_ = IA * ( seed_ - k * IQ ) - IR * k;
			if( seed_ < 0 )
				seed_ += IM;
			
			/* seed_ is in 0-(IM-1) */
			const double res = ( 0.5 + (double)(seed_) )/( (double)IM );
			
			seed_ ^= MASK;
			
			return res;
		}
		
		
	}
	
}
