#include "yocto/utest/run.hpp"
#include "yocto/spade/mpi/workspace.hpp"
#include "yocto/spade/rmesh.hpp"
#include "yocto/spade/mpi/collect0.hpp"
#include "yocto/auto-ptr.hpp"
#include "yocto/spade/vtk/writer.hpp"

#include "../tests/main.hpp"
#include "yocto/ios/ocstream.hpp"
#include "yocto/code/utils.hpp"


#define LX (1.2)
#define NX (100)

#define LY (1.4)
#define NY (150)


typedef array2D<double> Array2D;
typedef array1D<double> Array1D;
typedef workspace<layout2D,rmesh,double>     Workspace;
typedef mpi_workspace<layout2D,rmesh,double> MPI_Workspace;


void save_field( const Array2D &A , vtk_writer &vtk)
{
    static int idx = 0;
    
    
    ++idx;
}

YOCTO_UNIT_TEST_IMPL(h2d)
{
    const mpi & MPI  = mpi::init( &argc, &argv );
    const int   rank = MPI.CommWorldRank;
    const int   size = MPI.CommWorldSize;
    const int   last = MPI.CommWorldLast;
    
    //-- register the fields
    fields_setup<layout2D> F(2);
    Y_SPADE_FIELD(F, "A",  Array2D );
    Y_SPADE_LOCAL(F, "LA", Array2D );
    
    
    
    //-- register the ghosts
    ghosts_setup G;
    if( rank < last) G.set_async(ghost::at_upper_y, 1, MPI.CommWorldNext() );
    if( rank > 0   ) G.set_async(ghost::at_lower_y, 1, MPI.CommWorldPrev() );
    
    //-- layout
    const layout2D full_layout( coord2D(0,0), coord2D(0,NY) );
    const double dX  = LX/NX;
    const double dY  = LY/NY;
    const double dX2 = dX * dX;
    const double dY2 = dY * dY;
    const double dt  = 0.2 * min_of( dX2, dY2);
    
    auto_ptr<Workspace> pCom;
    Array2D            *pA = 0;
    if(0==rank)
    {
        const ghosts_setup     no_ghosts;
        fields_setup<layout2D> io_fields(1);
        Y_SPADE_FIELD(io_fields, "A",  Array2D );
        pCom.reset( new Workspace(full_layout,io_fields,no_ghosts) );
        pA= & ((*pCom)["A"].as<Array2D>());
        Array1D &pX  = (*pCom)["X"].as<Array1D>();
        Array1D &pY  = (*pCom)["Y"].as<Array1D>();
        for( unit_t i=pX.lower;i<=pX.upper;++i) pX[i] = (i*LX)/NX;
        for( unit_t j=pY.lower;j<=pY.upper;++j) pY[j] = (j*LX)/NY;
    }
    
    
    //-- worspace
    const layout2D L( full_layout.split(rank,size,on_y) );
    mpi_workspace<layout2D,rmesh,double> W(L,F,G);
    Array2D &A  = W["A"].as<Array2D>();
    Array2D &LA = W["LA"].as<Array2D>();
    Array1D &X  = W["X"].as<Array1D>();
    Array1D &Y  = W["Y"].as<Array1D>();
    
    //-- setup
    for( unit_t i=X.lower;i<=X.upper;++i) X[i] = (i*LX)/NX;
    for( unit_t j=Y.lower;j<=Y.upper;++j) Y[j] = (j*LX)/NY;
    A.ldz();
    LA.ldz();
    
    //-- boundary condition
    if(0==rank)
    {
        assert(0==A.lower.y);
        for( unit_t i=A.lower.x;i<=A.upper.x;++i)
            A[A.lower.y][i] = 1;
    }
    
    vtk_writer vtk;
    
    W.sync(MPI);
    if( rank == 0 )
    {
        mpi_collect0::get(MPI, pA, A,full_layout);
        //save_field(*pA,vtk);
    }
    
    
}
YOCTO_UNIT_TEST_DONE()

