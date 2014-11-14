#include "yocto/utest/run.hpp"
#include "yocto/spade/contour/iso2d.hpp"
#include "yocto/spade/workspace.hpp"
#include "yocto/spade/rmesh.hpp"
#include "yocto/spade/vtk/writer.hpp"
#include "yocto/spade/variables.hpp"
#include <cmath>

using namespace yocto;
using namespace spade;

typedef math::v2d<double>    vertex;
typedef iso2d<double,double> ISO2D;

namespace
{
    class lproc
    {
    public:
        vector< vector<vertex> > vv;
        
        explicit lproc() : vv()
        {
            
        }
        
        virtual ~lproc() throw() {}
        
        void init( const levels<double> &ls )
        {
            vv.free();
            vv.reserve(ls.size());
            for( size_t i=1; i <= ls.size(); ++i )
            {
                const vector<vertex> tmp;
                vv.push_back(tmp);
            }
        }
        
        void perform(const vertex &a,const vertex &b,const levels<double> &ls, size_t idx )
        {
            assert(idx>0);
            assert(idx<=ls.size());
            vector<vertex> &v = vv[idx];
            //std::cerr << ls[idx] << ": " << a << "-> " << b << std::endl;
            v.push_back(a);
            v.push_back(b);
        }
        
        void save( const levels<double> &ls ) const
        {
            for( size_t i=1; i <= ls.size(); ++i )
            {
                const string  fn = vformat("l%g.dat", ls[i] );
                ios::ocstream fp( fn, false);
                
                const vector<vertex> &v = vv[i];
                const size_t nv = v.size()/2;
                for( size_t j=1,k=1;j<=nv;++j)
                {
                    const vertex &a = v[k++];
                    const vertex &b = v[k++];
                    fp("%g %g\n", a.x, a.y );
                    fp("%g %g\n", b.x, b.y );
                    fp("\n");
                }
            }
        }
        
    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(lproc);
    };
}


YOCTO_UNIT_TEST_IMPL(levels)
{
 
    
    const layout2D         L( coord2D(1,1), coord2D(50,50) );
    fields_setup<layout2D> F(4);
    ghosts_setup           G;
    const  vertex          V(1,1);
    const  vertex          Vc = V/2.0; //(3.0*V)/4.0;
    std::cerr << "Center@" << Vc << std::endl;
    
    Y_SPADE_FIELD(F, "A", array2D<double> );
    
    workspace<layout2D, rmesh, double> W( L, F, G );
    array2D<double> &A = W["A"].as< array2D<double> >();
    array1D<double> &X = W.mesh.get_axis(on_x);
    array1D<double> &Y = W.mesh.get_axis(on_y);

    for( unit_t i=X.lower; i <= X.upper;++i)
    {
        X[i] = double(i-X.lower) * V.x / (X.upper-X.lower);
    }
    
    
    for(unit_t j=Y.lower; j <= Y.upper; ++j )
    {
        Y[j] = double(j-Y.lower) * V.y / (Y.upper - Y.lower);
    }
    
    for( unit_t j=Y.lower;j<=Y.upper;++j)
    {
        const double y  = Y[j];
        const double dy = y - Vc.y;
        for( unit_t i=X.lower; i <= X.upper;++i)
        {
            const double x = X[i];
            const double dx = x - Vc.x;
            
            
            A[j][i] = 1.0 / ( 1 + 5*dy*dy + 7*dx*dx + 0.8*sin( 8 * dx ) );
        }
    }
    
    {
        vtk_writer vtk;
        variables  var;
        var.append("A");
        vtk.save("A.vtk", "A", W, var, W.as_layout());
    }
    
    
    lproc           LP;
    ISO2D::callback cb( &LP, & lproc::perform);
    levels<double>  ls;
    
    
    ls.insert( 0.99 );
    ls.insert( 0.5  );
    
    LP.init(ls);
    ISO2D::compute(ls, A, X, Y, A, cb);
    
    LP.save(ls);
    
    
    
}
YOCTO_UNIT_TEST_DONE()
