#ifndef YOCTO_FOVEA_BOX_INCLUDED
#define YOCTO_FOVEA_BOX_INCLUDED 1

#include "yocto/fovea/types.hpp"

namespace yocto
{
    namespace fovea
    {
        template <size_t DIM,typename T>
        class Box
        {
        public:
            typedef typename vertex_for<DIM,T>::type vtx;
            
            const vtx vmin;
            const vtx vmax;
            const vtx length;
            const T   volume;
            
            explicit Box( vtx a, vtx b ) throw() :
            vmin(a),
            vmax(b),
            length(),
            volume(1)
            {
                T &vol  = (T&)volume;
                T *pmin = (T*)&vmin;
                T *pmax = (T*)&vmax;
                T *plen = (T*)&length;
                for(size_t i=0;i<DIM;++i)
                {
                    T &qmin = pmin[i];
                    T &qmax = pmax[i];
                    T &qlen = plen[i];
                    if(qmin>qmax)
                    {
                        cswap(qmin,qmax);
                    }
                    qlen = qmax-qmin; assert(qlen>=0);
                    vol *= qlen;
                }
            }
            
            
            inline Box( const Box &b) throw() :
            vmin(b.vmin),
            vmax(b.vmax),
            length(b.length),
            volume(b.volume)
            {
                
            }
            
            inline ~Box() throw()
            {
            }
            
        private:
            YOCTO_DISABLE_ASSIGN(Box);
        };
    }
}


#endif

