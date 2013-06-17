#include "yocto/mpi/mpi.hpp"

namespace yocto
{
    mpi:: db_item:: db_item( const std::type_info &info, const MPI_Datatype ID) throw() :
    spec(info),
    id(ID)
    {
    }
    
    mpi:: db_item:: ~db_item() throw() {}
    
    mpi::db_item:: db_item( const db_item &other ) throw() :
    spec( other.spec ),
    id( other.id )
    {
    }
    
    int mpi::db_item:: compare( const db_item &lhs, const db_item &rhs ) throw()
    {
        return lhs.spec.compare_to( rhs.spec );
    }
    
    MPI_Datatype mpi:: get_type(  const type_spec &spec) const
    {
        size_t jhi = db.size();
        if(jhi>0 )
        {
            size_t jlo = 1;
            while(jhi>=jlo)
            {
                const size_t   mid  = (jlo+jhi)>>1;
                const db_item &item = db[mid];
                const int      cmp  = spec.compare_to(item.spec);
                if(cmp == 0 )
                {
                    return item.id;
                }
                else
                {
                    if(cmp<0)
                    {
                        jhi = mid-1;
                    }
                    else
                    {
                        assert(cmp>0);
                        jlo = mid+1;
                    }
                }
            }
        }
        throw yocto::exception("no MPI_TYPE for '%s'", spec.name() );
    }
    
    MPI_Datatype mpi:: get_type(  const std::type_info &info ) const
    {
        const type_spec spec(info);
        return get_type(spec);
    }
}

