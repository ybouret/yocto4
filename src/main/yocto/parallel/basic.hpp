#ifndef YOCTO_PARALLEL_BASIC_INCLUDED
#define YOCTO_PARALLEL_BASIC_INCLUDED 1

#include "yocto/os.hpp"

namespace yocto
{
    namespace parallel
    {

        template <typename T> inline
        void basic_split(const size_t rank,
                         const size_t size,
                         T           &offset,
                         T           &length) throw()
        {
            assert(size>0);
            assert(rank<size);
            T todo(length/size);
            for(size_t r=1;r<=rank;++r)
            {
                length -= todo;
                offset += todo;
                todo    = length/(size-r);
            }
            length = todo;
        }


        template <typename T,typename U> inline
        void basic_update( T offset, T todo, U &box ) throw()
        {
            box.start = offset;
            box.count = todo;
            box.final = offset+todo-1;
        }

        //! compute in one pass
        /**
         arr : 0..size-1, with start/final/count memberss
         */
        template <
        typename T,
        typename C_ARRAY
        > inline
        void basic_splitV(const size_t size,
                          T            offset,
                          T            length,
                          C_ARRAY     &arr) throw()
        {
            assert(size>0);
            T todo(length/size);
            basic_update(offset,todo,arr[0]);
            for(size_t r=1;r<size;++r)
            {
                length -= todo;
                offset += todo;
                todo    = length/(size-r);
                basic_update(offset,todo,arr[r]);
            }
        }
    }
}

#endif
