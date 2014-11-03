#include "yocto/utest/run.hpp"
#include "yocto/fame/vertices.hpp"
#include "yocto/fame/box.hpp"

#include "yocto/fame/edge.hpp"


using namespace yocto;
using namespace fame;
using namespace math;


YOCTO_UNIT_TEST_IMPL(vtx)
{
    
    arrays adb(8);
    adb.store( new array1D<double>("X",layout1D(-5,5)) );
    adb.store( new array1D<double>("Y",layout1D(-6,6)) );
    adb.store( new array1D<double>("Z",layout1D(-10,10)) );
    
    box<3,float> B(math::v3d<float>(1.0f,-2.0f,1.2f), math::v3d<float>(-1.0f,3.0f,-0.7f) );
    B.map_rect<double>(adb);
    
    Vertex<1,double>::List v1;
    v1.Rectilinear( adb["X"].as< array1D<double> >());
    std::cerr << "v1.size=" << v1.size << std::endl;
    {
        typedef Edge<1,double> edge1;
        core::list_of_cpp<edge1> e1;
        for( Vertex<1,double> *node = v1.head; node; node=node->next )
        {
            if(node->next)
            {
                e1.push_back( new edge1(node, node->next) );
            }
        }
    }
    
    
    Vertex<2,double>::List v2;
    v2.Rectilinear( adb["X"].as< array1D<double> >(), adb["Y"].as< array1D<double> >());
    std::cerr << "v2.size=" << v2.size << std::endl;
    {
        typedef Edge<2,double>   edge2;
        core::list_of_cpp<edge2> e2;
        for( Vertex<2,double> *node = v2.head; node; node=node->next )
        {
            if(node->next)
            {
                e2.push_back( new edge2(node, node->next) );
            }
        }
    }
    
    
    Vertex<3,double>::List v3;
    v3.Rectilinear(adb["X"].as< array1D<double> >(),
                   adb["Y"].as< array1D<double> >(),
                   adb["Z"].as< array1D<double> >());
    
    std::cerr << "v3.size=" << v3.size << std::endl;
    {
        typedef Edge<3,double>   edge3;
        core::list_of_cpp<edge3> e3;
        for( Vertex<3,double> *node = v3.head; node; node=node->next )
        {
            if(node->next)
            {
                e3.push_back( new edge3(node, node->next ) );
            }
        }
    }

    
}
YOCTO_UNIT_TEST_DONE()
