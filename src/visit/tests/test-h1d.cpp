#include "yocto/fame/visit.hpp"
#include "yocto/fame/mesh/rectilinear.hpp"
#include "yocto/fame/split/mpi-quad-exchange.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/code/ipower.hpp"

#include <cmath>


using namespace yocto;
using namespace fame;

namespace
{
    typedef layout1D                       Layout;
    typedef RectilinearMesh<Layout,double> MeshType;
    
    static const bool the_pbc[1] = { true };
    
    class Heat : public arrays, public MeshType
    {
    public:
        explicit Heat(const Layout &full,
                      const int     sim_rank,
                      const int     sim_size) :
        arrays(),
        MeshType( *this, "mesh",full,the_pbc,sim_rank,sim_size,1),
        curv("T")
        {
            store( new array1D<double>("T",outline) );
            store( new array1D<double>("L",outline) );
        }
        
        virtual ~Heat() throw()
        {
        }
        
        CurvInfo curv; //! for visit
        uniform_generator<double> ran;
        
    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(Heat);
    };
    
    
    class HeatSim : public VisIt::Simulation, public Heat
    {
    public:
        explicit HeatSim( const mpi &ref , const Layout &full) :
        VisIt::Simulation(ref),
        Heat(full,par_rank,par_size),
        X( getX() ),
        T( (*this)["T"].as< array1D<double> >() ),
        L( (*this)["L"].as< array1D<double> >() ),
        handles()
        {
            const double dx = 1.0 / full.width;
            for(unit_t i=outline.lower;i<=outline.upper;++i)
            {
                X[i] = (i-1)*dx;
            }
            
            handles.append(&T);
            
            allocate_async_for(handles.chunk_size);
            
            
            reset();
        }
        
        virtual ~HeatSim() throw()
        {
        }
        
        array1D<double> &X;
        array1D<double> &T;
        array1D<double> &L;
        linear_handles   handles;
        
        void exchange()
        {
            mpi_quad_exchange::data(*this, handles, MPI);
        }
        
        void reset()
        {
            MPI.Printf(stderr, "reset...\n");
            for(unit_t i=lower;i<=upper;++i)
            {
                T[i] = ipower(sin( 2*M_PI*X[i] ),1) + 0.05 * (0.5-ran());
            }
            exchange();
            cycle=0;
            runTime=0;
        }
        
        
        //! sending meta data
        virtual void get_meta_data( visit_handle &md ) const
        {
            //
            add_generic_command("quit",md);

            //
            add_generic_command("reset",md);
            
            // curv
            visit:: add_curv_meta_data(md, curv);
        }
        
        virtual visit_handle get_curve( const string &name ) const
        {
            
            if(name == "T" )
            {
                return visit::get_curv(X,T);
            }
            return VISIT_INVALID_HANDLE;
        }

        virtual void execute(const string cmd, const array<string> &args)
        {
            if( cmd == "reset" )
            {
                reset();
            }
        }
        
        virtual void step()
        {
            runTime = cycle;
            
            // we start from a valid state
            for(unit_t i=lower;i<=upper;++i)
            {
                L[i] = (T[i-1] - 2.0 *T[i] + T[i+1]);
            }
            
            for(unit_t i=lower;i<=upper;++i)
            {
                T[i] += 0.1 * L[i];
            }
            
            exchange();
        
            for(unit_t i=lower;i<=upper;++i)
            {
                const double u =T[i];
                T[i] += 0.04 * (u-u*u*u);
            }
            
            exchange();

            
        }
        
    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(HeatSim);
    };
    
}

YOCTO_UNIT_TEST_IMPL(h1d)
{
    const string sim_name    = "Heat1D";
    const string sim_comment = "Visit+Fame=>Heat 1D";
    const string sim_path    = ".";
    
    YOCTO_MPI(MPI_THREAD_SINGLE);
    
    
    VisIt::TraceFile trace(MPI,"trace.dat");
    VisIt::SetupParallel(MPI,sim_name,sim_comment,sim_path,NULL);
    
    const Layout full(1,1000);
    
    HeatSim sim(MPI,full);
    
    VisIt::Loop(sim);
    
    
}
YOCTO_UNIT_TEST_DONE()
