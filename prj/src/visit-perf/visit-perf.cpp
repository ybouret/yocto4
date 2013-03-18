#include "yocto/spade/array3d.hpp"
#include "yocto/spade/visit.hpp"
#include "yocto/spade/ops/laplacian.hpp"

#include "yocto/code/rand.hpp"
#include "yocto/spade/mpi/workspace.hpp"
#include "yocto/spade/region3d.hpp"
#include "yocto/visit/interface.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/math/round.hpp"
#include "yocto/code/pid.hpp"

using namespace yocto;
using namespace spade;

typedef coord3D  Coord;
typedef layout3D Layout;
typedef double   Real;

typedef geom::v3d<Real>                  Vertex;
typedef array3D<Real>                    Array;
typedef mpi_workspace<Layout,rmesh,Real> Workspace;
typedef fields_setup<Layout>             FieldsSetup;
typedef ghosts_setup                     GhostsSetup;
typedef array1D<Real>                    Array1D;
typedef region3D<Real>::type             Region;



class Parameters
{
public:
    static const size_t NumGhosts = 1;
    
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
        ghosts.set_local(on_x, NumGhosts); // PBC on x
        ghosts.set_local(on_y, NumGhosts); // PBC on y
        if( MPI.IsParallel )
        {
            ghosts.set_async(ghost::at_lower_z, NumGhosts, MPI.CommWorldPrev());
            ghosts.set_async(ghost::at_upper_z, NumGhosts, MPI.CommWorldNext());
        }
        else
            ghosts.set_local(on_z, NumGhosts); // PBC on z, anyway
        
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
    double         dt;
    const Vertex   delta;
    const Vertex   delsq;
    const Vertex   inv_dsq;
    linear_handles handles; //!< for MPI
    Real           Du;
    mutable double sum_steps;
    mutable size_t num_steps;
    mutable double sum_tcomm;
    bool           do_sync;
    int            num_iter;
    size_t         counts;
    
    explicit MySim(const mpi    &ref,
                   const Layout &l,
                   const Vertex &d ) :
    VisIt::Simulation(ref),
    Parameters(MPI),
    Workspace( l, fields, ghosts   ),
    U(  (*this)["U" ].as<Array>()  ),
    dU( (*this)["dU"].as<Array>()  ),
    X( mesh.X() ),
    Y( mesh.Y() ),
    Z( mesh.Z() ),
    dt(0),
    delta(d),
    delsq(delta.x*delta.x,delta.y*delta.y,delta.z*delta.z),
    inv_dsq( 1.0/delsq.x, 1.0/delsq.y, 1.0/delsq.z),
    handles(),
    Du( 5.0 ),
    sum_steps(0),
    num_steps(0),
    sum_tcomm(0),
    do_sync(true),
    num_iter(1),
    counts(0)
    {
        query( handles, "U" );
        dt = math::log_round(0.1 * min_of( delsq.x, min_of(delsq.y, delsq.z))/max_of(Du,Du));
        num_iter = int(ceil(0.1/dt));
        MPI.PrintfI(stderr, "Ready (dt=%g|num_iter=%d)\n", dt, num_iter);

    }
    
    virtual ~MySim() throw()
    {
    }
    
