#ifndef _YOCTO_TRIDIAG_HPP_
#define _YOCTO_TRIDIAG_HPP_ 1

#include "yocto/sequence/vector.hpp"
#include <iosfwd>

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

			T       &a( size_t n ) throw();
			const T &a( size_t n ) const throw();

			T       &b( size_t n ) throw();
			const T &b( size_t n ) const throw();

			T       &c( size_t n ) throw();
			const T &c( size_t n ) const throw();

			void     solve( array<T> &u, const array<T> &r );

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
			T          *g_;
		};
	}

}

#endif
