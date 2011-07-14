#ifndef _YOCTO_SPLINE_HPP_
#define _YOCTO_SPLINE_HPP_ 1

#include "yocto/sequence/vector.hpp"
#include "yocto/math/types.hpp"

namespace yocto {

	namespace math {


		template <class T>
		class spline : public object
		{
			public:
				class boundary {
					public:
						explicit boundary( bool natural, const T slope = 0) throw();
						virtual ~boundary() throw();
						boundary( const boundary & ) throw();

						const bool is_natural;
						const T    derivative;

					private:
						boundary&operator=( boundary & );
				};

				spline( const array<T> &x, const array<T> &y, const boundary &lo, const boundary &hi );
				virtual ~spline() throw();

				T operator()( T x ) const throw();

			private:
				YOCTO_DISABLE_COPY_AND_ASSIGN(spline);
				T           *x_;
				T           *y_;
				T           *y2_;
				const size_t n_;
				const T      xlo_;
				const T      xhi_;
				const T      ylo_;
				const T      yhi_;
				vector<T>    w_;
		};

		//YOCTO_DMC_MATH(class spline)

	}

}


#endif
