#ifndef _YOCTO_CHEBYSHEV_HPP_
#define _YOCTO_CHEBYSHEV_HPP_ 1

#include "yocto/sequence/vector.hpp"
#include "yocto/math/types.hpp"

namespace yocto {

	namespace math {

		template <typename T>
		class chebyshev : public object
		{
		public:
			explicit chebyshev( T a, T b, size_t n, typename numeric<T>::function & );
			virtual ~chebyshev() throw();

			size_t size() const throw();               //!< number of coefficients
			T      get( size_t m, T x ) const throw(); //!< compute value up to m<=n coefficients


		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(chebyshev);
			const T   a_;
			const T   b_;
			const T   bma_; // b-a
			const T   bpa_; // b+a
			vector<T> wksp_;
			T        *coef_;
			const T   hc0_;
		};

		//YOCTO_DMC_MATH(class chebyshev)

	}

}

#endif
