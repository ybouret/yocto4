#include "yocto/fame/visit.hpp"
#include "yocto/fame/mesh/rectilinear.hpp"
#include "yocto/fame/split/mpi-quad-exchange.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/code/ipower.hpp"
#include "yocto/fame/array3d.hpp"

#include <cmath>


using namespace yocto;
using namespace fame;

namespace
{
    typedef layout3D                       Layout;
    typedef coord3D                        Coord;
    typedef RectilinearMesh<Layout,double> MeshType;
    
    static const bool the_pbc[3] = { true, true, true };
    
    class Heat : public arrays, public MeshType
    {
    public:
        explicit Heat(const Layout &full,
                      const int     sim_rank,
                      const int     sim_size) :
        arrays(),
        MeshType( *this, "mesh",full,the_pbc,sim_rank,sim_size,1)
        {
            std::cerr << "Allocating Arrays with " << outline.items << " items" << std::endl;
            store( new array3D<double>("T",outline) );
            store( new array3D<double>("L",outline) );
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
        Z( getZ() ),
        T( (*this)["T"].as< array3D<double> >() ),
        L( (*this)["L"].as< array3D<double> >() ),
        dx( 1.0 / full.width.x),
        dy( 1.0 / full.width.y),
        dz( 1.0 / full.width.z),
        handles()
        {
            MPI.Printf(stderr,"initiliazing...\n");
            for(unit_t i=outline.lower.x;i<=outline.upper.x;++i)
            {
                X[i] = (i-1)*dx;
            }
            
            for(unit_t j=outline.lower.y;j<=outline.upper.y;++j)
            {
                Y[j] = (j-1)*dy;
            }
            
            for(unit_t k=outline.lower.z;k<=outline.upper.z;++k)
            {
                Z[k] = (k-1)*dz;
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
        array1D<double> &Z;
        array3D<double> &T;
        array3D<double> &L;
        const double     dx;
        const double     dy;
        const double     dz;
        linear_handles   handles;
        
        void exchange()
        {
            mpi_quad_exchange::data(*this, handles, MPI);
        }
        
        void reset()
        {
            MPI.Printf(stderr, "reset...\n");
            for(unit_t k=lower.z;k<=upper.z;++k)
            {
                const double dz = Z[k] - 0.5;
                for(unit_t j=lower.y;j<=upper.y;++j)
                {
                    const double dy = Y[j] - 0.5;
                    for(unit_t i=lower.x;i<=upper.x;++i)
                    {
                        const double dx = X[i] - 0.5;
                        T[k][j][i] = exp(- (dx*dx+dy*dy+dz*dz)/3 ) + 0.05 * (0.5-ran());
                    }
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
            const double rhoy = ipower(dx/dy,2);
            const double rhoz = ipower(dx/dz,2);
            // we start from a valid state
            for(unit_t k=lower.z;k<=upper.z;++k)
            {
                for(unit_t j=lower.y;j<=upper.y;++j)
                {
                    for(unit_t i=lower.x;i<=upper.x;++i)
                    {
                        L[k][j][i] = (T[k][j][i-1] - 2.0 *T[k][j][i] + T[k][j][i+1])
                        + rhoy * ( T[k][j-1][i] - 2.0 * T[k][j][i] + T[k][j+1][i])
                        + rhoz * ( T[k-1][j][i] - 2.0 * T[k][j][i] + T[k+1][j][i]);
                    }
                }
            }
            
            for(unit_t k=lower.z;k<=upper.z;++k)
            {
                for(unit_t j=lower.y;j<=upper.y;++j)
                {
                    for(unit_t i=lower.x;i<=upper.x;++i)
                    {
                        T[k][j][i] += 0.02 * L[k][j][i];
                    }
                }
            }
            
            exchange();
            
            if(false)
            {
                for(unit_t k=lower.z;k<=upper.z;++k)
                {
                    for(unit_t j=lower.y;j<=upper.y;++j)
                    {
                        for(unit_t i=lower.x;i<=upper.x;++i)
                        {
                            const double u =T[k][j][i];
                            T[k][j][i] += 0.04 * (u-u*u*u);
                        }
                    }
                }
                exchange();
            }
        }
        
    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(HeatSim);
    };
    
}

YOCTO_UNIT_TEST_IMPL(h3d)
{
    const string sim_name    = "Heat3D";
    const string sim_comment = "Visit+Fame=>Heat 3D";
    const string sim_path    = ".";
    
    YOCTO_MPI(MPI_THREAD_SINGLE);
    
    int Nx=10;
    int Ny=12;
    int Nz=14;
    
    VisIt::TraceFile trace(MPI,"trace.dat");
    VisIt::SetupParallel(MPI,sim_name,sim_comment,sim_path,NULL);
    
    const Layout full( Coord(1,1,1), Coord(Nx,Ny,Nz) );
    
    
    HeatSim sim(MPI,full);
    MPI.Printf0(stderr, "sizes= %d x %d x %d\n", (int)sim.sizes.x,(int)sim.sizes.y, (int)sim.sizes.z);
    
    
    VisIt::Loop(sim);
    
    
}
YOCTO_UNIT_TEST_DONE()
