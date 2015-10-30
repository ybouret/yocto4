#ifndef YOCTO_GRAPHIX_BLOB_INCLUDED
#define YOCTO_GRAPHIX_BLOB_INCLUDED 1

#include "yocto/graphix/pixmap.hpp"
#include "yocto/core/list.hpp"

namespace yocto
{
    namespace graphix
    {

#if 0
        class vnode
        {
        public:
            vnode *next;
            vnode *prev;
            vertex     r;
            inline  vnode() throw();
            inline ~vnode() throw();
            YOCTO_MAKE_OBJECT
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(vnode);
        };

        typedef core::list_of_cpp<vnode> vlist;
#endif

        class blob : public pixmap<int>
        {
        public:
            explicit blob(size_t W,size_t H);
            virtual ~blob() throw();

            template <typename T>
            inline void detect( const pixmap<T> &p )
            {
                counter = 0;
                ldz();
            }

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(blob);
            type counter;
        };

    }

}

#endif

