#include "yocto/program.hpp"
#include "yocto/sequence/list.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/math/types.hpp"

using namespace yocto;
using namespace math;

void ComputedUdz( array<double> &dUdz, double z, const array<double> &U, const array<double> &params)
{
    const double r    =  U[1];
    const double drdz =  U[2];

    const double speed = sqrt( 1.0 + drdz*drdz );

    const double K     = params[1];

    dUdz[1] = drdz;
    dUdz[2] = speed*speed*speed * ( K*K*y + 1.0/speed/r);
}

void UpdateRK4( array<double>      &U_new,
               const double         z,
               const double         z_new,
               const array<double> &U,
               const array<double> &params)
{

}

YOCTO_PROGRAM_START()
{
    vector<double> U[2];


}
YOCTO_PROGRAM_END()