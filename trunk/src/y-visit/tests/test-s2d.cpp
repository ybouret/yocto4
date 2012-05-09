#include "yocto/utest/run.hpp"
#include "yocto/visit/interface.hpp"
#include "yocto/wtime.hpp"

#include "yocto/swamp/common.hpp"
#include "yocto/swamp/mpi.hpp"
#include "yocto/code/rand.hpp"

using namespace yocto;
using namespace swamp;

typedef double               Real;
typedef coord2D              Coord;
typedef array2D<Real>        Array;
typedef layout2D             Layout;
typedef region2D<Real>::type Region;
typedef vertex2D<Real>::type Vertex;
typedef workspace<Layout, Real,rmesh> Workspace;
typedef fields_setup<Layout> FieldsSetup;
typedef ghosts_setup<Coord>  GhostsSetup;
typedef array1D<Real>        Array1D;



namespace 
{
    
    class MyFields : public FieldsSetup
    {
    public:
        explicit MyFields() : FieldsSetup(8)
        {
            Y_SWAMP_DECL_VAR( *this, "U", Array);
            Y_SWAMP_DECL_VAR( *this, "V", Array);
        }
        
        virtual ~MyFields() throw()
        {
        }
        
        
    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(MyFields);
    };
    
    
    class MySim : 
    public VisIt :: Simulation,
    public MyFields,
    public Workspace
    {
    public:
        Array &U;
        Array &V;
        mpi::Requests requests;
        const Array1D &X;
        const Array1D &Y;
        
        explicit MySim(  const mpi &ref, const Layout &L, const GhostsSetup &G ) :
        VisIt::Simulation( ref ),
        MyFields(),
        Workspace( L, G, *this ),
        U( (*this)["U"].as<Array>() ),
        V( (*this)["V"].as<Array>() ),
        requests( num_requests() ),
        X(mesh.X()),
        Y(mesh.Y())
        {
            prepare_ghosts();
        }
        
        
        void initialize()
        {
            MPI.Printf( stderr, "rank %d> Initializing U/V\n", par_rank);
            for( unit_t j=lower.y; j<=upper.y; ++j )
            {
                const Real y = Y[j];
                for( unit_t i=lower.x;i<=upper.x;++i)
                {
                    const Real x= X[i];
                    const Real r2 = x*x+y*y;
                    const Real r  = sqrt(r2);
                    
                    U[j][i] = cos(5*r);// +  (0.5 - alea<Real>());
                    V[j][i] = Real(par_rank+1)/Real(par_size);
                }
            }
        }
        
        virtual ~MySim() throw()
        {}
        
        
        inline void init_exchange()
        {
            _mpi::init_exchange(MPI, *this, requests);
        }
        
        inline void wait_exchange()
        {
            _mpi::wait_exchange(MPI, *this, requests);
        }
        
        
        virtual void get_meta_data( visit_handle &md ) const
        {
            assert( VISIT_INVALID_HANDLE != md );
            
            {
                visit_handle mmd = VISIT_INVALID_HANDLE;
                if( VisIt_MeshMetaData_alloc( &mmd ) == VISIT_OKAY )
                {
                    assert( VISIT_INVALID_HANDLE != mmd );
                    VisIt_MeshMetaData_setName( mmd, "mesh2d" );
                    VisIt_MeshMetaData_setMeshType( mmd, VISIT_MESHTYPE_RECTILINEAR );
                    VisIt_MeshMetaData_setTopologicalDimension( mmd, 2);
                    VisIt_MeshMetaData_setSpatialDimension(mmd, 2);
                    VisIt_MeshMetaData_setNumDomains(mmd, par_size);
                    
                    VisIt_SimulationMetaData_addMesh(md, mmd);
                }
            }
            
            {
                visit_handle vmd = VISIT_INVALID_HANDLE;
                if(VisIt_VariableMetaData_alloc(&vmd) == VISIT_OKAY)
                {
                    VisIt_VariableMetaData_setName(vmd, "U");
                    VisIt_VariableMetaData_setMeshName(vmd, "mesh2d");
                    VisIt_VariableMetaData_setType(vmd, VISIT_VARTYPE_SCALAR);
                    VisIt_VariableMetaData_setCentering(vmd, VISIT_VARCENTERING_NODE);
                    VisIt_SimulationMetaData_addVariable(md, vmd);
                }
            }
            
            {
                visit_handle vmd = VISIT_INVALID_HANDLE;
                if(VisIt_VariableMetaData_alloc(&vmd) == VISIT_OKAY)
                {
                    VisIt_VariableMetaData_setName(vmd, "V");
                    VisIt_VariableMetaData_setMeshName(vmd, "mesh2d");
                    VisIt_VariableMetaData_setType(vmd, VISIT_VARTYPE_SCALAR);
                    VisIt_VariableMetaData_setCentering(vmd, VISIT_VARCENTERING_NODE);
                    VisIt_SimulationMetaData_addVariable(md, vmd);
                }
            }
            
            
            
        }
        
        virtual visit_handle get_mesh( int domain, const string &name ) const
        {
            //MPI.Printf( stderr, "rank %d> SIM:get_mesh '%s', domain=%d\n", par_rank, name.c_str(), domain);
            visit_handle h = VISIT_INVALID_HANDLE;
            if( name == "mesh2d" )
            {
                if( VisIt_RectilinearMesh_alloc(&h) == VISIT_OKAY )
                {
                    assert( h != VISIT_INVALID_HANDLE );
                    const Array1D &X  = mesh.X();
                    const Array1D &Y  = mesh.Y();
                    
                    visit_handle   hx = VISIT_INVALID_HANDLE;
                    visit_handle   hy = VISIT_INVALID_HANDLE;
                    VisIt_VariableData_alloc(&hx);
                    VisIt_VariableData_setDataD(hx, VISIT_OWNER_SIM, 1, X.items, (double *)&X[ X.lower ] );
                    VisIt_VariableData_alloc(&hy);
                    VisIt_VariableData_setDataD(hy, VISIT_OWNER_SIM, 1, Y.items, (double *)&Y[ Y.lower ] );
                    
                    VisIt_RectilinearMesh_setCoordsXY(h, hx, hy);
                }
            }
            return h;
        }
        
        
        virtual visit_handle get_variable( int domain, const string &name ) const
        {
            MPI.Printf( stderr, "rank %d> SIM:get_variable '%s', domain=%d\n", par_rank, name.c_str(), domain);
            visit_handle h = VISIT_INVALID_HANDLE;
            
            if( name == "U" )
            {
                const int nComponents=1;
                const int nTuples    =U.items;
                MPI.Printf0( stderr, "Sending U: %dx%d\n", nComponents, nTuples);
                assert(U.entry!=NULL);
                if(VisIt_VariableData_alloc(&h) == VISIT_OKAY)
                {
                    VisIt_VariableData_setDataD(h, VISIT_OWNER_SIM, nComponents, nTuples, U.entry);
                }
            }
            
            if( name == "V" )
            {
                const int nComponents=1;
                const int nTuples    =V.items;
                MPI.Printf0( stderr, "Sending V: %dx%d\n", nComponents, nTuples);
                assert(V.entry!=NULL);
                if(VisIt_VariableData_alloc(&h) == VISIT_OKAY)
                {
                    VisIt_VariableData_setDataD(h, VISIT_OWNER_SIM, nComponents, nTuples, V.entry);
                }
            }
            
            return h;
        }
        
        virtual void step()
        {
            MPI.Printf0( stderr, "[Simulation Cycle %6d ]\n", cycle);
            init_exchange();
            wait_exchange();
        }
        
    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(MySim);
    };
    
}

