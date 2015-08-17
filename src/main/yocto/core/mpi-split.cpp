#include "yocto/core/mpi-split.hpp"
#include "yocto/core/list.hpp"
#include "yocto/object.hpp"

#include <cmath>

namespace yocto
{

    namespace core
    {

        namespace
        {
            class node2D
            {
            public:
                const size_t x,y;
                const double ratio;
                node2D *next,*prev;
                inline node2D(const size_t px,
                              const size_t py,
                              const size_t Nx,
                              const size_t Ny) throw() :
                x(px),
                y(py),
                ratio( fabs( double(Nx)/px - double(Ny)/py) ),
                next(0),
                prev(0)
                {

                }

                inline ~node2D() throw()
                {}


                YOCTO_MAKE_OBJECT

            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(node2D);
            };

            typedef core::list_of_cpp<node2D> list2D;

        }

        static inline void build_list2D(list2D       &l,
                                        const size_t nproc,
                                        const size_t Nx,
                                        const size_t Ny)
        {
            assert(nproc>0);
            for(size_t x=1;x<=nproc;++x)
            {
                for(size_t y=1;y<=nproc;++y)
                {
                    if(x*y==nproc)
                    {
                        l.push_back( new node2D(x,y,Nx,Ny) );
                    }
                }
            }
        }

        void find_mpi_split(size_t      &px,
                            size_t      &py,
                            const size_t nproc,
                            const size_t Nx,
                            const size_t Ny)
        {
            list2D l;
            build_list2D(l, nproc, Nx, Ny);
            assert(l.size>0);
            px    = l.head->x;
            py    = l.head->y;
            double ratio = l.head->ratio;
            delete l.pop_front();
            while(l.size)
            {
                const double tmp = l.head->ratio;
                if(tmp<ratio)
                {
                    px    = l.head->x;
                    py    = l.head->y;
                    ratio = l.head->ratio;
                }
                delete l.pop_front();
            }
        }

    }
    
}