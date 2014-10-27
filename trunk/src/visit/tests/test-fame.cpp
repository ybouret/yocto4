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
    
    class MyData : public layout3D, public arrays
    {
    public:
        const layout2D l2;
        
        MyData() : layout3D( coord3D(-5,-6,-7), coord3D(5,6,7 ) ),
        l2( coord2D(lower.x,lower.y), coord2D(upper.x,upper.y) )
        {
            this->store( new array1D<double>("X",layout1D(lower.x,upper.x)) );
            this->store( new array1D<double>("Y",layout1D(lower.y,upper.y)) );
            this->store( new array1D<double>("Z",layout1D(lower.z,upper.z)) );
            this->store( new ScalarField2D("A2",l2) );
            this->store( new VectorField2D("V2",l2) );
            this->store( new ScalarField3D("A3",*this) );
            this->store( new VectorField3D("V3",*this) );

        }
        
        
        
        virtual ~MyData() throw() {}
        
    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(MyData);
        
    };
    
    
    
    class MySim : public MyData, public VisIt::Simulation
    {
    public:
        
        explicit MySim( const mpi &ref) :
        MyData(),
        VisIt::Simulation( ref ),
        m2(*this,"mesh2d"),
        m3(*this,"mesh3d"),
        A2( (*this)["A2"].as<ScalarField2D>() ),
        V2( (*this)["V2"].as<VectorField2D>() ),
        A3( (*this)["A3"].as<ScalarField3D>() ),
        V3( (*this)["V3"].as<VectorField3D>() ),
        curv("the_curve"),
        cx("cx",layout1D(lower.x,upper.x)),
        cy("cy",cx)
        {
            assert(cx.items==cy.items);
            
            box<3,float> B(math::v3d<float>(1.0f,-2.0f,1.2f), math::v3d<float>(-1.0f,3.0f,-0.7f) );
            B.map<double>(*this);
            
            for(unit_t j=A2.lower.y;j<=A2.upper.y;++j)
            {
                const double y = m2.Y()[j];
                for(unit_t i=A2.lower.x;i<=A2.upper.x;++i)
                {
                    const double x = m2.X()[i];
                    const double mg =exp(-(x*x+y*y)/2 );
                    A2[j][i]   = mg;
                    V2[j][i].x = mg;
                    V2[j][i].y = sin(3*(y+x));
                }
            }
            
            for(unit_t k=A3.lower.z;k<=A3.upper.z;++k)
            {
                const double z = m3.Z()[k];
                for(unit_t j=A3.lower.y;j<=A3.upper.y;++j)
                {
                    const double y = m3.Y()[j];
                    for(unit_t i=A3.lower.x;i<=A3.upper.x;++i)
                    {
                        const double x  = m3.X()[i];
                        const double mg = exp(-(x*x+y*y+z*z)/2 );
                        A3[k][j][i]     = mg;
                        v3d<double> &v = V3[k][j][i];
                        v.x = mg;
                        v.y = mg/2;
                        v.z = mg/3;
                    }
                }
            }
            
            
        }
        
        virtual ~MySim() throw() {}
        
        //! sending meta data
        virtual void get_meta_data( visit_handle &md ) const
        {
            //
            add_generic_command("quit",md);
            
            // meshes
            visit::add_mesh_meta_data(md, m2);
            
            visit::add_mesh_meta_data(md, m3);
            
            // data
            visit::add_variable_meta_data(md, A2, m2.name);
            
            visit::add_variable_meta_data(md, V2, m2.name);
            
            visit::add_variable_meta_data(md, A3, m3.name);
         
            visit::add_variable_meta_data(md, V3, m3.name);

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
            
            if( name == "V2" )
            {
                return visit::get_variable_data(ndomain, V2);
            }
            
            if( name == "A3" )
            {
                return visit::get_variable_data(ndomain, A3);
            }
            
            if( name == "V3" )
            {
                return visit::get_variable_data(ndomain, V3);
            }
            
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
                cx[i] = static_cast<float>(m2.X()[i]);
                cy[i] = sin( cx[i] + 0.1 * runTime );
            }
        }
        
        
        RectilinearMesh<2,double> m2;
        RectilinearMesh<3,double> m3;
        ScalarField2D            &A2;
        VectorField2D            &V2;
        ScalarField3D            &A3;
        VectorField3D            &V3;
        CurvInfo                  curv;
        array1D<float>            cx;
        array1D<float>            cy;
        
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
    
    MySim sim(MPI);
    sim.step();
    
    VisIt::Loop(sim);
    
}
YOCTO_UNIT_TEST_DONE()
