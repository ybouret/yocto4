#ifndef YOCTO_SPADE_VISIT_SILO_INCLUDED
#define YOCTO_SPADE_VISIT_SILO_INCLUDED 1

#if 0
#include "silo.h"
#include "yocto/spade/quadmesh.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace spade
    {
        
        struct Silo
        {
            
            class File;
            
            //! DBoptlist wrapper
            class OptList
            {
            public:
                explicit OptList(size_t n) : optlist( DBMakeOptlist(n) )
                {
                    if(!optlist) throw exception("Can't create DBoptlist");
                }
                
                virtual ~OptList() throw()
                {
                    assert(optlist);
                    DBFreeOptlist(optlist);
                    optlist = 0;
                }
                
                // data must be persitent
                inline void operator()( int kind, const void *data)
                {
                    assert(optlist);
                    DBAddOption(optlist,kind,(void*)data);
                }
                
                
            private:
                DBoptlist *optlist;
                YOCTO_DISABLE_COPY_AND_ASSIGN(OptList);
                friend class File;
            };
            
            
            //! DBfile wrapper
            class File
            {
            public:
                inline File(const string &filename, const string &title) :
                dbfile(0)
                {
                    __Open(filename,title);
                }
                
                virtual ~File() throw()
                {
                    assert(dbfile);
                    DBClose( dbfile );
                    dbfile = 0;
                }
                
                template <typename RMESH>
                void PutRectilinearMesh( const string &name, const RMESH &mesh, OptList *opt )
                {
                    const int   ndims         = RMESH::DIMENSIONS; assert(ndims<=3); assert(ndims>0);
                    char       *coordsname[3] = { 0,0,0 };
                    int         dims[3]       = { 0,0,0 };
                    float      *coords[3]     = { 0,0,0 };
                    DBoptlist  *optlist       = 0;
                    
                    if(opt)
                    {
                        assert(opt->optlist);
                        optlist = opt->optlist;
                    }
                    
                    for( int i=0; i < ndims; ++i )
                    {
                        const dimension_t dim = dimension_t(i);
                        const char *axis_name = RMESH::get_axis_name( dim );
                        coordsname[i]         = (char *)axis_name;
                        const typename RMESH::axis_type &axis = mesh.get_axis(dim);
                        dims[i]   = axis.items;
                        coords[i] = (float *) axis.entry;
                    }
                    
                    int data_type = -1;
                    {
                        static const type_spec spec_flt = typeid(float);
                        static const type_spec spec_dbl = typeid(double);
                        static const type_spec spec_dat = typeid( typename RMESH::type );
                        if( spec_dat == spec_flt )
                        {
                            data_type = DB_FLOAT;
                            goto PUT_QUADMESH;
                        }
                        
                        if( spec_dat == spec_dbl )
                        {
                            data_type = DB_DOUBLE;
                            goto PUT_QUADMESH;
                        }
                        
                        throw exception("Silo.File.PutRectilinearMesh(invalid type '%s')", spec_dat.name());
                    }
                PUT_QUADMESH:
                    DBPutQuadmesh(dbfile,
                                  name.c_str(),
                                  coordsname,
                                  coords,
                                  dims,
                                  ndims,
                                  data_type,
                                  DB_COLLINEAR,
                                  optlist);
                }
                
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(File);
                DBfile *dbfile;
                void __Open( const string &filename, const string &title )
                {
                    assert(0==dbfile);
                    dbfile = DBCreate(filename.c_str(),
                                      DB_CLOBBER,
                                      DB_LOCAL,
                                      title.c_str(),
                                      DB_HDF5);
                    if(!dbfile)
                        throw exception("Can't create DBfile '%s'", filename.c_str());
                }
            };
            
            
            
        };
        
    }
}
#endif

#endif

