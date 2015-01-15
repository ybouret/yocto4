#ifndef YOCTO_GFX_RAWPIX_FILTER_INCLUDED
#define YOCTO_GFX_RAWPIX_FILTER_INCLUDED 1

#include "yocto/gfx/rawpix.hpp"
#include <cstdlib>
#include <cstring>

namespace yocto
{

    namespace gfx
    {

        template <typename T>
        class filter
        {
        public:
            T value[9];

            inline filter() throw() : value() { memset(value,0,sizeof(value)); }
            inline virtual ~filter() throw() {}
            
            inline T & operator()(int x, int y) throw()
            {
                assert( abs(x)<=1 );
                assert( abs(y)<=1 );
                return value[3*(y+1)+(x+1)];
            }

            inline void fill_with( const T a ) throw() { for(size_t i=0;i<9;++i) value[i] = a; }

            YOCTO_DISABLE_COPY_AND_ASSIGN(filter);
        };

        template <typename T, size_t N>
        class NDfilter
        {
        public:
            T value[9*N];
            inline  NDfilter() throw() : value() { memset(value,0,sizeof(value)); }
            inline ~NDfilter() throw() {}

            inline T * operator()(int x, int y) throw()
            {
                assert( abs(x)<=1 );
                assert( abs(y)<=1 );
                const int offset = N*(3*(y+1)+(x+1));
                return &value[offset];
            }

            YOCTO_DISABLE_COPY_AND_ASSIGN(NDfilter);

        };

        template <typename T,typename S,typename V,size_t N>
        inline
        void apply_filter( bitmap &target, const bitmap &source, const filter<V> &f)
        {
            assert(source.w==target.w);
            assert(source.h==target.h);
            assert(source.d>=unit_t(sizeof(S)*N));
            assert(target.d>=unit_t(sizeof(T)*N));

            const unit_t w = source.w;
            const unit_t h = source.h;

            NDfilter<S,N> s;

            std::cerr << "Using NDfilter" << N << ", size=" << sizeof(s) << std::endl;
            for(unit_t jm=-1,j=0,jp=1;j<h;++jm,++j,++jp)
            {
                for(unit_t im=-1,i=0,ip=1;i<w;++im,++i,++ip)
                {
                    source.safe_copy( s(-1,-1), im, jm );
                    source.safe_copy( s(-1, 0), im, j  );
                    source.safe_copy( s(-1, 1), im, jp );

                    source.safe_copy( s(0,-1), i, jm );
                    source.safe_copy( s(0, 0), i, j  );
                    source.safe_copy( s(0, 1), i, jp );

                    source.safe_copy( s(1,-1), ip, jm );
                    source.safe_copy( s(1, 0), ip, j  );
                    source.safe_copy( s(1, 1), ip, jp );
                }
                //break;
            }

#if 0
            for(int j=1;j>=-1;--j)
            {
                for(int i=-1;i<=1;++i)
                {
                    const T *p = s(i,j);
                    std::cerr << " [";
                    for(size_t k=0;k<N;++k)
                    {
                        std::cerr << ' ' << double(p[k]);
                    }
                    std::cerr << "]";
                }
                std::cerr << std::endl;
            }
#endif
            
        }
        
        
    }
    
}

#endif
