//! \file link1d.hpp
#ifndef YOCTO_SPADE_LINK1D_INCLUDED
#define YOCTO_SPADE_LINK1D_INCLUDED 

#include "yocto/spade/array1d.hpp"
#include "yocto/sequence/vector.hpp"

namespace yocto
{
	
	namespace spade
	{
		typedef vector<uint8_t> vector1d_type;
		
		template <typename T>
		void link1d( vector1d_type &x_data, array1D<T> &X );
		
		template <typename T>
		void link1d( vector1d_type &xy_data, array1D<T> &X, array1D<T> &Y );
		
		template <typename T>
		void link1d( vector1d_type &xyz_data, array1D<T> &X, array1D<T> &Y, array1D<T> &Z );
	}
	
}

#endif
