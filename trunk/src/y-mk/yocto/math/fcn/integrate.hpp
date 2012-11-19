/** \file  integrate.hpp
	\brief Integration of Functions.
*/

#ifndef YOCTO_INTEGRATE_INCLUDED
#define YOCTO_INTEGRATE_INCLUDED 1


#include "yocto/math/types.hpp"


namespace yocto {

	namespace math {


		//! recursive Simpson's integration.
        template <typename T>
        T integrate( T a, T b, typename  numeric<T>::function &F, T ftol );
        

		#if 0
		template <class T>
		T integrate( const T xmin,
		             const T xmax,
		             void                         (*proc_y)( T, T &ylo, T &yhi, void *),
		             void                          *data_y,
		             typename numerical<T>::proc2_t func_xy,
		             void                          *args_xy,
		             const T                        ftol);

		template <class T>
		T integrate( const T xmin,
		             const T xmax,
		             void                            (*proc_y)( T, T &ylo, T &yhi, void *),
		             void                             *data_y,
		             typename numerical<T>::CFunction2 func_xy,
		             const T                           ftol);
		#endif

	}

}

#endif
