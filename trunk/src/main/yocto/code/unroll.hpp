/** \file yce-unroll.h
 \brief generic unrolling
 */

#ifndef YCE_UNROLL_INCLUDED
#define YCE_UNROLL_INCLUDED 1

#include "yocto/os.hpp"

#if defined(__ICC) && defined(NDEBUG)
// selector expression is constant for optimized version
#pragma warning(disable:280)
#endif
/** N > 0 */
#define YOCTO_LOOP_(N,CODE)   do                       \
/*	*/	{                                              \
/*	*/		const register size_t iter_ = (N);         \
/*	*/		assert(iter_>0);                           \
/*	*/		{                                          \
/*	*/			register size_t loop_ = (iter_+7)>>3;  \
/*	*/			switch (iter_&7) {                     \
/*	*/			case 0: do { { CODE; }                 \
/*	*/			case 7:      { CODE; }			       \
/*	*/			case 6:      { CODE; }                 \
/*	*/			case 5:      { CODE; }                 \
/*	*/			case 4:      { CODE; }                 \
/*	*/			case 3:      { CODE; }                 \
/*	*/			case 2:      { CODE; }                 \
/*	*/			case 1:      { CODE; }                 \
/*	*/						} while (--loop_ > 0);    \
/*	*/			}                                     \
/*	*/		}										  \
/*	*/	} while(0)


/**  N >= 0 */
#define YOCTO_LOOP(N,CODE)   do                       \
/*	*/	{                                             \
/*	*/		const register size_t iter_ = (N);        \
/*	*/		if(iter_>0)                               \
/*	*/		{                                         \
/*	*/			register size_t loop_ = (iter_+7)>>3; \
/*	*/			switch (iter_&7) {                    \
/*	*/			case 0: do { { CODE; }                \
/*	*/			case 7:      { CODE; }                \
/*	*/			case 6:      { CODE; }                \
/*	*/			case 5:      { CODE; }                \
/*	*/			case 4:      { CODE; }                \
/*	*/			case 3:      { CODE; }                \
/*	*/			case 2:      { CODE; }                \
/*	*/			case 1:      { CODE; }                \
/*	*/						} while (--loop_ > 0);    \
/*	*/			}                                     \
/*	*/		}                                         \
/*	*/	} while(0)


/** loop from SHIFT to (N-1+SHIFT), N>=0 */
#define YOCTO_LOOP_FUNC(N,FUNC,SHIFT)     do                      \
{                                                                 \
const register size_t iter_ = (N);                                \
if(iter_>0)                                                       \
{                                                                 \
register size_t       loop_ = (iter_+0x7)>>0x3;                   \
const register size_t jump_ = iter_&0x7;                          \
register size_t       indx_ = jump_-(0x8-SHIFT);                  \
switch (jump_) {                                                  \
case 0: do { { const size_t i0_ = (indx_ +=0x8); { FUNC(i0_); } } \
case 7:      { const size_t i1_ = indx_+0x1;     { FUNC(i1_); } } \
case 6:      { const size_t i2_ = indx_+0x2;     { FUNC(i2_); } } \
case 5:      { const size_t i3_ = indx_+0x3;     { FUNC(i3_); } } \
case 4:      { const size_t i4_ = indx_+0x4;     { FUNC(i4_); } } \
case 3:      { const size_t i5_ = indx_+0x5;     { FUNC(i5_); } } \
case 2:      { const size_t i6_ = indx_+0x6;     { FUNC(i6_); } } \
case 1:      { const size_t i7_ = indx_+0x7;     { FUNC(i7_); } } \
} while (--loop_ > 0);                                            \
}                                                                 \
}                                                                 \
} while(0)

/** loop from SHIFT to (N-1+SHIFT), N>0 */
#define YOCTO_LOOP_FUNC_(N,FUNC,SHIFT) do                         \
{                                                                 \
const register size_t iter_ = (N);                                \
assert(iter_>0);                                                  \
{                                                                 \
register size_t       loop_ = (iter_+0x7)>>0x3;                   \
const register size_t jump_ = iter_&0x7;                          \
register size_t       indx_ = jump_-(0x8-SHIFT);                  \
switch (jump_) {                                                  \
case 0: do { { const size_t i0_ = (indx_ +=0x8); { FUNC(i0_); } } \
case 7:      { const size_t i1_ = indx_+0x1;     { FUNC(i1_); } } \
case 6:      { const size_t i2_ = indx_+0x2;     { FUNC(i2_); } } \
case 5:      { const size_t i3_ = indx_+0x3;     { FUNC(i3_); } } \
case 4:      { const size_t i4_ = indx_+0x4;     { FUNC(i4_); } } \
case 3:      { const size_t i5_ = indx_+0x5;     { FUNC(i5_); } } \
case 2:      { const size_t i6_ = indx_+0x6;     { FUNC(i6_); } } \
case 1:      { const size_t i7_ = indx_+0x7;     { FUNC(i7_); } } \
} while (--loop_ > 0);                                            \
}                                                                 \
}                                                                 \
} while(0)


#endif
