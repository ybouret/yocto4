#ifndef YOCTO_GRAPHICS_REGION_INCLUDED
#define YOCTO_GRAPHICS_REGION_INCLUDED 1

#include "yocto/graphics/types.hpp"
#include "yocto/sequence/list.hpp"

namespace yocto
{
    namespace graphics
    {
        class displacement
        {
        public:
            vertex r;
            char   data[sizeof(vertex)];

            displacement() throw();
            displacement(const vertex &p) throw();
            ~displacement() throw();

            displacement(const displacement &) throw();
            displacement & operator=(const displacement &) throw();

            template <typename T>
            T & get() throw()
            {
                return *(T *)&data[0];
            }

            template <typename T>
            const T & get() const throw()
            {
                return *(T *)&data[0];
            }
        };

        

    }
}

#endif
