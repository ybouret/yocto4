#ifndef YOCTO_MATH_KERNEL_LU_INCLUDED
#define YOCTO_MATH_KERNEL_LU_INCLUDED 1

#include "yocto/math/kernel/matrix.hpp"
#include "yocto/sequence/c-array.hpp"
#include "yocto/math/types.hpp"

namespace yocto
{
	namespace math
	{
        
        
		template <typename T>
		class lu
		{
		public:
			typedef typename real_of<T>::type real_type;
			typedef size_t                    indx_type;
            
			bool build(matrix<T> &a) throw();
            
			void solve(const matrix<T>     &a,
                       array<T>            &b) const throw();
            
			void solve(const matrix<T>     &M,
                       matrix<T>           &Q) const throw();
            
			explicit lu() throw();
			explicit lu(size_t);
			virtual ~lu() throw();
            
			void release() throw();
			void ensure( size_t n );
            
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(lu);
			c_array<indx_type> indx;
			c_array<real_type> scal;
			void              *buffer_;
			size_t             buflen_;
			size_t             maxi_;
			void               link(size_t n) throw();
            
		public:
			bool              dneg;
            array<real_type> &arr;   //!< scal can be an extraneous array
		};
        
	}
    
}

#endif

