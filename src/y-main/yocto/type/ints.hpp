//! \file type-ints.hpp


#ifndef YOCTO_TYPE_INTS_INCLUDED
#define YOCTO_TYPE_INTS_INCLUDED 1

#include "yocto/os.hpp"

namespace yocto {

	template <const size_t N>
	struct unsigned_int ;

	template <>
	struct unsigned_int<1> {
		typedef uint8_t type;
		static const uint8_t minimun = 0;
		static const uint8_t maximum = 0xff;
	};

	template <>
	struct unsigned_int<2> {
		typedef uint16_t type;
		static const uint16_t minimun = 0;
		static const uint16_t maximum = 0xffff;
	};

	template <>
	struct unsigned_int<4> {
		typedef uint32_t type;
		static const uint32_t minimun = 0;
		static const uint32_t maximum = 0xffffffff;
	};

	template <>
	struct unsigned_int<8> {
		typedef uint64_t type;
		static const uint64_t minimun = 0;
		static const uint64_t maximum = YOCTO_U64(0xffffffffffffffff);
	};

	template <const size_t N>
	struct signed_int;

	template <>
	struct signed_int<1> {
		typedef int8_t type;
		static const int8_t maximum =  127;
		static const int8_t minimun = (-maximum-1);
	};

	template <>
	struct signed_int<2> {
		typedef int16_t type;
		static const int16_t maximum =  32767;
		static const int16_t minimun = (-maximum-1);

	};

	template <>
	struct signed_int<4> {
		typedef int32_t type;
		static const int32_t maximum =  2147483647;
		static const int32_t minimun = (-maximum-1);

	};

	template <>
	struct signed_int<8> {
		typedef int64_t type;
		static const int64_t maximum =  YOCTO_U64(0x7fffffffffffffff);
		static const int64_t minimun =  YOCTO_U64(0xffffffffffffffff);

	};

	template <typename T>
	struct is_signed {
		enum { value = T(-1) < T(0) ? true : false  };
	};

	template <typename T>
	struct limit_of
	{
		enum { size_of = sizeof(T) };
		typedef typename select< is_signed<T>::value,
		signed_int<size_of>,
		unsigned_int<size_of>
		>::result as_integer;

		static const typename as_integer::type minimun = as_integer::minimun;
		static const typename as_integer::type maximum = as_integer::maximum;
	};







}

#endif