YOCTO_UNIT_TEST_IMPL(s2d)
{
    
    //--------------------------------------------------------------------------
    //info for VisIt
    //--------------------------------------------------------------------------
    const string sim_name    = "Simulation2D";
    const string sim_comment = "Simple Simulation 2D";
    const string sim_path    = ".";
    
    //--------------------------------------------------------------------------
    // initialize MPI
    //--------------------------------------------------------------------------
    mpi &MPI = mpi::init( &argc, &argv );
    const int sim_rank = MPI.CommWorldRank;
    const int sim_size = MPI.CommWorldSize;
    
    const string       trace_name = "trace.dat";
    VisIt:: TraceFile  trace_file(sim_rank,trace_name );
    
    //--------------------------------------------------------------------------
    // parallel VisIt
    //--------------------------------------------------------------------------
    VisIt:: SetupParallel( MPI, sim_name, sim_comment, sim_path, NULL);
    
    //--------------------------------------------------------------------------
    // get the full layout
    //--------------------------------------------------------------------------
    const Layout full_layout( Coord(1,1), Coord(21,21) );
    
    //--------------------------------------------------------------------------
    // create the ghosts
    //--------------------------------------------------------------------------
    GhostsSetup  sim_ghosts;
    if( MPI.CommWorldSize > 1 )
    {
        sim_ghosts.lower.count.y = 2;
        sim_ghosts.lower.peer.y  = MPI.CommWorldPrev();
        sim_ghosts.upper.count.y = 2;
        sim_ghosts.upper.peer.y  = MPI.CommWorldNext();
    }
    else 
    {
        sim_ghosts.local.count.y = 2;
    }
    
    //--------------------------------------------------------------------------
    // create the sim layout
    //--------------------------------------------------------------------------
    const Layout sim_layout = full_layout.split(sim_rank,sim_size);
    
    //--------------------------------------------------------------------------
    // Ready to create the simulation !
    //--------------------------------------------------------------------------
    MySim  sim( MPI, sim_layout, sim_ghosts );
    Region R( Vertex(-1,-1), Vertex(1,1) );
    sim.mesh.regular_map_to(R,full_layout);
    
    //--------------------------------------------------------------------------
    // Initialize the simulation
    //--------------------------------------------------------------------------
    sim.initialize();
    
    //--------------------------------------------------------------------------
    // Initialize ghosts
    //--------------------------------------------------------------------------
    sim.init_exchange();
    sim.wait_exchange();
    
    VisIt:: MainLoop(sim);
    
    
}
YOCTO_UNIT_TEST_DONE()
