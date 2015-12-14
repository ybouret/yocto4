/** \file triplet.hpp
 */

#ifndef _YOCTO_TRIPLET_HPP_
#define _YOCTO_TRIPLET_HPP_

#include "yocto/math/types.hpp"
#include "yocto/type/args.hpp"
#include "yocto/sort/network.hpp"

#include <iostream>

namespace yocto {

    namespace math {

        template <class T>
        struct triplet
        {
            YOCTO_ARGUMENTS_DECL_T;

            T a,b,c;

            inline type & operator[](const size_t index) throw()
            {
                assert( index >= 1 );
                assert( index <= 3 );
                return *(((&a)-1)+index);
            }

            inline const_type & operator[](const size_t index) const throw()
            {
                assert( index >= 1 );
                assert( index <= 3 );
                return *(((&a)-1)+index);
            }

            inline void sort() throw()
            {
                netsort<T>::level3(a,b,c);
            }

            inline void co_sort( triplet<T> &other ) throw()
            {
                netsort<T>::co_level3(a,b,c,other.a,other.b,other.c);
            }


            friend inline std::ostream & operator<<( std::ostream &os, const triplet &triplet_out)
            {
                os << "[ " << triplet_out.a << " " << triplet_out.b << " " << triplet_out.c << " ]'";
                return os;
            }

            inline bool is_ordered() const throw()
            {
                return (a <= b && b <= c) || (a >= b && b >= c);
                
            }
            
        };
        
    }
}
#endif
