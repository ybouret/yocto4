#ifndef YOCTO_MK_DATA_BSPLINES_INCLUDED
#define YOCTO_MK_DATA_BSPLINES_INCLUDED 1

#include "yocto/math/types.hpp"
#include "yocto/sequence/array.hpp"
#include "yocto/code/utils.hpp"

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
                const size_t lmin = 1+n;
                const size_t l    = max_of(jlo,lmin);
                std::cerr << "\tl=" << l << std::endl;
                return 0;
            }
            
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(Bsplines);
        };
        
    }
}

#endif
