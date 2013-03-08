#ifndef YOCTO_SPADE_VISIT_SILO_INCLUDED
#define YOCTO_SPADE_VISIT_SILO_INCLUDED 1

#include "silo.h"
#include "yocto/spade/quadmesh.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace spade
    {
        
        struct Silo
        {
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
                    DBClose( dbfile );
                    dbfile = 0;
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
                        throw exception("Can't Open Silo file '%s'", filename.c_str());
                }
            };
        };
        
    }
}

#endif

