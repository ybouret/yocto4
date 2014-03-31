#include "yocto/fovea/vtk.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace fovea
    {
        
        vtk:: ~vtk() throw()
        {
        }
        
        namespace
        {
            static inline
            void write_float( ios::ostream &fp, const vtk &VTK, const void *addr )
            {
                assert(addr);
                const double x = *(float *)addr;
                fp( VTK.real_fmt.c_str(), x );
            }
            
            static inline
            void write_double( ios::ostream &fp, const vtk &VTK, const void *addr )
            {
                const double x = *(double *)addr;
                fp( VTK.real_fmt.c_str(), x );
            }
            
            static inline
            void write_char( ios::ostream &fp, const vtk &VTK, const void *addr)
            {
                const int x = *(char *)addr;
                fp( VTK.unit_fmt.c_str(),x);
            }
            
            static inline
            void write_short( ios::ostream &fp, const vtk &VTK, const void *addr)
            {
                const int x = *(short *)addr;
                fp( VTK.unit_fmt.c_str(),x);
            }
            
            static inline
            void write_int( ios::ostream &fp, const vtk &VTK, const void *addr)
            {
                const int x = *(int *)addr;
                fp( VTK.unit_fmt.c_str(),x);
            }
            
            static inline
            void write_long( ios::ostream &fp, const vtk &VTK, const void *addr)
            {
                const int x = *(long *)addr;
                fp( VTK.unit_fmt.c_str(),x);
            }
            
            static inline
            void write_v2d_f( ios::ostream &fp, const vtk &VTK, const void *addr)
            {
                const math::v2d<float> &v = *(math::v2d<float> *)addr;
                write_float(fp,VTK,&v.x);
                fp.write(' ');
                write_float(fp,VTK,&v.y);
            }
            
            static inline
            void write_v3d_f( ios::ostream &fp, const vtk &VTK, const void *addr)
            {
                const math::v3d<float> &v = *(math::v3d<float> *)addr;
                write_float(fp,VTK,&v.x);
                fp.write(' ');
                write_float(fp,VTK,&v.y);
                fp.write(' ');
                write_float(fp,VTK,&v.z);
            }
            
            
            static inline
            void write_v2d_d( ios::ostream &fp, const vtk &VTK, const void *addr)
            {
                const math::v2d<double> &v = *(math::v2d<double> *)addr;
                write_double(fp,VTK,&v.x);
                fp.write(' ');
                write_double(fp,VTK,&v.y);
            }
            
            static inline
            void write_v3d_d( ios::ostream &fp, const vtk &VTK, const void *addr)
            {
                const math::v3d<double> &v = *(math::v3d<double> *)addr;
                write_double(fp,VTK,&v.x);
                fp.write(' ');
                write_double(fp,VTK,&v.y);
                fp.write(' ');
                write_double(fp,VTK,&v.z);
            }

            
        }
        
        vtk:: vtk() :
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
        
        void vtk:: define( const std::type_info &tid, callback cb)
        {
            assert(cb);
            const type_spec sp( tid );
            if( db.search(sp) )
                throw exception("vtk.db(insert multiple type '%s')", sp.name() );
            if( !db.insert(sp,cb) )
                throw exception("vtk.db(unexpected insert failure for type '%s')", sp.name());
        }
        
        
        const char vtk::name[] = "vtk";
        
        
        void vtk::output( ios::ostream &fp, const std::type_info &tid, const void *addr) const
        {
            assert(addr);
            const type_spec sp(tid);
            const callback *pCB = db.search(sp);
            if(!pCB)
                throw exception("vtk::output(undefined type '%s')", sp.name() );
            (*pCB)(fp,*this,addr);
        }
        
    }
}
