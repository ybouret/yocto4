#ifndef YOCTO_GFX_OPS_DCT_INCLUDED
#define YOCTO_GFS_OPS_DCT_INCLUDED 1

#include "yocto/gfx/pixmaps.hpp"
#include "yocto/gfx/pixel.hpp"
#include <cstring>

namespace yocto
{
    namespace gfx
    {
        template <size_t N>
        class DCT
        {
        public:
            static const size_t BLOCK_SIZE=N;
            inline  DCT() throw() : COS(), LAM(), pix() { setup(); }
            inline ~DCT() throw() {}

            const float COS[N][N];
            const float LAM[N][N];
            float       pix[4][N][N];
            float       dct[4][N][N];

            inline friend std::ostream & operator<<(std::ostream &os, const DCT<N> &dct)
            {
                os << '[';
                for(size_t i=0;i<N;++i)
                {
                    for(size_t j=0;j<N;++j)
                    {
                        os << ' ' << dct.COS[i][j];
                    }
                    if(i<N-1) os << ';';
                }
                os << ']';
                return os;
            }

            inline void forward(const size_t nch)
            {
                assert(nch>0);
                assert(nch<=4);
                for(size_t i=0;i<N;++i)
                {
                    for(size_t j=0;j<N;++j)
                    {
                        float      q[4]    = {0,0,0,0};
                        const float LAM_ij = LAM[i][j];
                        for(size_t x=0;x<N;++x)
                        {
                            const float Cxi = COS[x][i];
                            for(size_t y=0;y<N;++y)
                            {
                                const float Cyj    = COS[y][j];
                                const float weight = Cxi*Cyj;
                                for(size_t k=0;k<nch;++k)
                                {
                                    q[k] += pix[k][x][y] * weight;
                                }
                            }
                        }

                        for(size_t k=0;k<nch;++k)
                        {
                            dct[k][i][j] = q[k] * LAM_ij;
                        }
                    }
                }
            }

            inline void reverse(const size_t nch) const
            {

            }


            //! which=0 => pix, which=1 => dct
            inline void show_local(const int which, const size_t nch) const
            {
                assert(nch>0);
                assert(nch<=4);
                assert(which<=1);
                const bool is_pix = (which==0);
                for(size_t i=0;i<N;++i)
                {
                    for(size_t j=0;j<N;++j)
                    {
                        std::cerr << '(';
                        for(size_t k=0;k<nch;++k)
                        {
                            std::cerr << (is_pix?pix[k][i][j]:dct[k][i][j]);
                            if(k<nch-1)
                            {
                                std::cerr << ',';
                            }
                        }
                        std::cerr << ')' << ' ';
                    }
                    std::cerr << std::endl;
                }
            }

            inline void show_pix(const size_t nch) const
            {
                show_local(0,nch);
            }
            inline void show_dct(const size_t nch) const
            {
                show_local(1,nch);
            }


        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(DCT);
            inline void setup() throw()
            {
                static const size_t NN = N<<1;
                static const float  isqrt2 = 1.0f/sqrtf(2.0f);
                for(register size_t i=0;i<N;++i)
                {
                    const float lam_i = (i<=0) ? isqrt2 : 1.0f;
                    for(register size_t j=0;j<N;++j)
                    {
                        const float lam_j = (j<=0) ? isqrt2 : 1.0f;
                        const float arg   = (((2*j+1)*i)*3.1415927410125732421875f)/NN;
                        (float &)(COS[i][j]) = cosf(arg);
                        const float lam_ij   = lam_i * lam_j;
                        (float&)(LAM[i][j])  = (lam_ij+lam_ij)/N;
                    }
                }
                memset(pix,0,sizeof(pix));
                memset(dct,0,sizeof(dct));
            }
        };
    }
}

#endif
