#include "yocto/utest/run.hpp"
#include "yocto/spade/contour/iso3d.hpp"
#include "yocto/spade/workspace.hpp"
#include "yocto/spade/rmesh.hpp"
#include "yocto/spade/vtk/writer.hpp"
#include "yocto/spade/variables.hpp"

using namespace yocto;
using namespace spade;

typedef geom::v3d<double> vertex;
typedef iso3d<double,double> ISO3D;

namespace
{
    class lproc
    {
    public:
        
        lproc()
        {
        }
        
        ~lproc() throw() {}
        
        void process(  const triangle3D<double> &tr, const levels<double> &ls, size_t k)
        {
            
        }
        
    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(lproc);
    };
}

YOCTO_UNIT_TEST_IMPL(iso3d)
{
    
    const layout3D         L( coord3D(1,1,1), coord3D(50,50,50) );
    fields_setup<layout3D> F(4);
    ghosts_setup           G;
    const  vertex          V(1,1,1);
    const  vertex          Vc = 0.5*V; //(3.0*V)/4.0;
    std::cerr << "Center@" << Vc << std::endl;
    
    Y_SPADE_FIELD(F, "A", array3D<double> );
    
    workspace<layout3D, rmesh, double> W( L, F, G );
    array3D<double> &A = W["A"].as< array3D<double> >();
    array1D<double> &X = W.mesh.get_axis(on_x);
    array1D<double> &Y = W.mesh.get_axis(on_y);
    array1D<double> &Z = W.mesh.get_axis(on_z);
    
    for( unit_t i=X.lower; i <= X.upper;++i)
    {
        X[i] = double(i-X.lower) * V.x / (X.upper-X.lower);
    }
    
    
    for(unit_t j=Y.lower; j <= Y.upper; ++j )
    {
        Y[j] = double(j-Y.lower) * V.y / (Y.upper - Y.lower);
    }
    
    for(unit_t k=Z.lower; k <= Z.upper; ++k )
    {
        Z[k] = double(k-Z.lower) * V.z / (Z.upper - Z.lower);
    }
    
    for(unit_t k=Z.lower; k <= Z.upper; ++k )
    {
        const double z  = Z[k];
        const double dz = z - Vc.z;
        for( unit_t j=Y.lower;j<=Y.upper;++j)
        {
            const double y  = Y[j];
            const double dy = y - Vc.y;
            for( unit_t i=X.lower; i <= X.upper;++i)
            {
                const double x = X[i];
                const double dx = x - Vc.x;
                
                
                A[k][j][i] = 1.0 / ( 1 + 5*dy*dy + 7*dx*dx + 0.8*sin( 8 * dx ) ) * exp( -dz*dz );
            }
        }
    }
    
    vtk_writer vtk;
    {
        variables  var;
        var.append("A");
        vtk.save("A.vtk", "A", W, var, W.as_layout());
    }

    levels<double> ls;
    ls.insert( 0.5 );
    lproc           LP;
    ISO3D::callback cb( &LP, & lproc::process);
    
    ISO3D::compute(A, X, Y, Z, W.as_layout(), ls, cb);
    
}
YOCTO_UNIT_TEST_DONE()
