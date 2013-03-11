#include "yocto/spade/array3d.hpp"
#include "yocto/spade/visit.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/spade/mpi/workspace.hpp"
#include "yocto/spade/region3d.hpp"
#include "yocto/visit/interface.hpp"

using namespace yocto;
using namespace spade;

typedef coord3D  Coord;
typedef layout3D Layout;
typedef double   Real;

typedef geom::v3d<Real> Vertex;
typedef array3D<Real>   Array;
typedef mpi_workspace<Layout,rmesh,Real> Workspace;
typedef fields_setup<Layout>             FieldsSetup;
typedef ghosts_setup                     GhostsSetup;
typedef array1D<Real>                    Array1D;
typedef region3D<Real>::type             Region;



class Parameters 
{
public:
    
    FieldsSetup fields;
    GhostsSetup ghosts;
    explicit Parameters( const mpi &MPI ) :
    fields(sizeof(Real)),
    ghosts()
    {
        //-- declare the fields
        Y_SPADE_FIELD( fields, "U",  Array);
        Y_SPADE_FIELD( fields, "dU", Array);
        
        //-- declare the ghosts
        ghosts.set_local(on_x, 2); // PBC on x
        ghosts.set_local(on_y, 2); // PBC on y
        if( MPI.IsParallel )
        {
            ghosts.set_async(ghost::at_lower_z, 2, MPI.CommWorldPrev());
            ghosts.set_async(ghost::at_upper_z, 2, MPI.CommWorldNext());
        }
        else
            ghosts.set_local(on_z, 2);
        
    }
    
    virtual ~Parameters() throw()
    {
    }
    
    
private:
    YOCTO_DISABLE_COPY_AND_ASSIGN(Parameters);
};


class MySim :
public VisIt:: Simulation,
public Parameters,
public Workspace,
public VisItIO
{
public:
    Array         &U;
    Array         &dU;
    Array1D       &X;
    Array1D       &Y;
    Array1D       &Z;
    linear_handles handles; //!< for MPI

    explicit MySim(  const mpi &ref, const Layout &l) :
    VisIt::Simulation(ref),
    Parameters(MPI),
    Workspace( l, fields, ghosts  ),
    U(  (*this)["U"].as<Array>()  ),
    dU( (*this)["dU"].as<Array>() ),
    X( mesh.X() ),
    Y( mesh.Y() ),
    Z( mesh.Z() )
    {
        MPI.PrintfI(stderr, "Ready\n");
        query( handles, "U" );
    }
    
    virtual ~MySim() throw()
    {
    }
    
    void initialize()
    {
        U.ldz();
        sync(MPI,handles);
    }
    
    
    virtual void get_meta_data( visit_handle &md ) const
    {
        assert( VISIT_INVALID_HANDLE != md );
        
        MPI.Printf0(stderr, "\t\t+mesh\n");
        //! append the mesh
        {
            visit_handle mmd = mesh_meta_data(mesh, "mesh", par_size);
            VisIt_SimulationMetaData_addMesh(md, mmd);
        }
    
    }
    
    
    virtual visit_handle get_mesh( int domain, const string &name ) const
    {
        visit_handle h = VISIT_INVALID_HANDLE;
        if( name == "mesh" )
        {
            if( VisIt_RectilinearMesh_alloc(&h) == VISIT_OKAY )
            {
                assert( h != VISIT_INVALID_HANDLE );
                
                visit_handle   hx = VISIT_INVALID_HANDLE;
                visit_handle   hy = VISIT_INVALID_HANDLE;
                visit_handle   hz = VISIT_INVALID_HANDLE;
                
                VisIt_VariableData_alloc(&hx);
                VisIt_VariableData_setDataD(hx, VISIT_OWNER_SIM, 1, X.items, X.entry );
                
                VisIt_VariableData_alloc(&hy);
                VisIt_VariableData_setDataD(hy, VISIT_OWNER_SIM, 1, Y.items, Y.entry );
                
                VisIt_VariableData_alloc(&hz);
                VisIt_VariableData_setDataD(hz, VISIT_OWNER_SIM, 1, Z.items, Z.entry );
                
                VisIt_RectilinearMesh_setCoordsXYZ(h, hx, hy, hz);
                
#if 0
                int minRealIndex[3] = { 1,         1 ,        2        };
                int maxRealIndex[3] = { X.width-2, Y.width-2, Z.width-3};
                
                if(  par_rank < par_size - 1)
                    maxRealIndex[2]++;
                
                VisIt_RectilinearMesh_setRealIndices(h, minRealIndex, maxRealIndex);
#endif
            }
        }
        return h;
    }

    
private:
    
};

int main( int argc, char *argv[] )
{
    try
    {
        
        //----------------------------------------------------------------------
        //info for VisIt
        //----------------------------------------------------------------------
        const string sim_name    = "VisItPerf3D";
        const string sim_comment = "SimpleVisIt 3D";
        const string sim_path    = ".";
        
        //----------------------------------------------------------------------
        // initialize MPI
        //----------------------------------------------------------------------
        YOCTO_MPI;
        const int rank = MPI.CommWorldRank;
        const int size = MPI.CommWorldSize;
       
        
        //----------------------------------------------------------------------
        // parallel VisIt
        //----------------------------------------------------------------------
        const string       trace_name = "trace.dat";
        VisIt:: TraceFile  trace_file(rank,trace_name );
        VisIt:: SetupParallel( MPI, sim_name, sim_comment, sim_path, NULL);

        
        //----------------------------------------------------------------------
        // Geometry/Workspace
        //----------------------------------------------------------------------
        const Coord cmin(1,1,1);
        const Coord cmax(10,12,14);
        const Layout full_layout( cmin, cmax );
        const Layout sim_layout = full_layout.split(rank, size);

        //----------------------------------------------------------------------
        // Create Simulation
        //----------------------------------------------------------------------
        MySim sim(MPI,sim_layout);
        
        //----------------------------------------------------------------------
        // create mesh
        //----------------------------------------------------------------------
        const Region full_region( Vertex(-1,-1,-1), Vertex(1,1,1) );
        const Real   dx = full_region.length.x / full_layout.width.x;
        const Real   dy = full_region.length.y / full_layout.width.y;
        const Real   dz = full_region.length.z / full_layout.width.z;

        for( unit_t i= sim.X.lower; i <= sim.X.upper; ++i )
        {
            sim.X[i] = full_region.vmin.x + (i - full_layout.lower.x) * dx;
        }
        
        
        for( unit_t j=sim.Y.lower; j <= sim.Y.upper; ++j )
        {
            sim.Y[j] = full_region.vmin.y + (j- full_layout.lower.y) * dy;
        }
        

        for( unit_t k=sim.Z.lower; k<= sim.Z.upper; ++k )
        {
            sim.Z[k] = full_region.vmin.z + (k- full_layout.lower.z) * dz;
        }
       

        //----------------------------------------------------------------------
        //
        //----------------------------------------------------------------------
        sim.initialize();

        VisIt::MainLoop(sim);
        
        return 0;
    }
    catch( const exception &e )
    {
        std::cerr << e.what() << std::endl;
        std::cerr << e.when() << std::endl;
    }
    catch(...)
    {
        std::cerr << "Unhandled Exception.hpp" << std::endl;
    }
    return 1;
}