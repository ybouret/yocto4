#include "yocto/utest/run.hpp"
#include "yocto/spade/mpi/workspace.hpp"
#include "yocto/spade/rmesh.hpp"
#include "yocto/spade/mpi/collect0.hpp"
#include "yocto/ptr/auto.hpp"
#include "yocto/spade/vtk/writer.hpp"

#include "../tests/main.hpp"
#include "yocto/ios/ocstream.hpp"
#include "yocto/code/utils.hpp"


#define LX (1.2)
#define NX (20)

#define LY (1.4)
#define NY (30)


typedef array2D<double> Array2D;
typedef array1D<double> Array1D;
typedef workspace<layout2D,rmesh,double>     Workspace;
typedef mpi_workspace<layout2D,rmesh,double> MPI_Workspace;


void save_fields( const array<string> &var , const Workspace &W, vtk_writer &vtk)
{
    static int idx = 0;
    
    vtk.save( vformat("a%08d.vtk",idx), "a", W, var, W.as_layout());
    
    ++idx;
}

YOCTO_UNIT_TEST_IMPL(h2d)
{
    const mpi & MPI  = mpi::init( &argc, &argv , MPI_THREAD_SINGLE);
    const int   rank = MPI.CommWorldRank;
    const int   size = MPI.CommWorldSize;
    const int   last = MPI.CommWorldLast;
    
    //-- register the fields
    fields_setup<layout2D> F( 2*sizeof(double) );
    Y_SPADE_FIELD(F, "A",  Array2D );
    Y_SPADE_FIELD(F, "LA", Array2D );
    
    
    
    //-- register the ghosts
    ghosts_setup G;
    if( rank < last) G.set_async(ghost::at_upper_y, 1, MPI.CommWorldNext() );
    if( rank > 0   ) G.set_async(ghost::at_lower_y, 1, MPI.CommWorldPrev() );
    
    //-- layout
    const layout2D full_layout( coord2D(0,0), coord2D(NX,NY) );
    const double dX  = LX/NX;
    const double dY  = LY/NY;
    const double dX2 = dX * dX;
    const double dY2 = dY * dY;
    const double idX2 = 1.0/dX2;
    const double idY2 = 1.0/dY2;
    const double dt   = 0.2 * min_of( dX2, dY2);
    
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
    vector<string> var;
    if(0==rank)
        var.push_back("A");
    
    linear_handles handles;
    W.query(handles, "A");
    MPI.Printf(stderr, "#handles=%u\n", unsigned(handles.size()));
    
    
    W.sync(MPI,handles);
    
    mpi_collect0::get(MPI, pA, A,full_layout);
    if(0==rank)
    {
        save_fields( var, *pCom, vtk );
    }
    
    unsigned iter=0;
    for(;;)
    {
        ++iter;
        MPI.Printf0( stderr, "iter= %u\n", iter);
        //-- compute laplacian
        for( unit_t j=A.lower.y+1;j<A.upper.y;++j)
        {
            const Array1D &Aj = A[j];
            Array1D       &Lj = LA[j];
            for(unit_t i=A.lower.x+1;i<A.upper.x;++i)
            {
                const double A0 = Aj[i];
                const double mid = -(A0+A0);
                Lj[i] = (Aj[i+1] + mid + Aj[i-1]) * idX2 + ( A[j+1][i]+mid+A[j-1][i]) * idY2;
            }
        }
        
        //-- update field
        for( unit_t j=A.lower.y+1;j<A.upper.y;++j)
        {
            for(unit_t i=A.lower.x+1;i<A.upper.x;++i)
            {
                A[j][i] += LA[j][i] * dt;
            }
        }
        
        W.sync(MPI,handles);
        if( 0 == (iter%10) )
        {
            mpi_collect0::get(MPI, pA, A,full_layout);
            if(0==rank)
            {
                save_fields( var, *pCom, vtk );
            }
        }
        
        if(iter>5000)
            break;
    }
    
}
YOCTO_UNIT_TEST_DONE()

