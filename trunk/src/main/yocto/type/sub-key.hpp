#ifndef YOCTO_TYPE_SUB_KEY_INCLUDED
#define YOCTO_TYPE_SUB_KEY_INCLUDED 1

#include "yocto/type/key.hpp"




#define	YOCTO_ARGUMENTS_DECL_SUBKEY                                              \
typedef SUBKEY                                      subkey_type;                 \
typedef typename type_traits<SUBKEY>::mutable_type  mutable_subkey;              \
typedef const mutable_subkey                        const_subkey;                \
typedef typename                                                                 \
yocto::select<                                                                   \
type_traits<mutable_subkey>::is_primitive, /* no references in containers */     \
const_subkey  , /* pass primitive mutable types by const value            */     \
const_subkey &  /* pass non primitive mutable types by const reference    */     \
>::result param_subkey



#endif

