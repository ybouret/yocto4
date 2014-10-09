#ifndef YOCTO_FAME_BOX_INCLUDED
#define YOCTO_FAME_BOX_INCLUDED 1

#include "yocto/fame/array3d.hpp"
#include "yocto/fame/arrays.hpp"


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
            
            const vtx_type vmin;   //!< lower coordinates
            const vtx_type vmax;   //!< upper coordinates
            const vtx_type length; //!< differences
            const T        space;  //!< product
            
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
            inline void map( array1D<U> &x ) const throw()
            {
                const T *qmin = (const T *)&vmin;
                const T *qmax = (const T *)&vmax;
                const T *qlen = (const T *)&length;
                map1<U>(x.entry,x.items,qmin[0],qmax[0],qlen[0]);
            }
            
            
            //! map each dimension of the arrays
            template <typename U>
            inline void map( array1D<U> &x, array1D<U> &y) const throw()
            {
                const T *qmin = (const T *)&vmin;
                const T *qmax = (const T *)&vmax;
                const T *qlen = (const T *)&length;
                map1<U>(x.entry,x.items,qmin[0],qmax[0],qlen[0]);
                map1<U>(y.entry,y.items,qmin[1],qmax[1],qlen[1]);
            }
            
            //! map each dimension of the array
            template <typename U>
            inline void map( array1D<U> &x, array1D<U> &y, array1D<U> &z) const throw()
            {
                const T *qmin = (const T *)&vmin;
                const T *qmax = (const T *)&vmax;
                const T *qlen = (const T *)&length;
                map1<U>(x.entry,x.items,qmin[0],qmax[0],qlen[0]);
                map1<U>(y.entry,y.items,qmin[1],qmax[1],qlen[1]);
                map1<U>(z.entry,z.items,qmin[2],qmax[2],qlen[2]);
            }
            
            //! map X/Y/Z in the arrays
            template <typename U>
            inline void map( arrays &adb )
            {
                const T *qmin = (const T *)&vmin;
                const T *qmax = (const T *)&vmax;
                const T *qlen = (const T *)&length;
                for(size_t d=0;d<DIM;++d)
                {
                    const char id[4] = { char('X'+d), 0, 0, 0 };
                    array1D<U> &axis = adb[id].as< array1D<U> >();
                    map1<U>(axis.entry,axis.items,qmin[d],qmax[d],qlen[d]);
                }
            }
            
            
        private:
            YOCTO_DISABLE_ASSIGN(box);
            template <typename U>
            static inline void map1( U *u, size_t n, const U umin, const U umax, const U ulen ) throw()
            {
                assert(u);
                assert(n>0);
                *u = umin;
                --n;
                ++u;
                for(size_t i=1;i<n;++i,++u)
                {
                    *u = umin + (i*ulen)/n; // actually n-1
                }
                *u = umax;
            }
        };
    }
}

#endif

