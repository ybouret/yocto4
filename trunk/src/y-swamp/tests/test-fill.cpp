#include "yocto/utest/run.hpp"
#include "yocto/swamp/fill.hpp"
#include "yocto/swamp/workspace.hpp"
#include "yocto/swamp/rectilinear-mesh.hpp"
#include "yocto/ios/ocstream.hpp"

using namespace yocto;
using namespace swamp;

namespace {
    
    struct filler
    {
        double F1( double X )
        {
            return cos( 3*X );
        }
        
        double F2( double x, double y )
        {
            return sin(3*sqrt(x*x+y*y));
        }
    };
 
    static inline double vproc( const double &x ) { return x; }
}

YOCTO_UNIT_TEST_IMPL(fill)
{
    typedef fill<double,double> fill_type;
    filler           F;

    {
        const layout1D  L(1,100);
        const ghosts_setup<coord1D> no_ghosts;
        workspace<layout1D,double,rectilinear_mesh> W(L,no_ghosts);
        const  region1D<double>::type R(-1,1);
        W.mesh.regular_map_to(R, L);
        
        array1D<double> &A = W.create< array1D<double> >( "A" );
        fill_type::function1D f1( &F, & filler::F1 );
        
        fill_type::in(A, W.__layout(), f1, W.mesh.X() );
        
        ios::ocstream fp( "a1.dat", false );
        for( unit_t i=W.lower; i <= W.upper; ++i )
        {
            fp("%g %g\n", W.mesh.X()[i], A[i] );
        }
    }
    
    {
        const coord2D lo(1,1);
        const coord2D hi(100,100);
        const layout2D L(lo,hi);
        const ghosts_setup<coord2D> no_ghosts;
        workspace<layout2D,double,rectilinear_mesh> W(L,no_ghosts);
        
        const geom::v2d<double> vlo(-1,-1);
        const geom::v2d<double> vhi(1,1);
        const region2D<double>::type R(vlo,vhi);
        
        W.mesh.regular_map_to(R,L);
        
        const array1D<double> &X = W.mesh.X();
        const array1D<double> &Y = W.mesh.Y();
        fill_type::function2D f2( &F, & filler::F2 );
        
        array2D<double> &A = W.create< array2D<double> >( "A" );
        
        fill_type::in(A,L,f2,X,Y);
        
        A.ppm("a2.ppm", "fill", L, vproc, NULL, -1, 1);
        
    }
    
    
}
YOCTO_UNIT_TEST_DONE()