    void initialize()
    {
        U.ldz();
        for( unit_t i=lower.x;i<=upper.x;++i)
        {
            for(unit_t j=lower.y;j<=upper.y;++j)
            {
                for(unit_t k=lower.z;k<=upper.z;++k)
                {
                    U[k][j][i] = 0.5*(1-2*alea<Real>());
                }
            }
        }
        sync(MPI,handles);
        counts=0;
    }
    
    
    virtual void get_meta_data( visit_handle &md ) const
    {
        assert( VISIT_INVALID_HANDLE != md );
        //! append an UI command
        add_generic_command("raz", md);
        
        //! append an UI command
        add_generic_command("quit", md);
        
        //! append the mesh
        {
            visit_handle mmd = mesh_meta_data(mesh, "mesh", par_size);
            VisIt_SimulationMetaData_addMesh(md, mmd);
            VisIt_MeshMetaData_setXLabel(mmd, "X");
            VisIt_MeshMetaData_setYLabel(mmd, "Y");
            VisIt_MeshMetaData_setZLabel(mmd, "Z");
        }
        
        //! append U on mesh
        {
            visit_handle vmd = variable_meta_data<Real>("U", "mesh");
            VisIt_SimulationMetaData_addVariable(md, vmd);
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
                int minRealIndex[3];
                int maxRealIndex[3];
                VisIt_RectilinearMesh_getRealIndices(h,minRealIndex,maxRealIndex);
                MPI.PrintfI( stderr,
                            "minRealIndex= { %d, %d, %d } maxRealIndex={ %d, %d, %d } widths={ %d, %d, %d}\n",
                            minRealIndex[0],
                            minRealIndex[1],
                            minRealIndex[2],
                            maxRealIndex[0],
                            maxRealIndex[1],
                            maxRealIndex[2],
                            int(X.width),
                            int(Y.width),
                            int(Z.width)
                            );
#endif
                
#if 0
                int minRealIndex[3] = { 0,         0 ,        0        };
                int maxRealIndex[3] = { X.width-1, Y.width-1, Z.width-1};
                if( par_rank < par_last ) -- maxRealIndex[2];
                
                VisIt_RectilinearMesh_setRealIndices(h, minRealIndex, maxRealIndex);
#endif
            }
        }
        return h;
    }
    
    virtual visit_handle get_variable( int domain, const string &name ) const
    {
        //MPI.Printf( stderr, "rank %d> SIM:get_variable '%s', domain=%d\n", par_rank, name.c_str(), domain);
        visit_handle h = VISIT_INVALID_HANDLE;
        
        if( name == "U" )
        {
            const int nComponents=1;
            const int nTuples    =U.items;
            //MPI.Printf0( stderr, "Sending U: %dx%d\n", nComponents, nTuples);
            assert(U.entry!=NULL);
            if(VisIt_VariableData_alloc(&h) == VISIT_OKAY)
            {
                VisIt_VariableData_setDataD(h, VISIT_OWNER_SIM, nComponents, nTuples, U.entry);
            }
        }
        
        return h;
    }
    
    
    //! auxiliary commands
    virtual void perform( const string &cmd, const array<string> & )
    {
        MPI.PrintfI( stderr, "cmd: %s\n", cmd.c_str() );
        if( cmd == "raz" )
        {
            initialize();
            cycle   = 0;
            runTime = 0;
        }
    }
    
    
    virtual void step_prolog()
    {
        VisIt::Simulation::step_prolog();
    }
    
    virtual void step()
    {
        for( int iter=0; iter<num_iter;++iter)
        {
            ++counts;
            runTime = counts * dt;
            
            laplacian<Real>::compute( dU, U, as_layout(), inv_dsq);
            
            for(unit_t k=lower.z;k<=upper.z;++k)
            {
                for(unit_t j=lower.y;j<=upper.y;++j)
                {
                    for( unit_t i=lower.x;i<=upper.x;++i)
                    {
                        const Real u = U[k][j][i];
                        U[k][j][i] += dt * ( (u - u*u*u) + Du * dU[k][j][i] );
                    }
                }
            }
            if(do_sync)
                sync(MPI,handles);
        }
    }
    
    virtual void step_epilog()
    {
        sum_steps += stepTime;
        sum_tcomm += double(commTime);
        num_steps += 1;
        MPI.Printf0(stderr,
                    "\tsteps/s = %8.2f < %8.2f >\n"
                    "\tloops/s = %8.2f [ %8.5fx ]\n"
                    "\tComm/us = %8u [ %8.3f%% ] < %8.3f%% >\n"
                    ,
                    1.0/stepTime,
                    num_steps/sum_steps,
                    1.0/loopTime,
                    stepTime/loopTime,
                    commTime,
                    ( 1e-4 * double(commTime) ) / stepTime,
                    ( 1e-4 * sum_tcomm) / sum_steps
                    );
        
    }
    
    
private:
    YOCTO_DISABLE_COPY_AND_ASSIGN(MySim);
};

#include "yocto/string/conv.hpp"
#include "yocto/string/env.hpp"

int main( int argc, char *argv[] )
{
    try
    {
        if( argc <= 3 )
            throw exception("usage: simu Nx Ny Nz");
        const size_t Nx = strconv::to_size( argv[1], "Nx");
        const size_t Ny = strconv::to_size( argv[2], "Ny");
        const size_t Nz = strconv::to_size( argv[3], "Nz");
        
        _rand.wseed( get_process_h32() );
        
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
        const Coord cmax(Nx,Ny,Nz);
        const Layout full_layout( cmin, cmax );
        const Layout sim_layout = full_layout.split(rank, size);
        const Region full_region( Vertex(0,0,0), Vertex(100,150,200) );
        const Vertex delta(full_region.length.x / full_layout.width.x,
                           full_region.length.y / full_layout.width.y,
                           full_region.length.z / full_layout.width.z
                           );
        
        //----------------------------------------------------------------------
        // Create Simulation
        //----------------------------------------------------------------------
        MySim sim(MPI,sim_layout,delta);
        //sim.console = false;
        
        //----------------------------------------------------------------------
        // create mesh
        //----------------------------------------------------------------------
        
        
        for( unit_t i= sim.X.lower; i <= sim.X.upper; ++i )
        {
            sim.X[i] = full_region.vmin.x + (i - full_layout.lower.x) * delta.x;
        }
        
        
        for( unit_t j=sim.Y.lower; j <= sim.Y.upper; ++j )
        {
            sim.Y[j] = full_region.vmin.y + (j- full_layout.lower.y) * delta.y;
        }
        
        
        for( unit_t k=sim.Z.lower; k<= sim.Z.upper; ++k )
        {
            sim.Z[k] = full_region.vmin.z + (k- full_layout.lower.z) * delta.z;
        }
        
        
        //----------------------------------------------------------------------
        //
        //----------------------------------------------------------------------
        sim.initialize();
        sim.do_sync = true; //! default
        {
            string EnvSync;
            if( environment::get(EnvSync, "SYNC") )
            {
                if( EnvSync == "true" )
                {
                    sim.do_sync = true;
                    goto HAS_SYNC;
                }
                
                if( EnvSync == "false" )
                {
                    sim.do_sync = false;
                    goto HAS_SYNC;
                }
                
                throw exception("Invalid Env 'SYNC=%s", EnvSync.c_str());
            }
        HAS_SYNC:;
        }
        MPI.PrintfI( stderr, "Synchronizing/MPI: %s\n" , sim.do_sync ? "TRUE" : "FALSE" );
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
