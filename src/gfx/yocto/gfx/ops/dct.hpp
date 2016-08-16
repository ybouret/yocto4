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


            inline void forward(const size_t nch) throw()
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

            inline void reverse(const size_t nch) throw()
            {
                for(size_t x=0;x<N;++x)
                {
                    for(size_t y=0;y<N;++y)
                    {
                        float q[4] = { 0,0,0,0 };
                        for(size_t i=0;i<N;++i)
                        {
                            const float Cxi = COS[x][i];
                            for(size_t j=0;j<N;++j)
                            {
                                const float Cyj    = COS[y][j];
                                const float weight = LAM[i][j] * Cxi * Cyj;
                                for(size_t k=0;k<nch;++k)
                                {
                                    q[k] += dct[k][i][j] * weight;
                                }
                            }
                        }

                        for(size_t k=0;k<nch;++k)
                        {
                            pix[k][x][y] = q[k];
                        }
                    }
                }
            }






        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(DCT);

            inline void setup() throw()
            {
                static const size_t NN     = N<<1;
                for(size_t i=0;i<N;++i)
                {
                    for(size_t j=0;j<N;++j)
                    {
                        const float arg      = (((2*i+1)*j)*3.1415927410125732421875f)/NN;
                        (float &)(COS[i][j]) = cosf(arg);
                    }
                }
                zpix();
                zdct();

                static const float two_over_N = 2.0f / N;
                static const float sq2_over_N = sqrtf(2.0f)/N;
                for(size_t i=1;i<N;++i)
                {
                    (float&)(LAM[0][i]) = sq2_over_N;
                    (float&)(LAM[i][0]) = sq2_over_N;
                    for(size_t j=1;j<N;++j)
                    {
                        (float&)(LAM[i][j]) = two_over_N;
                    }
                }

                (float&)(LAM[0][0]) = 1.0f/N;

                std::cerr << "LAM=" << std::endl;
                for(size_t i=0;i<N;++i)
                {
                    for(size_t j=0;j<N;++j)
                    {
                        std::cerr << (N*LAM[i][j]*0.5f) << " ";
                    }
                    std::cerr << std::endl;
                }

                }

            public:
                inline void show_pix(const size_t nch) const
                {
                    show_local(0,nch);
                }
                inline void show_dct(const size_t nch) const
                {
                    show_local(1,nch);
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


                inline void zpix() throw()
                {
                    memset(pix,0,sizeof(pix));
                }

                inline void zdct() throw()
                {
                    memset(dct,0,sizeof(dct));
                }


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
                
                };
                
                }
                }
                
#endif
