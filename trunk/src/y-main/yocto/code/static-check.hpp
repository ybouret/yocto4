#ifndef YOCTO_STATIC_CHECK_INCLUDED
#define YOCTO_STATIC_CHECK_INCLUDED  1

namespace yocto
{
	template <bool> struct compile_time_check {
		compile_time_check(...);
	};
	
	template <> struct compile_time_check<false> {};
	
#define YOCTO_STATIC_CHECK(expr,msg)         \
/*	*/	{                                    \
/*	*/		class ERROR_##msg {};            \
/*	*/		(void)sizeof( (compile_time_check<(expr)==true>(ERROR_##msg())) );\
/*	*/	}
	
}

#endif
