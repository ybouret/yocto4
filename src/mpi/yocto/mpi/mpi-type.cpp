#include "yocto/mpi/mpi.hpp"
#include "yocto/sort/heap.hpp"
#include "yocto/ios/graphviz.hpp"

namespace yocto
{
    mpi:: data_type:: data_type( const std::type_info &info, const MPI_Datatype ID) throw() :
    type_spec(info),
    id(ID)
    {
    }
    
    mpi:: data_type:: ~data_type() throw() {}
    
    mpi::data_type:: data_type( const data_type &other ) throw() :
    type_spec( other ),
    id( other.id )
    {
    }
    
    int mpi::data_type:: compare( const data_type &lhs, const data_type &rhs ) throw()
    {
        return lhs.compare_to( rhs );
    }
    
    MPI_Datatype mpi:: get_type(const type_spec &spec) const
    {
        size_t jhi = db.size();
        if(jhi>0 )
        {
            size_t jlo = 1;
            while(jhi>=jlo)
            {
                const size_t     mid  = (jlo+jhi)>>1;
                const data_type &item = db[mid];
                const int        cmp  = spec.compare_to(item);
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
    
    static inline
    void __append_data_type(mpi::db_type         &db,
                            const mpi::data_type &item
                            )
    {
        const type_spec &item_spec = item;
        for(size_t i=db.size();i>0;--i)
        {
            const type_spec &curr_spec = db[i];
            if( curr_spec == item_spec )
            {
                return;
            }
        }
        db.push_back(item);
    }
    
    void mpi:: gendb()
    {
        

#define Y_MPI_DB ((db_type&)db)
#define Y_MPI_REGISTER(T,ID) do {     \
const data_type item( typeid(T), ID); \
__append_data_type(Y_MPI_DB,item);    \
} while(false)
        
        //----------------------------------------------------------------------
        // reals
        //----------------------------------------------------------------------
        Y_MPI_REGISTER(float,         MPI_FLOAT);
        Y_MPI_REGISTER(double,        MPI_DOUBLE);
        Y_MPI_REGISTER(long double,   MPI_LONG_DOUBLE);


        //----------------------------------------------------------------------
        // standard integers, likely to be aliases
        //----------------------------------------------------------------------
        Y_MPI_REGISTER(char,    MPI_CHAR);
        Y_MPI_REGISTER(uint8_t, MPI_BYTE);
        Y_MPI_REGISTER(short,   MPI_SHORT);
        Y_MPI_REGISTER(unsigned short, MPI_UNSIGNED_SHORT);
        
        Y_MPI_REGISTER(int,           MPI_INT);
        Y_MPI_REGISTER(unsigned,      MPI_UNSIGNED);
        
        Y_MPI_REGISTER(long,          MPI_LONG);
        Y_MPI_REGISTER(unsigned long, MPI_UNSIGNED_LONG);
        
        Y_MPI_REGISTER(long long,          MPI_LONG_LONG);
        Y_MPI_REGISTER(unsigned long long, MPI_UNSIGNED_LONG_LONG);
        
        
        
        hsort(Y_MPI_DB, data_type::compare);

#if 0
        types_db.enroll<float>(MPI_FLOAT);
        types_db.enroll<double>(MPI_DOUBLE);
        types_db.enroll<long double>(MPI_LONG_DOUBLE);

        types_db.enroll<char>(MPI_CHAR);
        types_db.enroll<unsigned char>(MPI_BYTE);
        types_db.enroll<short>(MPI_SHORT);
        types_db.enroll<unsigned short>(MPI_UNSIGNED_SHORT);
        types_db.enroll<int>(MPI_INT);
        types_db.enroll<unsigned>(MPI_UNSIGNED);
        types_db.enroll<long>(MPI_LONG);
        types_db.enroll<unsigned long>(MPI_LONG);
        types_db.enroll<long long>(MPI_LONG_LONG);
        types_db.enroll<unsigned long long>(MPI_UNSIGNED_LONG_LONG);


        types_db.optimize();

        if(0==CommWorldRank)
        {
            types_db.graphviz("types_db.dot");
            ios::graphviz_render("types_db.dot");
        }
#endif

    }
    
    
}

