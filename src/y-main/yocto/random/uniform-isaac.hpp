#ifndef YOCTO_RANDOM_UNIFORM_ISAAC_INCLUDED
#define YOCTO_RANDOM_UNIFORM_ISAAC_INCLUDED  1

#include "yocto/random/uniform.hpp"
#include "yocto/random/isaac.hpp"

namespace yocto {

	namespace Random {


		//! simulation isaac
		class isaac_fast : public Uniform {
		public:
			virtual ~isaac_fast() throw();
			explicit isaac_fast( Bits & ) throw();

			virtual double operator()(void) throw();
			virtual void   seed( Bits &)    throw();

		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(isaac_fast);
			uint32_t           mseed_;
			ISAAC_FAST         isaac_;

		};

		//! cryptographic isaasc
		class isaac_crypto : public Uniform {
		public:
			virtual ~isaac_crypto() throw();
			explicit isaac_crypto( Bits & ) throw();
			
			virtual double operator()(void)     throw();
			virtual void   seed( Bits & ) throw();
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(isaac_crypto);
			uint32_t           mseed_;
			ISAAC              isaac_;
		};
		

	}

}

#endif
