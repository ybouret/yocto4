#ifndef YOCTO_GFX_RAWPIX_FILTER_INCLUDED
#define YOCTO_GFX_RAWPIX_FILTER_INCLUDED 1

#include "yocto/gfx/rawpix.hpp"
#include <cstdlib>
#include <cstring>

namespace yocto
{

    namespace gfx
    {


        class filter
        {
        public:
            double value[9];

            inline filter() throw() : value() { memset(value,0,sizeof(value)); }
            inline virtual ~filter() throw() {}
            
            inline double & operator()(int x, int y) throw()
            {
                assert( abs(x)<=1 );
                assert( abs(y)<=1 );
                return value[3*(y+1)+(x+1)];
            }

            inline void fill_with( const double a ) throw() { for(size_t i=0;i<9;++i) value[i] = a; }

            YOCTO_DISABLE_COPY_AND_ASSIGN(filter);
        };

        template <typename T,size_t N>
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

            //! N channels for U
            template <typename U>
            inline void compute( U *results, const filter &f ) const throw()
            {

                assert(NULL!=results);

                // accumulator
                double sum[N] = { 0 };
                for(size_t k=0;k<N;++k) sum[k] = 0;

                // apply
                for(size_t i=0;i<9;++i)
                {
                    const T     *p = &value[N*i];
                    const double w =  f.value[i];
                    for(size_t k=0;k<N;++k)
                    {
                        sum[k] += double(p[k]) * w;
                    }
                }

                // store
                for(size_t k=0;k<N;++k)
                {
                    results[k] = U(sum[k]);
                }

            }

            YOCTO_DISABLE_COPY_AND_ASSIGN(NDfilter);

        };

        template <typename T,typename S,size_t N>
        inline
        void apply_filter( bitmap &target, const bitmap &source, const filter &f)
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
                    // collect data
                    source.safe_copy( s(-1,-1), im, jm, N);
                    source.safe_copy( s(-1, 0), im, j , N);
                    source.safe_copy( s(-1, 1), im, jp, N);

                    source.safe_copy( s(0,-1), i, jm, N);
                    source.safe_copy( s(0, 0), i, j , N);
                    source.safe_copy( s(0, 1), i, jp, N);

                    source.safe_copy( s(1,-1), ip, jm,N);
                    source.safe_copy( s(1, 0), ip, j ,N);
                    source.safe_copy( s(1, 1), ip, jp,N);

                    s.template compute<T>( (T*)(target.get(i,j)),f );

                }

            }


            
        }
        
        
    }
    
}

#endif
