#ifndef YOCTO_CODE_ILOG2_HPP_INCLUDED
#define YOCTO_CODE_ILOG2_HPP_INCLUDED 1

#include "yocto/os.hpp"
namespace
{

	template <size_t> struct ilog2;
	template <> struct ilog2<1>   { enum { value = 0  }; };
	template <> struct ilog2<2>   { enum { value = 1  }; };
	template <> struct ilog2<4>   { enum { value = 2  }; };
	template <> struct ilog2<8>   { enum { value = 3  }; };
	template <> struct ilog2<16>  { enum { value = 4  }; };
	template <> struct ilog2<32>  { enum { value = 5  }; };
	template <> struct ilog2<64>  { enum { value = 6 }; };
	template <> struct ilog2<128> { enum { value = 7 }; };
    
    template <typename T>
    struct ilog2_of { enum { value = ilog2<sizeof(T)>::value }; };
    

}
#endif
