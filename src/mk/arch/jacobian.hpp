#ifndef YOCTO_JACOBIAN_INCLUDED
#define YOCTO_JACOBIAN_INCLUDED 1

#include "yocto/sequence/vector.hpp"
#include "yocto/container/matrix.hpp"
#include "yocto/math/types.hpp"

namespace yocto
{
	
	namespace math
	{
		
		
		template <class T>
		class jacobian
		{
		public:
            typedef functor<void,TL2(matrix<T>&,const array<T>&)> type;
            
			explicit jacobian();
			virtual ~jacobian() throw();
			
			void acquire( size_t n );
			void release() throw();
			
			size_t size() const throw();
			
			//! with automatic memory allocation
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
		
#if defined(_MSC_VER)
#pragma warning (push)
#pragma warning (disable:4355)
#endif
        template <typename T>
        class jacobian_of
        {
        public:
            explicit jacobian_of( typename numeric<T>::vector_field &F ) :
            call( this, & jacobian_of<T>::compute ),
            step(1e-4),
            func(F),
            fjac()
            {
            }
            
            virtual ~jacobian_of() throw() {}
            typename jacobian<T>::type call;
            T                          step; //!< default is 1e-4
            
        private:
            typename numeric<T>::vector_field &func;
            jacobian<T>                        fjac;
            inline void compute( matrix<T> &J, const array<T> &X)
            {
                fjac(J,func,X,step);
            }
        };
#if defined(_MSC_VER)
#pragma warning (pop)
#endif
	}
	
}

#endif
