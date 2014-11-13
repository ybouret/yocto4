#include "yocto/fame/mesh/rectilinear.hpp"
#include "yocto/fame/split/mpi-quad-exchange.hpp"
#include "yocto/fame/array3d.hpp"

#include "yocto/ios/ocstream.hpp"


using namespace yocto;
using namespace fame;


int main(int argc, char *argv[])
{
    
    bool pbc[3] = { true, true, true };
    for(int i=1;i<argc;++i)
    {
        if( strcmp(argv[i],"-x") == 0 )
        {
            pbc[0] = false;
            continue;
        }
        
        if( strcmp(argv[i],"-y") == 0 )
        {
            pbc[1] = false;
            continue;
        }
        
        if( strcmp(argv[i],"-z") == 0 )
        {
            pbc[2] = false;
            continue;
        }
        
        
    }
    
    try
    {
        YOCTO_MPI_ENV();
        arrays adb;
        const int    rank = MPI.CommWorldRank;
        const int    size = MPI.CommWorldSize;
        const string pfx  = vformat("%d.%d", size, rank);
        
        //______________________________________________________________________
        //
        // in 1 D
        //______________________________________________________________________
        {
            
            const layout1D full(1,16);
            RectilinearMesh<layout1D,double> mesh(adb,"m1",full,pbc,rank,size,1);
            for(unit_t i=mesh.outline.lower;i<=mesh.outline.upper;++i)
            {
                mesh.getX()[i] = i;
            }
            MPI.Printf(stderr, "layout:  %d->%d\n", (int)mesh.lower, (int)mesh.upper);
            MPI.Newline(stderr);
            MPI.Printf(stderr, "outline: %d->%d\n", (int)mesh.outline.lower, (int)mesh.outline.upper);
            MPI.Newline(stderr);
            MPI.Printf(stderr, "#local ghost: %2d | #async ghost: %2d\n", (int) mesh.local_ghosts.size, (int) mesh.async_ghosts.size );
            MPI.Printf(stderr, "#local async x ghosts: %d\n", int(mesh.asynchronous[0].count));
            adb.store( new array1D<double>("C", mesh.outline) );
            adb.store( new array1D<float>("D",mesh.outline)   );
            adb.store( new array1D<double>("G", mesh.outline) );

            array1D<double> &C = adb["C"].as< array1D<double> >();
            array1D<float>  &D = adb["D"].as< array1D<float>  >();
            //array1D<double> &G = adb["G"].as< array1D<double> >();

            
            for(unit_t i=mesh.lower;i<=mesh.upper;++i)
            {
                C[i] = 1+i;
                D[i] = 1+i;
            }
            
            
            // fill ghosts
            {
                for( quad_ghosts<layout1D> *g = mesh.local_ghosts.head;g;g=g->next)
                {
                    //C.fill( -rank, g->source.zone );
                    C.fill( -rank, g->target.zone );
                    
                    //D.fill(  -rank, g->source.zone );
                    D.fill(  -rank, g->target.zone );
                    
                }
                
                for( quad_ghosts<layout1D> *g = mesh.async_ghosts.head;g;g=g->next)
                {
                    //C.fill( -rank, g->source.zone );
                    C.fill( -rank, g->target.zone );
                    
                    //D.fill(  -rank, g->source.zone );
                    D.fill(  -rank, g->target.zone );
                }
            }
            
            
            {
                ios::ocstream fp( "ini" + pfx + ".dat", false);
                for(unit_t i=mesh.outline.lower;i<=mesh.outline.upper;++i)
                {
                    fp("%g %g %g\n", mesh.getX()[i], C[i], D[i]);
                }
            }
            
            linear_handles handles;
            handles.append( &C );
            handles.append( &D );
            
            mesh.allocate_async_for(handles.chunk_size);
            
            // data exchange
            MPI.Printf0(stderr, "Exchanging 1D...\n");
            
            
            {
                mpi_quad_exchange::data(mesh, handles, MPI);
            }
            
            //mesh.gradient(G,C);
            
            {
                ios::ocstream fp( "end" + pfx + ".dat", false);
                for(unit_t i=mesh.outline.lower;i<=mesh.outline.upper;++i)
                {
                    fp("%g %g %g\n", mesh.getX()[i], C[i], D[i]);
                }
            }
            
        }
        
        MPI.Newline(stderr);
        
        adb.free();
        
        { // 2D
            
            const layout2D full( coord2D(1,1), coord2D(20,20) );
            
            RectilinearMesh<layout2D,double> mesh(adb,"m2",full,pbc,rank,size,1);
            
            for(unit_t i=mesh.outline.lower.x;i<=mesh.outline.upper.x;++i)
            {
                mesh.getX()[i] = i;
            }
            
            for(unit_t j=mesh.outline.lower.y;j<=mesh.outline.upper.y;++j)
            {
                mesh.getY()[j] = j;
            }
            
            MPI.Printf(stderr, "layout : %2d,%2d --> %2d,%2d\n", (int)mesh.lower.x,(int)mesh.lower.y, (int)mesh.upper.x, (int)mesh.upper.y);
            MPI.Printf(stderr, "outline: %2d,%2d --> %2d,%2d\n", (int)mesh.outline.lower.x,(int)mesh.outline.lower.y, (int)mesh.outline.upper.x, (int)mesh.outline.upper.y);
            
            MPI.Printf(stderr, "#local ghost: %2d | #async ghost: %2d\n", (int) mesh.local_ghosts.size, (int) mesh.async_ghosts.size );
            
            
            adb.store( new array2D<double>("C", mesh.outline) );
            adb.store( new array2D< math::v2d<float> >("D",mesh.outline)   );
            adb.store( new array2D<double>("G", mesh.outline) );

            array2D<double>              &C = adb["C"].as< array2D<double> >();
            array2D< math::v2d<float> >  &D = adb["D"].as<   array2D< math::v2d<float> > >();
            //array2D<double>              &G = adb["G"].as< array2D<double> >();

            for(unit_t j=mesh.lower.y;j<=mesh.upper.y;++j)
            {
                for(unit_t i=mesh.lower.x;i<=mesh.upper.x;++i)
                {
                    C[j][i]   = 1+i+j;
                    D[j][i].x = 1+i;
                    D[j][i].y = 1+j;
                }
            }
            
            
            linear_handles handles;
            handles.append( &C );
            handles.append( &D );
            
            mesh.allocate_async_for(handles.chunk_size);
            
            
            MPI.Printf0(stderr, "Exchanging 2D...\n");
            mpi_quad_exchange::data(mesh, handles, MPI);
            
            //mesh.gradient(G, C);
            
        }
        
        MPI.Newline(stderr);
        
        adb.free();
        
        { // 3D
            
            const layout3D full( coord3D(1,1,1), coord3D(30,30,30) );
            
            RectilinearMesh<layout3D,double> mesh(adb,"m3",full,pbc,rank,size,1);
            MPI.Printf0(stderr, "sizes: %d,%d,%d\n", (int)mesh.sizes.x, (int)mesh.sizes.y, (int)mesh.sizes.z);
            
            for(unit_t i=mesh.outline.lower.x;i<=mesh.outline.upper.x;++i)
            {
                mesh.getX()[i] = i;
            }
            
            for(unit_t j=mesh.outline.lower.y;j<=mesh.outline.upper.y;++j)
            {
                mesh.getY()[j] = j;
            }
            
            for(unit_t k=mesh.outline.lower.z;k<=mesh.outline.upper.z;++k)
            {
                mesh.getZ()[k] = k;
            }
            
            
            MPI.Printf(stderr, "layout : %2d,%2d,%2d --> %2d,%2d,%2d\n", (int)mesh.lower.x,(int)mesh.lower.y, (int)mesh.lower.z, (int)mesh.upper.x, (int)mesh.upper.y,(int)mesh.upper.z);
            MPI.Printf(stderr, "outline: %2d,%2d,%2d --> %2d,%2d,%2d\n", (int)mesh.outline.lower.x,(int)mesh.outline.lower.y, (int)mesh.outline.lower.z, (int)mesh.outline.upper.x, (int)mesh.outline.upper.y, (int)mesh.outline.upper.z);
            
            MPI.Printf(stderr, "#local ghost: %2d | #async ghost: %2d\n", (int) mesh.local_ghosts.size, (int) mesh.async_ghosts.size );
            
            
            adb.store( new array3D<double>("C", mesh.outline) );
            adb.store( new array3D< math::v3d<float> >("D",mesh.outline)   );
            adb.store( new array3D<double>("G", mesh.outline) );

            array3D<double>              &C = adb["C"].as< array3D<double> >();
            array3D< math::v3d<float> >  &D = adb["D"].as<   array3D< math::v3d<float> > >();
            //array3D<double>              &G = adb["G"].as< array3D<double> >();

            for(unit_t k=mesh.lower.z;k<=mesh.upper.z;++k)
            {
                for(unit_t j=mesh.lower.y;j<=mesh.upper.y;++j)
                {
                    for(unit_t i=mesh.lower.x;i<=mesh.upper.x;++i)
                    {
                        C[k][j][i]   = 1+i+j+k;
                        D[k][j][i].x = 1+i;
                        D[k][j][i].y = 1+j;
                        D[k][j][i].y = 1+k;
                    }
                }
                
            }
            
            linear_handles handles;
            handles.append( &C );
            handles.append( &D );
            
            mesh.allocate_async_for(handles.chunk_size);
            
            
            MPI.Printf0(stderr, "Exchanging 3D...\n");
            mpi_quad_exchange::data(mesh, handles, MPI);
            
            //mesh.gradient(G,C);
            
        }
        
        
        return 0;
    }
    catch(...)
    {
        std::cerr << "Uncaught Exception!" << std::endl;
    }
    return 1;
}