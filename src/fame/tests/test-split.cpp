#include "yocto/fame/split/quad1d.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/fame/split/quad2d.hpp"
#include "yocto/fame/split/quad3d.hpp"

#include "yocto/fame/split/build-quad-ghosts.hpp"
#include "yocto/fame/array3d.hpp"

using namespace yocto;
using namespace fame;

#include <cstdlib>

YOCTO_UNIT_TEST_IMPL(split)
{
    quad_links  links[3];
    quad_links &xlinks = links[0];
    linear_handles handles;
    
    {
        const layout1D l1 = layout1D(1,12);
        quad_ghosts<layout1D>::list lg;
        quad_ghosts<layout1D>::list ag;

        for(int size=1;size<=8;++size)
        {
            std::cerr << std::endl;
            std::cerr << "size=" << size << ", full=" << l1 << std::endl;
            for(int rank=0;rank<size;++rank)
            {
                handles.free();
                
                const layout1D s = quad1D::split(l1, rank, size, true,xlinks);
                std::cerr << "\trank   =" << rank << ",\t sub=" << s << ",\t links=" << xlinks << std::endl;
                const layout1D s_out = build_quad_ghosts<layout1D>::outline_for(rank, s, 1, links, lg,ag);
                std::cerr << "\t\toutline=" << s_out << std::endl;
                handles.append( new array1D<float>("A",s_out) );
                handles.append( new array1D<int>("B",s_out) );

                for(quad_ghosts<layout1D> *g = lg.head;g;g=g->next)
                {
                    g->local_update(handles);
                }
                
                for(quad_ghosts<layout1D> *g = ag.head;g;g=g->next)
                {
                    g->allocate_for(handles);
                    g->send_assemble(handles);
                    g->recv_dispatch(handles);
                }
                
            }
        }
    }
    
    
    quad_links &ylinks = links[1];
    
    {
        const layout2D l2 = layout2D( coord2D(1,1), coord2D(10,12) );
        quad_ghosts<layout2D>::list lg;
        quad_ghosts<layout2D>::list ag;

        coord2D    sizes;
        coord2D    ranks;
        for(int size=1;size<=4;++size)
        {
            std::cerr << std::endl;
            std::cerr << "size=" << size << ", full=" << l2 << std::endl;
            for(int rank=0;rank<size;++rank)
            {
                handles.free();
                const layout2D s = quad2D::split(l2, rank, size, true,xlinks, true,ylinks,ranks,sizes);
                std::cerr << "\trank = " << rank << " : " << ranks << " / " << sizes << ", sub=" << s
                << " xlinks=" << xlinks << ", ylinks=" << ylinks
                << std::endl;
                const layout2D s_out = build_quad_ghosts<layout2D>::outline_for(rank, s, 1, links, lg,ag);
                std::cerr << "\t\toutline=" << s_out << std::endl;
                handles.append( new array2D<float>("A",s_out) );
                handles.append( new array2D< math::v2d<double> >("B",s_out) );

                for(quad_ghosts<layout2D> *g = lg.head;g;g=g->next)
                {
                    g->local_update(handles);
                }
                
                for(quad_ghosts<layout2D> *g = ag.head;g;g=g->next)
                {
                    g->allocate_for(handles);
                    g->send_assemble(handles);
                    g->recv_dispatch(handles);
                }

            }
            
        }
    }
    
    quad_links &zlinks = links[2];
    {
        const layout3D l3 = layout3D( coord3D(1,1,1), coord3D(10,12,14) );
        quad_ghosts<layout3D>::list lg;
        quad_ghosts<layout3D>::list ag;
        
        coord3D    sizes;
        coord3D    ranks;
        for(size_t size=1;size<=9;++size)
        {
            handles.free();
            std::cerr << std::endl;
            std::cerr << "size=" << size << ", full=" << l3 << std::endl;
            for(int rank=0;rank<size;++rank)
            {
                const layout3D s = quad3D::split(l3, rank, size, true,xlinks, true,ylinks,true,zlinks,ranks,sizes);
                std::cerr << "\trank " << rank << " : " << ranks << " / " << sizes
                << ", sub=" << s << std::endl
                << "\t\txlinks=" << xlinks << ", ylinks=" << ylinks << ", zlinks=" << zlinks
                << std::endl;
                const layout3D s_out = build_quad_ghosts<layout3D>::outline_for(rank, s, 1, links, lg, ag);
                std::cerr << "\t\toutline=" << s_out << std::endl;
                
                handles.append( new array3D<long>("A",s_out) );
                handles.append( new array3D< math::v3d<float> >("B",s_out) );
                
                for(quad_ghosts<layout3D> *g = lg.head;g;g=g->next)
                {
                    g->local_update(handles);
                }
                
                for(quad_ghosts<layout3D> *g = ag.head;g;g=g->next)
                {
                    g->allocate_for(handles);
                    g->send_assemble(handles);
                    g->recv_dispatch(handles);
                }

            }
        }
        

    }
    
}
YOCTO_UNIT_TEST_DONE()