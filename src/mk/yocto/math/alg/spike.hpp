#ifndef YOCTO_MATH_ALG_SPIKE_INCLUDED
#define YOCTO_MATH_ALG_SPIKE_INCLUDED 1

#include "yocto/sort/index.hpp"
#include "yocto/core/list.hpp"
#include "yocto/core/pool.hpp"
#include "yocto/ptr/arc.hpp"
#include "yocto/counted-object.hpp"
#include "yocto/code/utils.hpp"

#include "yocto/sequence/vector.hpp"
#include <iostream>

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
                static int compare( const index_node *lhs, const index_node *rhs) throw();

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
                dd.run();
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

                inline index_node *new_index_node(size_t j) throw()
                {
                    assert(pool.size>0);
                    return ::new ( pool.query() ) index_node(j);
                }

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
                        // prepare indices nodes
                        for(size_t i=size;i>0;--i)
                        {
                            pool.store( object::acquire1<index_node>() );
                        }

                        // memory for transformed data + flag + indx
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

                void run()
                {
                    vector<spike::pointer> spikes;

                    for(size_t i=1;i<=size;++i)
                    {
                        // next position to process
                        const size_t j=indx[i];

                        // is is the neighbor of a former spike ?
                        for(size_t k=spikes.size();k>0;--k)
                        {
                            spike      &spk = *spikes[k];
                            index_list &idx = spk.indices;
                            assert(idx.size>0);

                            // is it before ?
                            {
                                const size_t before = idx.head->value-1;
                                if(before==j)
                                {
                                    if(data[j]<=data[before])
                                    {
                                        // expand spike @before
                                        assert(pool.size>0);
                                        idx.push_front( new_index_node(j) );
                                        goto NEXT_DATA;
                                    }
                                    else
                                    {
                                        goto NEW_SPIKE;
                                    }
                                }

                            }

                            // is it after ?
                            {
                                const size_t after = idx.tail->value+1;
                                if(after==j)
                                {
                                    if(data[j]<=data[after])
                                    {
                                        // expand spike @after
                                        assert(pool.size>0);
                                        idx.push_back( new_index_node(j) );
                                        goto NEXT_DATA;
                                    }
                                    else
                                    {
                                        goto NEW_SPIKE;
                                    }
                                }
                            }
                        }

                    NEW_SPIKE:
                        {
                            spike::pointer pS( new spike(j) );
                            pS->indices.push_back( new_index_node(j) );
                            spikes.push_back(pS);
                        }
                        //continue;
                        
                    NEXT_DATA:;
                    }

                    std::cerr << "#spikes=" << spikes.size() << std::endl;
                }
                
                YOCTO_DISABLE_COPY_AND_ASSIGN(detector);
                
                
            };
            
        };
        
    }
}

#endif
