#include "yocto/fame/visit.hpp"
#include "yocto/fame/mesh/rectilinear.hpp"
#include "yocto/fame/split/mpi-quad-exchange.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/code/ipower.hpp"
#include "yocto/fame/array2d.hpp"

#include <cmath>


using namespace yocto;
using namespace fame;

namespace
{
    typedef layout2D                       Layout;
    typedef coord2D                        Coord;
    typedef RectilinearMesh<Layout,double> MeshType;
    
    static const bool the_pbc[2] = { true, true };
    
    class Heat : public arrays, public MeshType
    {
    public:
        explicit Heat(const Layout &full,
                      const int     sim_rank,
                      const int     sim_size) :
        arrays(),
        MeshType( *this, "mesh",full,the_pbc,sim_rank,sim_size,1),
	ran()
        {
            store( new array2D<double>("T",outline) );
            store( new array2D<double>("L",outline) );
        }
        
        virtual ~Heat() throw()
        {
        }
        
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
        Y( getY() ),
        T( (*this)["T"].as< array2D<double> >() ),
        L( (*this)["L"].as< array2D<double> >() ),
        dx( 1.0 / full.width.x),
        dy( 1.0 / full.width.y),
        handles()
        {
            for(unit_t i=outline.lower.x;i<=outline.upper.x;++i)
            {
                X[i] = (i-1)*dx;
            }
            
            for(unit_t j=outline.lower.y;j<=outline.upper.y;++j)
            {
                Y[j] = (j-1)*dy;
            }
            
            handles.append(&T);
            
            allocate_async_for(handles.chunk_size);
            
            
            reset();
        }
        
        virtual ~HeatSim() throw()
        {
        }
        
        array1D<double> &X;
        array1D<double> &Y;
        array2D<double> &T;
        array2D<double> &L;
        const double     dx;
        const double     dy;
        linear_handles   handles;
        
        void exchange()
        {
            mpi_quad_exchange::data(*this, handles, MPI);
        }
        
        void reset()
        {
            MPI.Printf(stderr, "reset...\n");
            for(unit_t j=lower.y;j<=upper.y;++j)
            {
                for(unit_t i=lower.x;i<=upper.x;++i)
                {
                    T[j][i] = sin( 2*M_PI*X[i] ) * sin( 2*M_PI*Y[j]) + 0.05 * (0.5-ran());
                }
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
            
            visit::add_mesh_meta_data(md, *this, par_size);
            
            visit::add_variable_meta_data(md, T, "mesh");
            
        }
        
        virtual visit_handle get_mesh( int domain, const string &name) const
        {
            
            if(name=="mesh")
            {
                return visit::get_mesh(*this,domain);
            }
            
            
            return VISIT_INVALID_HANDLE;
        }
        
        virtual visit_handle get_variable( int domain, const string &name ) const
        {
            if( name == "T" )
            {
                return visit::get_variable_data(T,domain);
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
            const double lrho = ipower(dx/dy,2);
            // we start from a valid state
            for(unit_t j=lower.y;j<=upper.y;++j)
            {
                for(unit_t i=lower.x;i<=upper.x;++i)
                {
                    L[j][i] = (T[j][i-1] - 2.0 *T[j][i] + T[j][i+1])
                    + lrho * ( T[j-1][i] - 2.0 * T[j][i] + T[j+1][i]);
                }
            }
            
            
            for(unit_t j=lower.y;j<=upper.y;++j)
            {
                for(unit_t i=lower.x;i<=upper.x;++i)
                {
                    T[j][i] += 0.02 * L[j][i];
                }
            }
            
            exchange();
            
            
            for(unit_t j=lower.y;j<=upper.y;++j)
            {
                for(unit_t i=lower.x;i<=upper.x;++i)
                {
                    const double u =T[j][i];
                    T[j][i] += 0.04 * (u-u*u*u);
                }
            }
            
            exchange();
            
        }
        
    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(HeatSim);
    };
    
}

YOCTO_UNIT_TEST_IMPL(h2d)
{
    const string sim_name    = "Heat2D";
    const string sim_comment = "Visit+Fame=>Heat 2D";
    const string sim_path    = ".";
    
    YOCTO_MPI(MPI_THREAD_SINGLE);
    
    
    VisIt::TraceFile trace(MPI,"trace.dat");
    VisIt::SetupParallel(MPI,sim_name,sim_comment,sim_path,NULL);
    
    const Layout full( Coord(1,1), Coord(100,110) );
    
    HeatSim sim(MPI,full);
    //MPI.Printf(stderr, "sizes= %d x %d\n", sim.sizes.x, sim.sizes.y);
    
    //return 0;
    
    VisIt::Loop(sim);
    
    
}
YOCTO_UNIT_TEST_DONE()
