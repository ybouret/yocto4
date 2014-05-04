#ifndef _YOCTO_RANDOM64_HPP_
#define _YOCTO_RANDOM46_HPP_ 1

#include "yocto/random/uniform.hpp"
#include "yocto/hashing/hash64.hpp"

namespace yocto {
	
	namespace Random {
		
		template <hashing::hash64::proc H>
		class hprng : public Uniform {
		public:
			virtual ~hprng() throw() {}
			
			virtual void seed( Bits & s ) throw() {
				seed_ = 1;
				saux_ = s.full<uint32_t>();
			}
			
			virtual double operator()(void) throw() {
				uint32_t lword = saux_;
				uint32_t rword = seed_++;
				H( &lword, &rword );
				return ( double(rword) + 0.5 ) / 4294967296.0;
			}
			
		protected:
			explicit hprng( Bits &s ) throw() :
			seed_( 1 ),
			saux_( s.full<uint32_t>() )
			{
			}
			
		private:
			hprng( const hprng & );
			hprng&operator=( const hprng & );
			uint32_t seed_;
			uint32_t saux_;
		};
		
		
#define YOCTO_RANDOM_HASH64_DECL(CLASS_NAME, HASH_NAME)    \
/* */class CLASS_NAME : public hprng<HASH_NAME> {          \
/* */	public:                                            \
/* */		explicit CLASS_NAME( Bits &s) throw();         \
/* */		virtual ~CLASS_NAME() throw();                 \
/* */	private:                                           \
/* */		CLASS_NAME( const CLASS_NAME & );              \
/* */		CLASS_NAME&operator=( const CLASS_NAME & );    \
}
		
		YOCTO_RANDOM_HASH64_DECL(Uniform64BJ,hashing::hash64::BJ);
		YOCTO_RANDOM_HASH64_DECL(Uniform64NR,hashing::hash64::NR);
		
	}
	
}

#endif
