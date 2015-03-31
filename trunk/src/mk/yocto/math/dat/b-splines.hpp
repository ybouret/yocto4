#ifndef YOCTO_MK_DATA_BSPLINES_INCLUDED
#define YOCTO_MK_DATA_BSPLINES_INCLUDED 1

#include "yocto/math/types.hpp"
#include "yocto/sequence/vector.hpp"
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
             \param x the value where to be computed
             \param t assume increasingly ordered knots
             \param P the control points
             \param degree the degree of the B-splines
             */
            static U compute( const T x, const array<T> &t, const array<U> &P, const size_t n )
            {
                assert(t.size()>=2);
                assert(t.size()==P.size());
                assert(n<t.size());

                const size_t m = t.size();

                if(x<=t[1])
                {
                    //__________________________________________________________
                    //
                    // first point
                    //__________________________________________________________
                    return P[1];
                }
                else
                {
                    if(x>=t[m])
                    {
                        //______________________________________________________
                        //
                        // last point
                        //______________________________________________________
                        return P[m];
                    }
                    else
                    {
                        const size_t ntot = 2*(n)+m;
                        vector<T> u(ntot,as_capacity);
                        vector<U> p(ntot,as_capacity);

                        for(size_t i=1;i<=n;++i)
                        {
                            u.push_back( t[1] );
                            p.push_back( P[1] );
                        }

                        for(size_t i=1;i<=m;++i)
                        {
                            u.push_back(t[i]);
                            p.push_back(P[i]);
                        }

                        for(size_t i=1;i<=n;++i)
                        {
                            u.push_back( t[m] );
                            p.push_back( P[m] );
                        }

                        //assert(u.size()==ntot);
                        //assert(p.size()==ntot);

                        //______________________________________________________
                        //
                        // bracket it
                        //______________________________________________________
                        size_t jlo = 1+n; // =n
                        size_t jhi = m+n;

                        while(jhi-jlo>1)
                        {
                            const size_t jmid = (jlo+jhi)>>1;
                            const T      tmid = u[jmid];
                            if(x>tmid)
                            {
                                jlo = jmid;
                            }
                            else
                            {
                                jhi = jmid;
                            }
                        }

                        const size_t l = jlo;
                        assert(l>=n);
                        const U tmp;
                        Tableau2D<U> d(0,n,l-n,l,tmp);
                        for(size_t i=l-n;i<=l;++i)
                        {
                            d[0][i] = p[i];
                        }

                        for(size_t k=1;k<=n;++k)
                        {
                            for(size_t i=l-n+k;i<=l;++i)
                            {
                                const T u_i   = u[i];
                                //const T alpha = (x-u_i)/(u[(i+n+k)-1]-u_i);
                                const size_t j= (i+n+1) - k;
                                //std::cerr << "j=" << j << " / " << ntot << std::endl;
                                const T alpha = (x-u_i)/(u[j]-u_i);
                                d[k][i] = (T(1)-alpha)* d[k-1][i-1] + alpha * d[k-1][i];
                            }

                        }

                        return d[n][l];
                    }
                    
                    
                }
                
            }
            
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(Bsplines);
        };
        
    }
}

#endif
