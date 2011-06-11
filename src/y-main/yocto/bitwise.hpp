//! \file bitwise.hpp
/**
	\brief bitwise indicators
 */
#ifndef YOCTO_BITWISE_INCLUDED
#define YOCTO_BITWISE_INCLUDED 1

#include "yocto/type-traits.hpp"


namespace yocto {

	//! can we use memcpy/memmove...?
	template <typename T>
	struct support_bitwise_ops
	{
		enum { value = type_traits<T>::is_primitive };
	};

	//! do we need to destruct the type ?
	template <typename T>
	struct support_no_destruct
	{
		enum { value = type_traits<T>::is_primitive };
	};


	//! if we can use bitwise ops and no destruct.
	template <typename T>
	struct support_c_style_ops
	{
		enum { value = support_bitwise_ops<T>::value && support_no_destruct<T>::value };
	};


	#define YOCTO_SUPPORT_BITWISE_OPS(CLASS) \
	namespace yocto {                        \
		template <>                          \
		struct support_bitwise_ops< CLASS >  \
		{ enum {value = true}; };            \
	}

	#define YOCTO_SUPPORT_NO_DESTRUCT(CLASS) \
	namespace yocto {                        \
		template <>                          \
		struct support_no_destruct< CLASS >    \
		{ enum {value = true}; };            \
	}

	#define YOCTO_SUPPORT_C_STYLE_OPS(CLASS) \
		YOCTO_SUPPORT_BITWISE_OPS(CLASS) \
		YOCTO_SUPPORT_NO_DESTRUCT(CLASS)

		
}

#endif
