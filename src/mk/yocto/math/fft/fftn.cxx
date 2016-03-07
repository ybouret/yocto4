#include "yocto/math/ztype.hpp"
#include "yocto/math/fft/fftn.hpp"
#include "yocto/code/bswap.hpp"
#include "yocto/math/types.hpp"
#include "yocto/code/utils.hpp"

#include <cmath>

namespace yocto
{
    namespace math
    {
        
        template <>
        void FourierN<real_t>:: Compute(real_t *data, const size_t nn[], const size_t ndim, int isign) throw()
        {
            /*
            std::cerr << "fft" << ndim << "(";
            for(size_t i=1;i<=ndim;++i)
            {
                std::cerr << " " << nn[i];
            }
            std::cerr << " , " << isign;
            std::cerr << " )" << std::endl;
             */
            assert(data);
            assert(nn);
            assert(ndim>0);
            assert(1==isign||-1==isign);
            
            size_t i1,i2,i3,i2rev,i3rev,ip1,ip2,ip3,ifp1,ifp2;
            size_t ibit,k1,k2,n,nprev,nrem,ntot;
            real_t  tempi,tempr;
            double  theta,wi,wpi,wpr,wr,wtemp;
            
            ntot=1;
            for(size_t idim=1;idim<=ndim;++idim)
            {
                assert(is_a_power_of_two(nn[idim]));
                ntot *= nn[idim];
            }
            nprev=1;
            for(size_t idim=ndim;idim>=1;--idim)
            {
                n     = nn[idim];
                nrem  = ntot/(n*nprev);
                ip1   = nprev << 1;
                ip2   = ip1*n;
                ip3   = ip2*nrem;
                i2rev = 1;
                for (i2=1;i2<=ip2;i2+=ip1)
                {
                    if (i2 < i2rev)
                    {
                        for (i1=i2;i1<=i2+ip1-2;i1+=2)
                        {
                            for (i3=i1;i3<=ip3;i3+=ip2)
                            {
                                i3rev=i2rev+i3-i2;
                                cswap(data[i3],data[i3rev]);
                                cswap(data[i3+1],data[i3rev+1]);
                            }
                        }
                    }
                    ibit=ip2 >> 1;
                    while (ibit >= ip1 && i2rev > ibit)
                    {
                        i2rev -= ibit;
                        ibit >>= 1;
                    }
                    i2rev += ibit;
                }
                
                ifp1=ip1;
                const double sgn_two_pi = numeric<double>::two_pi * isign;
                while (ifp1 < ip2)
                {
                    ifp2  = ifp1 << 1;
                    theta = sgn_two_pi/(ifp2/ip1);
                    wtemp = sin(0.5*theta);
                    wpr   = -2.0*wtemp*wtemp;
                    wpi   = sin(theta);
                    wr    = 1;
                    wi    = 0;
                    for (i3=1;i3<=ifp1;i3+=ip1)
                    {
                        for (i1=i3;i1<=i3+ip1-2;i1+=2)
                        {
                            for (i2=i1;i2<=ip3;i2+=ifp2)
                            {
                                k1=i2;
                                k2=k1+ifp1;
                                tempr=real_t(wr)*data[k2]  -real_t(wi)*data[k2+1];
                                tempi=real_t(wr)*data[k2+1]+real_t(wi)*data[k2];
                                data[k2]    = data[k1]-tempr;
                                data[k2+1]  = data[k1+1]-tempi;
                                data[k1]   += tempr;
                                data[k1+1] += tempi;
                            }
                        }
                        wr=(wtemp=wr)*wpr-wi*wpi+wr;
                        wi=wi*wpr+wtemp*wpi+wi;
                    }
                    ifp1=ifp2;
                }
                nprev *= n;
            }

            
            const real_t coef = real_t(1.0/sqrt( double(ntot) ));
#define _YOCTO_FFT_SCALE(INDEX) data[INDEX] *= coef
            YOCTO_LOOP_FUNC_(ntot<<1,_YOCTO_FFT_SCALE,1);


        }
        
        
    }
}
