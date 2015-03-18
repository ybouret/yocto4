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


            const size_t position;  //!< index of position
            const size_t lower;     //!< lower index, included
            const size_t upper;     //!< upper index, included
            const double value;     //!< the value, casted to double

            explicit spike(size_t pos, double val) throw();
            virtual ~spike() throw();
            
            template <
            typename COORD,
            typename ITERATOR,
            typename FUNC>
            static inline void detect(sequence<spike::pointer> &spikes,
                                      ITERATOR                 iter,
                                      const COORD              imin,
                                      const COORD              imax,
                                      FUNC                    &func)
            {
                detector<COORD> dd(iter,imin,imax,func);
                dd.run(spikes);
            }

            template <
            typename SEQ,
            typename FUNC>
            static inline void detect(sequence<spike::pointer> &spikes,
                                      const SEQ                &data,
                                      FUNC                     &func )
            {
                detect<size_t,typename SEQ::const_iterator,FUNC>(spikes,1,data.size(),func);
            }
            
            
            //! compare by increasing position
            static inline
            int compare_by_position( const spike::pointer &lhs, const spike::pointer &rhs) throw()
            {
                return __compare<size_t>(lhs->position,rhs->position);
            }


            //! compare by decreasing amplitude
            static inline
            int compare_by_value( const spike::pointer &lhs, const spike::pointer &rhs) throw()
            {
                return __compare_decreasing<double>(lhs->value,rhs->value);
            }

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(spike);
            template <typename COORD>
            class detector
            {
            public:
                const COORD               imin;
                const COORD               imax;
                const size_t              size;
                
                template <typename ITERATOR,typename FUNC>
                explicit detector(ITERATOR    iter,
                                  const COORD index_min,
                                  const COORD index_max,
                                  FUNC       &func) :
                imin( min_of(index_min,index_max) ),
                imax( max_of(index_min,index_max) ),
                size(imax+1-imin),
                wlen( size * (sizeof(size_t)+sizeof(double) )),
                wksp( NULL ),
                indx( NULL ),
                data( NULL )
                {
                    
                    // memory for transformed data + indx
                    wksp = memory::kind<memory::global>::acquire(wlen);
                    indx = static_cast<size_t *>(wksp);
                    size_t *tmp = indx + size;
                    data = (double *)&tmp[0];
                    --indx;
                    --data;

                    // build internal data
                    for(size_t i=1;i<=size;++i,++iter)
                    {
                        data[i] = func(*iter);
                        indx[i] = i;
                    }

                    // create the index
                    c_array<const double> ra(  data+1, size );
                    c_array<size_t>  idx( indx+1, size );
                    make_index( ra, idx, __compare_decreasing<double>);
                }
                
                virtual ~detector() throw()
                {
                    memory::kind<memory::global>::release(wksp, wlen);
                }
                
                
                size_t   wlen;
                void    *wksp;
                size_t  *indx;
                double  *data;
                
                inline void run(sequence<spike::pointer> &spikes)
                {
                    
                    for(size_t i=1;i<=size;++i)
                    {
                        //______________________________________________________
                        //
                        // next position to process
                        //______________________________________________________

                        const size_t  j=indx[i];
                        const double  v=data[j];

                        //______________________________________________________
                        //
                        // is is the neighbor of a former spike ?
                        //______________________________________________________
                        for(size_t k=spikes.size();k>0;--k)
                        {
                            spike      &spk = *spikes.at(k);

                            //__________________________________________________
                            //
                            // is it before ?
                            //__________________________________________________
                            {
                                const size_t before = spk.lower-1;
                                if(before==j)
                                {
                                    if(v<=data[before])
                                    {
                                        // expand spike @before
                                        --(size_t &)(spk.lower);
                                        goto NEXT_DATA;
                                    }
                                    else
                                    {
                                        goto NEW_SPIKE;
                                    }
                                }
                                
                            }

                            //__________________________________________________
                            //
                            // is it after ?
                            //__________________________________________________
                            {
                                const size_t after =spk.upper+1;
                                if(after==j)
                                {
                                    if(v<=data[after])
                                    {
                                        // expand spike @after
                                        ++(size_t&)(spk.upper);
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
                            spike::pointer pS( new spike(j,v) );
                            spikes.push_back(pS);
                        }

                    NEXT_DATA:;
                    }
                    
                }
                
                
                YOCTO_DISABLE_COPY_AND_ASSIGN(detector);
                
                
            };
            
        };
        
    }
}

#endif
