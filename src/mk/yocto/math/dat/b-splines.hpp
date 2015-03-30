#ifndef YOCTO_MK_DATA_BSPLINES_INCLUDED
#define YOCTO_MK_DATA_BSPLINES_INCLUDED 1

#include "yocto/math/types.hpp"
#include "yocto/sequence/array.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/container/tab2d.hpp"

namespace yocto {

    namespace math
    {

        template <typename T, typename U>
        class Bsplines
        {
        public:

            //!
            /**
             \param t assume increasingly ordered
             */
            static T compute( const T x, const array<T> &t, const array<U> &P, const size_t n )
            {
                assert(t.size()>=2);
                assert(t.size()==P.size());
                assert(n<=t.size());

                const size_t m = t.size();

                //______________________________________________________________
                //
                //-- find the localisation
                //______________________________________________________________
                size_t jlo = 1;
                {
                    size_t jhi = m;
                    if(x<=t[1])
                    {
                        jhi = 2;
                    }
                    else
                    {
                        if(x>=t[n])
                        {
                            jlo = m-1;
                            jhi = m;
                        }
                        else
                        {
                            while(jhi-jlo>1)
                            {
                                const size_t jmid = (jhi+jlo)>>1;
                                const T      tmid = t[jmid];
                                if(x>tmid)
                                {
                                    jlo = jmid;
                                }
                                else
                                {
                                    jhi = jmid;
                                }
                            }

                        }
                    }

                    std::cerr << t[jlo] << " <= " << x << " <= " << t[jhi] << std::endl;
                }

                //______________________________________________________________
                //
                //-- algorithm
                //______________________________________________________________
                const size_t np1 = 1+n;
                const size_t l    = max_of(jlo,np1);
                const size_t lmn  = l-n;
                assert(l<m);
                assert(lmn>0);
                std::cerr << "\tl=" << l << " / m=" << m << ", n=" << n << std::endl;

                const U tmp;
                Tableau2D<U> d(0,n,lmn,n,tmp);
                for(size_t i=lmn;i<=n;++i)
                {
                    d[0][i] = P[i];
                }


                for(size_t k=1;k<=n;++k)
                {
                    std::cerr << "\tk=" << k << std::endl;
                    for(size_t i=lmn+k;i<=l;++i)
                    {
                        std::cerr << "\t\ti=" << i << std::endl;
                        typename Tableau2D<U>::Row &d_k = d[k];
                        const typename Tableau2D<U>::Row &d_km = d[k-1];
                        std::cerr << "\t\ti+" << np1 << "-k=" << i+np1-k << std::endl;
                        const T t_i   = t[i];
                        //const T t_k   = t[(i+np1)-k];
                        //const T alpha = (x-t_i)/( t[(i+np1)-k]-t_i );
                        //const T alpha=1;
                        //d_k[i] = (T(1)-alpha) * d[k-1][i-1];
                    }
                }

                return 0;
            }
            
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(Bsplines);
        };
        
    }
}

#endif
