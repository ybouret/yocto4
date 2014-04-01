#include "yocto/utest/run.hpp"
#include "yocto/math/kernel/crout.hpp"
#include "yocto/math/v2d.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/ios/ocstream.hpp"

using namespace yocto;
using namespace math;


#define gen_coord() (alea<double>()-0.5)
YOCTO_UNIT_TEST_IMPL(tri2d)
{
    matrix<double> M(6,6);
    crout<double>  LU(6);
    vector<double> coeff_x(4,0);
    vector<double> coeff_y(4,0);

    {
        v2d<double> A( gen_coord(), 1+gen_coord() );
        v2d<double> B( -1+gen_coord(), gen_coord() );
        v2d<double> C( 1+gen_coord(), gen_coord() );
        v2d<double> D( gen_coord(), -1+gen_coord() );
        v2d<double> P = (A+B+C)/3.0;
        v2d<double> Q = (B+C+D)/3.0;
        const v2d<double> *V[6] = { &A, &B, &C, &D, &P, &Q };
        for(size_t i=0;i<6;++i)
        {
            const v2d<double> &v = *V[i];
            array<double>     &m = M[i+1];
            m[1] = 1;
            m[2] = v.x;
            m[3] = v.y;
            m[4] = 0.5*v.x*v.x;
            m[5] = v.x * v.y;
            m[6] = 0.5*v.y*v.y;
        }
        std::cerr << "M=" << M << std::endl;
        if( ! LU.inverse(M) )
        {
            throw exception("inversion error");
        }
        std::cerr << "W=" << M << std::endl;
        const array<double> &rx = M[2];
        const array<double> &ry = M[3];
        std::cerr << "rx=" << rx << std::endl;
        std::cerr << "ry=" << ry << std::endl;
        coeff_x[1] = rx[1] +  rx[5]/3;
        coeff_x[2] = rx[2] + (rx[5]+rx[6])/3;
        coeff_x[3] = rx[3] + (rx[5]+rx[6])/3;
        coeff_x[4] = rx[4] +  rx[6]/3;
        
        std::cerr << "coeff_x=" << coeff_x << std::endl;
        
        coeff_y[1] = ry[1] +  ry[5]/3;
        coeff_y[2] = ry[2] + (ry[5]+ry[6])/3;
        coeff_y[3] = ry[3] + (ry[5]+ry[6])/3;
        coeff_y[4] = ry[4] +  ry[6]/3;
        std::cerr << "coeff_y=" << coeff_y << std::endl;

        ios::ocstream fp("diam.dat",false);
        fp("%g %g\n", A.x,A.y);
        fp("%g %g\n", B.x,B.y);
        fp("%g %g\n", C.x,C.y);
        fp("%g %g\n", A.x,A.y);
        fp("\n");
        
        fp("%g %g\n", D.x,D.y);
        fp("%g %g\n", C.x,C.y);
        fp("%g %g\n", B.x,B.y);
        fp("%g %g\n", D.x,D.y);
        fp("\n");
    }
}
YOCTO_UNIT_TEST_DONE()
