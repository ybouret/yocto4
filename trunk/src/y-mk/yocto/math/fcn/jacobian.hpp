#ifndef YOCTO_JACOBIAN_INCLUDED
#define YOCTO_JACOBIAN_INCLUDED 1

#include "yocto/sequence/vector.hpp"
#include "yocto/math/kernel/matrix.hpp"
#include "yocto/math/types.hpp"

namespace yocto
{
	
	namespace math
	{
		
		
		template <class T>
		class jacobian
		{
		public:
			explicit jacobian();
			virtual ~jacobian() throw();
			
			void acquire( size_t n );
			void release() throw();
			
			size_t size() const throw();
			
			
			void operator()( matrix<T> &jac, typename numeric<T>::vector_field &F, const array<T> &X, T h );
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(jacobian);
			vector<T> xa_;
			vector<T> ylo_;
			vector<T> yhi_;
			vector<T> dy_;
			vector<T> yxtr_;
			vector<T> yerr_;
			matrix<T> ya_;
			matrix<T> c_;
			matrix<T> d_;
		};
		
	}
	
}

#endif
