//! \file bitwise.hpp
/**
	\brief bitwise indicators
 */
#ifndef YOCTO_BITWISE_INCLUDED
#define YOCTO_BITWISE_INCLUDED 1

#include "yocto/type/args.hpp"


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


    //! wrapper to setup/clear chunks with default constructor/destructor
    template <typename T>
    struct chunk_ops
    {
    public:
        YOCTO_ARGUMENTS_DECL_T;

        static inline void setup(T *base,const size_t size)
        {
            __setup(base,size,int2type< type_traits<T>::is_primitive >() );
        }

        static inline void clear(T *base,const size_t size) throw()
        {
            __clear(base,size,int2type< support_no_destruct<T>::value >() );
        }

        static inline void setup(T *base,const size_t size, param_type arg)
        {
            size_t i=0;
            try
            {
                while(i<size)
                {
                    new ( &base[i] ) T(arg);
                    ++i;
                }
            }
            catch(...)
            {
                while(i>0)
                {
                    destruct<T>(&base[--i]);
                }
                throw;
            }
        }

        static inline void __setup(T *,const size_t, int2type<true> )
        {
        }

        static inline void __setup(T *base,const size_t size, int2type<false> )
        {
            size_t i=0;
            try
            {
                while(i<size)
                {
                    new ( &base[i] ) T();
                    ++i;
                }
            }
            catch(...)
            {
                while(i>0)
                {
                    destruct<T>(&base[--i]);
                }
                throw;
            }
        }

        static inline void __clear(T*,size_t,int2type<true>) throw() {}

        static inline void __clear(T *base,size_t size,int2type<false>) throw()
        {
            while(size>0)
            {
                destruct<T>(&base[--size]);
            }
        }
    };
    
}

#endif
