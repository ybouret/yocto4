#include "yocto/gfx/ops/dct.hpp"
#include "yocto/exceptions.hpp"
#include "yocto/code/utils.hpp"

namespace yocto
{
    namespace gfx
    {

        DCT:: ~DCT() throw() {}


        DCT:: DCT(const unit_t W, const unit_t H) :
        pixmap<double>(W,H),
        XCOS( patch(vertex(0,0),vertex(w-1,w-1) ) ),
        YCOS( patch(vertex(0,0),vertex(h-1,h-1) ) ),
        LAMBDA(*this)
        {
            std::cerr << "XCOS=" << XCOS << std::endl;
            std::cerr << "YCOS=" << YCOS << std::endl;
            std::cerr << "LAMB=" << LAMBDA << std::endl;

            std::cerr << "--> XCOS" << std::endl;
            const size_t N  = size_t(w);
            const size_t NN = N<<1;
            for(size_t i=0;i<N;++i)
            {
                for(size_t j=0;j<N;++j)
                {
                    const double arg = (((2*i+1)*j)*3.141592653589793115997963468544185161590576171875)/NN;
                    XCOS[i][j]        = cos(arg);
                }
            }

            std::cerr << "--> YCOS" << std::endl;
            const size_t M  = size_t(h);
            const size_t MM = M<<1;
            for(size_t i=0;i<M;++i)
            {
                for(size_t j=0;j<M;++j)
                {
                    const double arg = (((2*i+1)*j)*3.141592653589793115997963468544185161590576171875)/MM;
                    YCOS[i][j]        = cos(arg);
                }
            }




            const double den          = sqrt( double(N) * double(M) );
            const double two_over_den = 2.0/den;
            const double sq2_over_den = sqrt(2.0)/den;
            std::cerr << "--> LAMBDA_ZERO" << std::endl;
            LAMBDA[0][0] = 1.0/den;
            std::cerr << "--> LAMBDA_CORE" << std::endl;
            for(size_t j=1;j<M;++j)
            {
                for(size_t i=1;i<N;++i)
                {
                    LAMBDA[j][i] = two_over_den;
                }
            }

            std::cerr << "--> LAMBDA_X" << std::endl;
            for(size_t i=1;i<N;++i)
            {
                LAMBDA[0][i] = sq2_over_den;
            }

            std::cerr << "--> LAMBDA_Y" << std::endl;
            for(size_t j=1;j<M;++j)
            {
                LAMBDA[j][0] = sq2_over_den;
            }
            std::cerr << "--> done" << std::endl;
        }
        
        
        
    }
    
}

