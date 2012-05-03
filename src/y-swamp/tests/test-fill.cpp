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
    };
    
}

YOCTO_UNIT_TEST_IMPL(fill)
{
    typedef fill<double,double> fill_type;
   
    {
        const layout1D  L(1,100);
        const ghosts_setup<coord1D> no_ghosts;
        workspace<layout1D,double,rectilinear_mesh> W(L,no_ghosts);
        const  region1D<double>::type R(-1,1);
        W.mesh.regular_map_to(R, L);
        
        array1D<double> &A = W.create< array1D<double> >( "A" );
        filler           F;
        fill_type::function1D f1( &F, & filler::F1 );
        
        fill_type::in(A, W.__layout(), f1, W.mesh.X() );
        
        ios::ocstream fp( "a1.dat", false );
        for( unit_t i=W.lower; i <= W.upper; ++i )
        {
            fp("%g %g\n", W.mesh.X()[i], A[i] );
        }
    }
    
    
    
}
YOCTO_UNIT_TEST_DONE()
