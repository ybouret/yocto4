#ifndef YOCTO_TYPE_ARGS_INCLUDED
#define YOCTO_TYPE_ARGS_INCLUDED 1

#include "yocto/type/traits.hpp"

/** \brief container like arguments
 * streams, containers,..., can't contain references,
 * so these are the aliases used for in/out parameters passing
 */
#define	YOCTO_ARGUMENTS_DECL_T                                              \
typedef T                                        type;                      \
typedef typename type_traits<T>::mutable_type    mutable_type;              \
typedef const mutable_type                       const_type;                \
typedef typename                                                            \
yocto::select<                                                              \
type_traits<mutable_type>::is_primitive, /* no references in containers */  \
const_type  , /* pass primitive mutable types by const value            */  \
const_type &  /* pass non primitive mutable types by const reference    */  \
>::result param_type


#endif
