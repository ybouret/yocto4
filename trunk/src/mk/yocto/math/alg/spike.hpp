#ifndef YOCTO_MATH_ALG_SPIKE_INCLUDED
#define YOCTO_MATH_ALG_SPIKE_INCLUDED 1

#include "yocto/sort/index.hpp"
#include "yocto/core/list.hpp"
#include "yocto/core/pool.hpp"
#include "yocto/ptr/arc.hpp"
#include "yocto/counted-object.hpp"
#include "yocto/code/utils.hpp"

namespace yocto
{
    namespace math
    {

        class spike : public counted_object
        {
        public:
            typedef arc_ptr<spike> pointer;

            class index_node
            {
            public:
                const size_t value;
                index_node  *next;
                index_node  *prev;

                YOCTO_MAKE_OBJECT
                ~index_node() throw();
                index_node(size_t) throw();

            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(index_node);
            };

            typedef core::list_of_cpp<index_node> index_list;

            const size_t position;  //!< index of position
            index_list   indices;   //!< list of indices

            explicit spike(size_t pos) throw();
            virtual ~spike() throw();

            template <
            typename T,
            typename COORD,
            typename ITERATOR,
            typename FUNC>
            static inline void detect( ITERATOR iter, const COORD imin, const COORD imax, FUNC &func)
            {
                detector<T,COORD> dd(iter,imin,imax,func);
            }

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(spike);

            template <typename T, typename COORD>
            class detector
            {
            public:
                const COORD               imin;
                const COORD               imax;
                const size_t              size;
                core::pool_of<index_node> pool;

                template <typename ITERATOR,typename FUNC>
                explicit detector(ITERATOR    iter,
                                  const COORD index_min,
                                  const COORD index_max,
                                  FUNC       &func) :
                imin( min_of(index_min,index_max) ),
                imax( max_of(index_min,index_max) ),
                size(imax+1-imin),
                pool(),
                wlen( size * (2*sizeof(size_t)+sizeof(T) )),
                wksp( NULL ),
                indx( NULL ),
                flag( NULL ),
                data( NULL )
                {
                    try
                    {
                        for(size_t i=size;i>0;--i)
                        {
                            pool.store( object::acquire1<index_node>() );
                        }

                        wksp = memory::kind<memory::global>::acquire(wlen);
                        indx = static_cast<size_t *>(wksp);
                        flag = indx + size;
                        size_t *tmp = flag + size;
                        data = (T *)&tmp[0];
                        --indx;
                        --flag;
                        --data;

                        for(size_t i=1;i<=size;++i,++iter)
                        {
                            data[i] = func(*iter);
                            flag[i] = 0;
                            indx[i] = i;
                        }
                        c_array<const T> ra(  data+1, size );
                        c_array<size_t>  idx( indx+1, size );
                        make_index( ra, idx, __compare<T>);
                    }
                    catch(...)
                    {
                        while(pool.size)
                        {
                            object::release1(pool.query());
                        }
                        throw;
                    }
                }

                virtual ~detector() throw()
                {
                    memory::kind<memory::global>::release(wksp, wlen);
                    while( pool.size )
                    {
                        object::release1<index_node>( pool.query() );
                    }
                }

                size_t  wlen;
                void   *wksp;
                size_t *indx;
                size_t *flag;
                T      *data;
                
                YOCTO_DISABLE_COPY_AND_ASSIGN(detector);
            };
            
        };
        
    }
}

#endif
