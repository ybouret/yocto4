#ifndef YOCTO_MATH_KERNEL_TRIDIAG_INCLUDED
#define YOCTO_MATH_KERNEL_TRIDIAG_INCLUDED 1

#include "yocto/sequence/vector.hpp"
#include "yocto/math/kernel/matrix.hpp"

namespace yocto {

	namespace math {

		template <class T>
		class tridiag : public object {
		public:
			explicit tridiag() throw();
			explicit tridiag( size_t n );
			virtual ~tridiag() throw();

			size_t size() const throw();


			void make( size_t n );

            //! lower diagonal
			T       &a( size_t n ) throw();
			const T &a( size_t n ) const throw();

            //! diagonal
			T       &b( size_t n ) throw();
			const T &b( size_t n ) const throw();

            //! upper diagonal
			T       &c( size_t n ) throw();
			const T &c( size_t n ) const throw();

			void     solve( array<T>  &u, const array<T>  &r) const;
            void     solve( matrix<T> &M, const matrix<T> &A) const;
            
            
			T operator()( size_t i, size_t j) const throw();
			void to_scilab( std::ostream &os ) const;

			friend inline
			std::ostream &operator<<( std::ostream &os, const tridiag &t )
			{
				t.to_scilab(os);
				return os;
			}

		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(tridiag);
			size_t      size_;
			vector<T>   wksp_;
			T          *a_;
			T          *b_;
			T          *c_;
			mutable T  *g_;
		};
	}

}

#endif
