#include "yocto/fovea/vtk.hpp"
#include "yocto/fovea/shape.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace fovea
    {
        
        VTK:: ~VTK() throw()
        {
        }
        
        //______________________________________________________________________
        //
        // callbacks
        //______________________________________________________________________
        
        namespace
        {
            static inline
            void write_float( ios::ostream &fp, const VTK &vtk, const void *addr )
            {
                assert(addr);
                const double x = *(float *)addr;
                fp( vtk.real_fmt.c_str(), x );
            }
            
            static inline
            void write_double( ios::ostream &fp, const VTK &vtk, const void *addr )
            {
                const double x = *(double *)addr;
                fp( vtk.real_fmt.c_str(), x );
            }
            
            static inline
            void write_char( ios::ostream &fp, const VTK &vtk, const void *addr)
            {
                const int x = *(char *)addr;
                fp( vtk.unit_fmt.c_str(),x);
            }
            
            static inline
            void write_short( ios::ostream &fp, const VTK &vtk, const void *addr)
            {
                const int x = *(short *)addr;
                fp( vtk.unit_fmt.c_str(),x);
            }
            
            static inline
            void write_int( ios::ostream &fp, const VTK &vtk, const void *addr)
            {
                const int x = *(int *)addr;
                fp( vtk.unit_fmt.c_str(),x);
            }
            
            static inline
            void write_long( ios::ostream &fp, const VTK &vtk, const void *addr)
            {
                const int x = *(long *)addr;
                fp( vtk.unit_fmt.c_str(),x);
            }
            
            static inline
            void write_v2d_f( ios::ostream &fp, const VTK &vtk, const void *addr)
            {
                const math::v2d<float> &v = *(math::v2d<float> *)addr;
                write_float(fp,vtk,&v.x);
                fp.write(' ');
                write_float(fp,vtk,&v.y);
            }
            
            static inline
            void write_v3d_f( ios::ostream &fp, const VTK &vtk, const void *addr)
            {
                const math::v3d<float> &v = *(math::v3d<float> *)addr;
                write_float(fp,vtk,&v.x);
                fp.write(' ');
                write_float(fp,vtk,&v.y);
                fp.write(' ');
                write_float(fp,vtk,&v.z);
            }
            
            
            static inline
            void write_v2d_d( ios::ostream &fp, const VTK &vtk, const void *addr)
            {
                const math::v2d<double> &v = *(math::v2d<double> *)addr;
                write_double(fp,vtk,&v.x);
                fp.write(' ');
                write_double(fp,vtk,&v.y);
            }
            
            static inline
            void write_v3d_d( ios::ostream &fp, const VTK &vtk, const void *addr)
            {
                const math::v3d<double> &v = *(math::v3d<double> *)addr;
                write_double(fp,vtk,&v.x);
                fp.write(' ');
                write_double(fp,vtk,&v.y);
                fp.write(' ');
                write_double(fp,vtk,&v.z);
            }
            
            
        }
        
        //______________________________________________________________________
        //
        // intialize
        //______________________________________________________________________
        VTK:: VTK() :
        real_fmt("%g"),
        unit_fmt("%d"),
        db(16,as_capacity)
        {
            define<float> (write_float);
            define<double>(write_double);
            define<char>  (write_char);
            define<short> (write_short);
            define<int>   (write_int);
            if( typeid(int) != typeid(long) )
                define<long>(write_long);
            define< math::v2d<float> >(write_v2d_f);
            define< math::v3d<float> >(write_v3d_f);
            define< math::v2d<double> >(write_v2d_d);
            define< math::v3d<double> >(write_v3d_d);
        }
        
        void VTK:: define( const std::type_info &tid, callback cb)
        {
            assert(cb);
            const type_spec sp( tid );
            if( db.search(sp) )
                throw exception("vtk.db(insert multiple type '%s')", sp.name() );
            if( !db.insert(sp,cb) )
                throw exception("vtk.db(unexpected insert failure for type '%s')", sp.name());
        }
        
        
        const char VTK::name[] = "vtk";
        
        //______________________________________________________________________
        //
        // write one registered type
        //______________________________________________________________________
        void VTK::output( ios::ostream &fp, const std::type_info &tid, const void *addr) const
        {
            assert(addr);
            const type_spec sp(tid);
            const callback *pCB = db.search(sp);
            if(!pCB)
                throw exception("vtk::output(undefined type '%s')", sp.name() );
            (*pCB)(fp,*this,addr);
        }
        
        //______________________________________________________________________
        //
        // vtk header
        //______________________________________________________________________
        void VTK::write_header( ios::ostream &fp, const string &title ) const
        {
            fp << "# vtk DataFile Version 2.0\n";
            // TODO: check
            fp << title << "\n";
            fp << "ASCII\n";
        }
        
        //______________________________________________________________________
        //
        // write a the mesh vertices
        //______________________________________________________________________
        void VTK:: write_mesh_vertices(ios::ostream &fp, const yocto::fovea::mesh &m) const
        {
            fp << "DATASET UNSTRUCTURED_GRID\n";
            fp("POINTS %u %s\n", unsigned(m.vertices), m.real_name());
            callback cb = m.real == m.use_float ? write_float : write_double;
            switch(m.dims)
            {
                case 1:
                    for(size_t i=0;i<m.vertices;++i)
                    {
                        const void *v = m.get_vertex_address(i);
                        cb(fp,*this,v);
                        fp << " 0 0\n";
                    }
                    break;
                    
                case 2:
                    for(size_t i=0;i<m.vertices;++i)
                    {
                        const uint8_t *v = (uint8_t*)(m.get_vertex_address(i));
                        cb(fp,*this,v);
                        fp << ' ';
                        v += m.rsz;
                        cb(fp,*this,v);
                        fp << " 0\n";
                    }
                    break;
                    
                case 3:
                    for(size_t i=0;i<m.vertices;++i)
                    {
                        const uint8_t *v = (uint8_t*)(m.get_vertex_address(i));
                        cb(fp,*this,v);
                        fp << ' ';
                        v += m.rsz;
                        cb(fp,*this,v);
                        fp << ' ';
                        v += m.rsz;
                        fp << "\n";
                    }
                    break;
            }
            fp << "\n";
        }
        
        //______________________________________________________________________
        //
        // write a the mesh cells
        //______________________________________________________________________
        void VTK:: write_mesh_cells( ios::ostream &fp, const mesh &m ) const
        {
            static int  cell_types[3] = {
                3, // VTK_LINE
                5, // VTK_TRIANGLE
                10 // VTK_TETRA
            };
            
            const size_t vtx_per_cell   = 1 + m.dims;
            const size_t num_cells      = m.num_cells();
            const size_t cells_size      = num_cells * ( 1+vtx_per_cell);
            fp("CELLS %u %u\n", unsigned(num_cells), unsigned(cells_size) );
            for(const ShapeBase *cell = m.get_first_cell(); cell; cell = cell->get_next() )
            {
                
            }
            fp << "\n";

            const int cell_type      = cell_types[m.dims-1];
            fp("CELL_TYPES %u\n", unsigned(num_cells));
            for(size_t i=0;i<num_cells;++i)
            {
                fp("%d\n", cell_type);
            }
            fp << "\n";
        }
        
        
        //______________________________________________________________________
        //
        // write a the mesh vertices as an unstructured grid
        //______________________________________________________________________
        void VTK:: write_mesh(ios::ostream &fp, const  mesh &m, const string &title) const
        {
            write_header(fp,title);
            write_mesh_vertices(fp,m);
        }
        
        
    }
}
