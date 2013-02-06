#ifndef YOCTO_RANDOM_BITS_INCLUDED
#define YOCTO_RANDOM_BITS_INCLUDED 1

#include "yocto/memory/buffer.hpp"

namespace yocto
{
	
	
	namespace Random
	{
		
		class Bits : public object
		{
		public:
			virtual ~Bits() throw();
			
			//! returns a random bit
			virtual bool operator()(void) throw() = 0;
			
			//! fill partially an integer type
			template <typename T>
			inline T rand( size_t nbits ) throw()
			{
				static const T _t0(0);
				static const T _t1(1);
				T ans( _t0 );
				for( size_t j=nbits; j>0;--j)
				{
					ans <<= 1;
					if( (*this)() )
						ans |= _t1;
				}
				return ans;
			}
			
			//! fill an integer type with random bits
			template <typename T>
			inline T full(void) throw()
			{
				return this->rand<T>( sizeof(T)*8 );
			}
			
			
			//! 0..X-1
			template <typename T>
			inline T lt( T X ) throw()
			{
				static const T _t0(0);
				return X  <= _t0 ? _t0 : ( full<T>() % X );
			}
			
			//! fill partially an integer type
			template <typename T>
			inline T fuzz() throw()
			{
				return this->rand<T>( 1 + this->lt<size_t>( sizeof(T) * 8 ) );
			}
			
			
			//! fill this buffer with random bits
			void fill( memory::rw_buffer & ) throw();
			
		protected:
			explicit Bits() throw();
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(Bits);
		};
		
	
		Bits & SimpleBits() throw();
		Bits & CryptoBits() throw();
		
	}
	
}

#endif
