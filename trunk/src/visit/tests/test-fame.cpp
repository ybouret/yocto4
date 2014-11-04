#include "yocto/fame/visit.hpp"
#include "yocto/utest/run.hpp"

#include "yocto/fame/mesh/rectilinear.hpp"
#include "yocto/fame/box.hpp"
#include <cmath>

using namespace yocto;
using namespace fame;
using namespace math;

namespace
{
    typedef array2D<double>       ScalarField2D;
    typedef array2D<v2d<double> > VectorField2D;
    
    typedef array3D<double>       ScalarField3D;
    typedef array3D<v3d<double> > VectorField3D;
    
    
    template <typename ARR>
    static inline void display1D( const ARR &arr )
    {
        std::cerr << "[";
        for(unit_t i=arr.lower;i<=arr.upper;++i)
        {
            std::cerr << " " << arr[i];
        }
        std::cerr << " ]" << std::endl;
    }
    
    class MyDataDB
    {
    public:
        explicit MyDataDB() : db2(), db3(), pbc()
        {
            pbc[0] = pbc[1] = pbc[2] = true;
        }
        
        arrays db2, db3;
        bool   pbc[3];
        
        virtual ~MyDataDB() throw() {}
        
    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(MyDataDB);
    };
    
    class MyData : public MyDataDB
    {
    public:
        RectilinearMesh<layout2D,float>  m2;
        RectilinearMesh<layout3D,double> m3;
        
        explicit MyData(const layout2D &full2,
                        const layout3D &full3,
                        const int rank,
                        const int size
                        ):
        m2( db2, "mesh2d", full2, pbc, rank, size, 1),
        m3( db3, "mesh3d", full3, pbc, rank, size, 1)
        {
            std::cerr << "sim rank=" << rank << std::endl;
            db2.store( new ScalarField2D("A2",m2.outline) );
            db3.store( new ScalarField3D("A3",m3.outline) );
        }
        
        
        
    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(MyData);
    };
    
    
    
    class MySim : public VisIt::Simulation, public MyData
    {
    public:
        
        
        explicit MySim(const mpi &ref,
                       const layout2D &full2d,
                       const layout3D &full3d ) :
        VisIt::Simulation(ref),
        MyData(full2d,full3d,par_rank,par_size),
        A2( db2["A2"].as<ScalarField2D>() ),
        A3( db3["A3"].as<ScalarField3D>() ),
        curv("the_curve"),
        cx("cx",layout1D(1,10)),
        cy("cy",cx)
        {
            assert(cx.items==cy.items);
            
            for(unit_t k=m3.outline.lower.z;k<=m3.outline.upper.z;++k)
            {
                m3.Z()[k] = k;
            }
            
            for(unit_t j=m3.outline.lower.y;j<=m3.outline.upper.y;++j)
            {
                m3.Y()[j] = j;
            }
            
            for(unit_t i=m3.outline.lower.x;i<=m3.outline.upper.x;++i)
            {
                m3.X()[i] = i;
            }
            
            
            for(unit_t j=m2.outline.lower.y;j<=m2.outline.upper.y;++j)
            {
                m2.Y()[j] = j;
            }
            
            for(unit_t i=m2.outline.lower.x;i<=m2.outline.upper.x;++i)
            {
                m2.X()[i] = i;
            }
            
            std::cerr << "m2.X="; display1D(m2.X());
            std::cerr << "m2.Y="; display1D(m2.Y());
            
            MPI.Printf(stderr, "Loading rank=%d\n", par_rank);
            A2.ld(par_rank);
            A3.ld(par_rank);
            
            
            
        }
        
        virtual ~MySim() throw() {}
        
        //! sending meta data
        virtual void get_meta_data( visit_handle &md ) const
        {
            //
            add_generic_command("quit",md);
            
            // meshes
            visit::add_mesh_meta_data(md, m2, par_size);
            
            visit::add_mesh_meta_data(md, m3, par_size);
            
            // data
            visit::add_variable_meta_data(md, A2, m2.name);
            
            //data
            visit::add_variable_meta_data(md, A3, m3.name);

#if 0
            // data
            visit::add_variable_meta_data(md, V2, m2.name);
            
            
            visit::add_variable_meta_data(md, V3, m3.name);
#endif
            
            // curv
            visit:: add_curv_meta_data(md, curv);
            
            
        }
        
        
        virtual visit_handle get_mesh( int domain, const string &name) const
        {
            
            if(name=="mesh2d")
            {
                return visit::get_mesh(m2);
            }
            
            if(name=="mesh3d")
            {
                return visit::get_mesh(m3);
            }
            
            return VISIT_INVALID_HANDLE;
        }
        
        virtual visit_handle get_variable( int ndomain, const string &name ) const
        {
            if( name == "A2" )
            {
                return visit::get_variable_data(ndomain, A2);
            }
            
            if( name == "A3" )
            {
                return visit::get_variable_data(ndomain, A3);
            }
#if 0
           
            
            if( name == "V2" )
            {
                return visit::get_variable_data(ndomain, V2);
            }
            
           
            
            if( name == "V3" )
            {
                return visit::get_variable_data(ndomain, V3);
            }
#endif
            
            return VISIT_INVALID_HANDLE;
        }
        
        virtual visit_handle get_curve( const string &name ) const
        {
            
            if(name == "the_curve" )
            {
                return visit::get_curv(cx, cy);
            }
            return VISIT_INVALID_HANDLE;
        }
        
        virtual void step()
        {
            runTime = cycle;
            for(unit_t i=cx.lower;i<=cx.upper;++i)
            {
                cx[i] = static_cast<float>(i);
                cy[i] = sin( cx[i] + 0.1 * runTime );
            }
        }
        
        
        ScalarField2D               &A2;
        ScalarField3D               &A3;
        CurvInfo                     curv;
        array1D<float>               cx;
        array1D<float>               cy;
        
    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(MySim);
    };
    
    
}


YOCTO_UNIT_TEST_IMPL(fame)
{
    const string sim_name    = "VisitAndFame";
    const string sim_comment = "Visit I/O";
    const string sim_path    = ".";
    
    YOCTO_MPI(MPI_THREAD_SINGLE);
    
    
    VisIt::TraceFile trace(MPI,"trace.dat");
    VisIt::SetupParallel(MPI,sim_name,sim_comment,sim_path,NULL);
    
    const layout3D full3( coord3D(1,1,1), coord3D(10,20,30) );
    const layout2D full2( coord2D(full3.lower.x,full3.lower.y), coord2D(full3.upper.x,full3.upper.y) );
    
    MySim sim(MPI,full2,full3);
    sim.step();
    
    VisIt::Loop(sim);
    
}
YOCTO_UNIT_TEST_DONE()
