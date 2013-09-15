#ifndef YOCTO_TYPE_KEY_INCLUDED
#define YOCTO_TYPE_KEY_INCLUDED 1

#include "yocto/type/args.hpp"

#define	YOCTO_ARGUMENTS_DECL_KEY                                           \
typedef KEY                                      key_type;                 \
typedef typename type_traits<KEY>::mutable_type  mutable_key;              \
typedef const mutable_key                        const_key;                \
typedef typename                                                           \
yocto::select<                                                             \
type_traits<mutable_key>::is_primitive, /* no references in containers */  \
const_key  , /* pass primitive mutable types by const value            */  \
const_key &  /* pass non primitive mutable types by const reference    */  \
>::result param_key

#define YOCTO_ASSOCIATIVE_KEY_T YOCTO_ARGUMENTS_DECL_T; YOCTO_ARGUMENTS_DECL_KEY

#endif

