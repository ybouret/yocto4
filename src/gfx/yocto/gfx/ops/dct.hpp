#ifndef YOCTO_GFX_OPS_DCT_INCLUDED
#define YOCTO_GFS_OPS_DCT_INCLUDED 1

#include "yocto/gfx/pixmaps.hpp"
#include "yocto/gfx/pixel.hpp"

namespace yocto
{
    namespace gfx
    {
        template <size_t N>
        class DCT
        {
        public:
            static const size_t BLOCK_SIZE=N;
            inline  DCT() throw() : C(), Lam() { setup(); }
            inline ~DCT() throw() {}

            const float C[N][N];
            const float Lam[N];
            inline friend std::ostream & operator<<(std::ostream &os, const DCT<N> &dct)
            {
                os << '[';
                for(size_t i=0;i<N;++i)
                {
                    for(size_t j=0;j<N;++j)
                    {
                        os << ' ' << dct.C[i][j];
                    }
                    if(i<N-1) os << ';';
                }
                os << ']';
                return os;
            }

            template <typename COLOR,typename T, size_t NCH>
            void forward( pixmaps<T> &target, const pixmap<COLOR> &source, const vertex offset)
            {
                static const float half = float(pixel<T>::opaque)*0.5f;
                for(unit_t j=N-1;j>=0;--j)
                {
                    const unit_t J = offset.y+j;
                    for(unit_t i=N-1;i>=0;--i)
                    {
                        const unit_t I = offset.x+i;
                        const COLOR &C = source[J][I];
                        const T     *p = (const T*)&C;
                        for(size_t k=0;k<NCH;++k)
                        {
                            pix[k][j][i] = float(p[k])-half;
                        }
                    }
                }

                for(unit_t j=N-1;j>=0;--j)
                {
                    const unit_t J = offset.y+j;
                    for(unit_t i=N-1;i>=0;--i)
                    {
                        const unit_t I = offset.x+i;
                        float q[4]= { 0,0,0,0 };

                        for(unit_t y=N-1;y>=0;--y)
                        {
                            for(unit_t x=N-1;x>=0;--x)
                            {
                                const float w = C[i][x] * C[j][y];
                                for(size_t k=0;k<NCH;++k)
                                {
                                    q[k] += pix[k][y][x] * w;
                                }
                            }
                        }


                        const float fac = Lam[i]*Lam[j]/(N/2);
                        for(size_t k=0;k<NCH;++k)
                        {
                            target[k][J][I] = q[k]*fac;
                        }
                    }
                }

            }

        private:
            float pix[4][N][N];
            YOCTO_DISABLE_COPY_AND_ASSIGN(DCT);
            inline void setup() throw()
            {
                static const size_t NN = N<<1;
                for(register size_t i=0;i<N;++i)
                {
                    for(register size_t j=0;j<N;++j)
                    {
                        const float arg = (((2*j+1)*i)*3.1415927410125732421875f)/NN;
                        (float &)(C[i][j]) = cosf(arg);
                    }
                }
                (float&)(Lam[0]) = 1.0f/sqrt(2.0f);
                for(register size_t i=1;i<N;++i) (float&)(Lam[1]) = 1.0f;
            }
        };
    }
}

#endif
