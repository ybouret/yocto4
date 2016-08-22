#ifndef YOCTO_GFX_OPS_DCT_INCLUDED
#define YOCTO_GFS_OPS_DCT_INCLUDED 1

#include "yocto/gfx/pixmaps.hpp"
#include "yocto/gfx/pixel.hpp"
#include "yocto/parallel/field.hpp"
#include <cstring>

namespace yocto
{
    namespace gfx
    {

        //! Discrete Cosine Transform Interface
        class DCT : public pixmap<double>
        {
        public:
            typedef parallel::field2D<double> Table;

            virtual ~DCT() throw();
            pixmap<double> pix; //!< where to store reverse double data

        protected:
            explicit DCT(const unit_t W, const unit_t H);
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(DCT);
        };


        //! Common (a.k.a) rectangular DCT
        class CommonDCT : public DCT
        {
        public:

            virtual ~CommonDCT() throw();
            explicit CommonDCT(const unit_t W, const unit_t H);
            
            template <typename T>
            void forward(const pixmap<T> &src,
                         const unit_t     xx,
                         const unit_t     yy) throw()
            {
                pixmap<double> &self  = *this;
                const unit_t    N     = w;
                const unit_t    M     = h;
                
                for(unit_t j=0;j<M;++j)
                {
                    for(unit_t i=0;i<N;++i)
                    {
                        double       q   = 0;
                        const double wij = LAMBDA[j][i];
                        for(unit_t y=0;y<M;++y)
                        {
                            const double Cyj    = YCOS[y][j];
                            for(unit_t x=0;x<N;++x)
                            {
                                const double Cxi = XCOS[x][i];
                                const double wxy = Cxi*Cyj;
                                q += double( src[yy+y][xx+x] ) * wxy;
                            }
                        }
                        
                        self[j][i] = q * wij;
                    }
                }
            }
            
            inline void reverse() throw()
            {
                const pixmap<double> &dct = *this;
                const size_t N = w;
                const size_t M = h;
                for(size_t y=0;y<M;++y)
                {
                    for(size_t x=0;x<N;++x)
                    {
                        
                        double q = 0;
                        for(size_t j=0;j<M;++j)
                        {
                            const double Cyj    = YCOS[y][j];
                            for(size_t i=0;i<N;++i)
                            {
                                const double Cxi    = XCOS[x][i];
                                const double weight = LAMBDA[j][i] * Cxi * Cyj;
                                q += dct[j][i] * weight;
                            }
                        }
                        pix[y][x] = q;
                    }
                }
                
            }
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(CommonDCT);
            Table XCOS;
            Table YCOS;
            Table LAMBDA; //! warning: Lambda[j][i] !!!
        };
        

        //! Square DCT
        class SquareDCT : public DCT
        {
        public:
            virtual ~SquareDCT() throw();
            explicit SquareDCT(const unit_t W);

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(SquareDCT);
            Table COS;
            Table LAMBDA;
        };


        
        template <size_t N>
        class DCT0
        {
        public:
            static const size_t BLOCK_SIZE=N;
            inline  DCT0() throw() : COS(), LAM(), pix() { setup(); }
            inline ~DCT0() throw() {}
            
            const double COS[N][N];
            const double LAM[N][N];
            double       pix[4][N][N];
            double       dct[4][N][N];
            
            
            inline void forward(const size_t nch) throw()
            {
                assert(nch>0);
                assert(nch<=4);
                for(size_t i=0;i<N;++i)
                {
                    for(size_t j=0;j<N;++j)
                    {
                        double       q[4]    = {0,0,0,0};
                        const double LAM_ij = LAM[i][j];
                        for(size_t x=0;x<N;++x)
                        {
                            const double Cxi = COS[x][i];
                            for(size_t y=0;y<N;++y)
                            {
                                const double Cyj    = COS[y][j];
                                const double weight = Cxi*Cyj;
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
                        double q[4] = { 0,0,0,0 };
                        for(size_t i=0;i<N;++i)
                        {
                            const double Cxi = COS[x][i];
                            for(size_t j=0;j<N;++j)
                            {
                                const double Cyj    = COS[y][j];
                                const double weight = LAM[i][j] * Cxi * Cyj;
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
            YOCTO_DISABLE_COPY_AND_ASSIGN(DCT0);
            
            inline void setup() throw()
            {
                static const size_t NN     = N<<1;
                for(size_t i=0;i<N;++i)
                {
                    for(size_t j=0;j<N;++j)
                    {
                        const double arg      = (((2*i+1)*j)*3.141592653589793115997963468544185161590576171875)/NN;
                        (double &)(COS[i][j]) = cos(arg);
                    }
                }
                zpix();
                zdct();
                
                static const double two_over_N = 2.0 / N;
                static const double sq2_over_N = sqrt(2.0)/N;
                for(size_t i=1;i<N;++i)
                {
                    (double&)(LAM[0][i]) = sq2_over_N;
                    (double&)(LAM[i][0]) = sq2_over_N;
                    for(size_t j=1;j<N;++j)
                    {
                        (double&)(LAM[i][j]) = two_over_N;
                    }
                }
                
                (double&)(LAM[0][0]) = 1.0/N;
                
                
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
                
                
                inline friend std::ostream & operator<<(std::ostream &os, const DCT0<N> &dct)
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
