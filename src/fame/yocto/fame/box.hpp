#ifndef YOCTO_FAME_BOX_INCLUDED
#define YOCTO_FAME_BOX_INCLUDED 1

#include "yocto/fame/array3d.hpp"

namespace yocto
{
    namespace fame
    {
        template <size_t DIM,typename T>
        class box
        {
        public:
            static const size_t dimensons = DIM;
            typedef typename vertex_for<DIM,T>::type vtx_type;
            typedef typename vertex_for<DIM,T>::args vtx_args;
            
            const vtx_type vmin;
            const vtx_type vmax;
            const vtx_type length;
            const T        space;
            
            inline explicit box( vtx_args u, vtx_args v ) throw() :
            vmin(u),
            vmax(v),
            length(v-u),
            space(1)
            {
                T *qmin = (T *)&vmin;
                T *qmax = (T *)&vmax;
                T *qlen = (T *)&length;
                for(size_t i=0;i<DIM;++i)
                {
                    if(qmin[i]>qmax[i])
                    {
                        cswap(qmin[i],qmax[i]);
                    }
                    qlen[i] = qmax[i] - qmin[i];
                    (T&)space *= qlen[i];
                }
            }
            
            inline virtual ~box() throw()
            {
            }
            
            inline box(const box &other ) throw() :
            vmin(   other.vmin  ),
            vmax(   other.vmax  ),
            length( other.length)
            {
            }
            
            friend inline std::ostream & operator<<(std::ostream &os, const box &b)
            {
                os << "[[ " << b.vmin << " -> " << b.vmax << " : " << b.length << " / " << b.space << " ]]";
                return os;
            }
            
            //! map each dimension of the array
            template <typename U>
            void map( const typename array_for<DIM,U>::type &arr ) const throw()
            {
                
            }
            
        private:
            YOCTO_DISABLE_ASSIGN(box);
        };
    }
}

#endif

