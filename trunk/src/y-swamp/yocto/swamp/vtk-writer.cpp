#include "yocto/swamp/vtk-writer.hpp"
#include "yocto/exception.hpp"

#include "yocto/geom/v3d.hpp"

namespace yocto 
{
    namespace swamp 
    {
        
        
        vtk_format:: vtk_format() :
        real_fmt( "%g" ),
        unit_fmt( "%ld" )
        {}
        
        vtk_format:: ~vtk_format() throw() {}
        
        
        vtk_writer:: record:: record( const vtk_writer::record &other ) :
        spec( other.spec ),
        kind( other.kind ),
        proc( other.proc )
        {
            assert(proc!=NULL);
        }
        
        vtk_writer:: record:: record( const std::type_info &id, const char *k, callback cb ) :
        spec( id ),
        kind( k  ),
        proc( cb )
        {
            assert(proc!=NULL);
        }
        
        typedef geom::v3d<float>  v3d_flt;
        typedef geom::v3d<double> v3d_dbl;

        
        namespace 
        {
            static inline void write_float( ios::ostream &fp, const void *data, const vtk_format &format )
            {
                assert(data);
                const float x = *(float*)data;
                fp( format.real_fmt.c_str(), double(x) );
            }
            
            static inline void write_double( ios::ostream &fp, const void *data, const vtk_format &format )
            {
                assert(data);
                const double x = *(double*)data;
                fp( format.real_fmt.c_str(), x );
            }
            
            static inline void write_unit( ios::ostream &fp, const void *data, const vtk_format &format )
            {
                assert(data);
                const unit_t x = *(unit_t*)data;
                fp( format.unit_fmt.c_str(), static_cast<long int>(x) );
            }
            
            static inline void write_v3d_flt( ios::ostream &fp, const void *data, const vtk_format &format )
            {
                assert(data);
                const v3d_flt &v = *(v3d_flt *)data;
                fp( format.real_fmt.c_str(), double(v.x) );
                fp.write(' ');
                fp( format.real_fmt.c_str(), double(v.y) );
                fp.write(' ');
                fp( format.real_fmt.c_str(), double(v.z) );
            }
            
            static inline void write_v3d_dbl( ios::ostream &fp, const void *data, const vtk_format &format )
            {
                assert(data);
                const v3d_dbl &v = *(v3d_dbl *)data;
                fp( format.real_fmt.c_str(), (v.x) );
                fp.write(' ');
                fp( format.real_fmt.c_str(), (v.y) );
                fp.write(' ');
                fp( format.real_fmt.c_str(), (v.z) );
            }

            
        }
        
#define Y_SWAMP_RECORD(TYPE,KIND,PROC) \
do { const record r( typeid(TYPE), KIND, PROC); if( !out_db.insert(r) ) throw exception("vtk_writer(multiple '%s')", r.spec.name()); } while(false)
        
#define Y_SWAMP_SCALARS(TYPE,PROC) Y_SWAMP_RECORD(TYPE,"SCALARS",PROC)
#define Y_SWAMP_VECTORS(TYPE,PROC) Y_SWAMP_RECORD(TYPE,"VECTORS",PROC)
        
        
        vtk_writer:: vtk_writer() : 
        format(),
        out_db(8,as_capacity)
        {
            Y_SWAMP_SCALARS(float,write_float);
            Y_SWAMP_SCALARS(double,write_double);
            Y_SWAMP_SCALARS(unit_t,write_double);
            Y_SWAMP_VECTORS(v3d_flt,write_v3d_flt);
            Y_SWAMP_VECTORS(v3d_dbl,write_v3d_dbl);

        }
        
        vtk_writer:: ~vtk_writer() throw()
        {
        }
        
        void vtk_writer:: prolog( ios::ostream &fp, const string &name, size_t num, const record &r) const
        {
            fp("POINT_DATA %u\n", unsigned(num));
            fp("%s %s float\n", r.kind.c_str(), name.c_str());
            fp("LOOKUP_TABLE default\n");
        }

        void vtk_writer:: write1( ios::ostream &fp, const void *data, const record &r) const
        {
            assert(data);
            assert(r.proc);
            r.proc( fp, data, format );
        }

        
        const vtk_writer:: record & vtk_writer:: operator[]( const type_spec &spec ) const
        {
            const record *r = out_db.search( spec );
            if( !r )
                throw exception("vtk_writer[no '%s']", spec.name() );
            return *r;
        }

        
    }
}
