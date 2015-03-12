#ifndef YOCTO_COMPARATOR_INCLUDED
#define YOCTO_COMPARATOR_INCLUDED 1

#include "yocto/type/args.hpp"

namespace yocto
{
    //! increasing order
    template<typename T>
    inline int __compare( T const &lhs, T const &rhs ) throw()
    {
        return (lhs < rhs) ? -1 : ( rhs < lhs ? 1 : 0 );
    }

    //! decreasing order
    template <typename T>
    inline int __compare_decreasing(  T const &lhs, T const &rhs ) throw()
    {
        return - __compare<T>(lhs,rhs);
    }

    //! increasing order, for libc functions
    template <typename T>
    inline int __compareC( const void *lhs, const void *rhs ) throw()
    {
        assert(lhs); assert(rhs);
        return __compare<T>( *(T*)lhs, *(T*)rhs );
    }

    //! decreasing order, for libc functions
    template <typename T>
    inline int __compareC_decreasing( const void *lhs, const void *rhs ) throw()
    {
        assert(lhs); assert(rhs);
        return __compare_decreasing<T>( *(T*)lhs, *(T*)rhs );
    }


    //! default increasing order comparator class
    template <typename T>
    class comparator
    {
    public:
        YOCTO_ARGUMENTS_DECL_T;

        inline  comparator() throw() {}
        inline ~comparator() throw() {}

        inline int operator()( param_type lhs, param_type rhs ) const throw()
        {
            return __compare<T>(lhs,rhs);
        }

    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(comparator);
    };


}

#endif
