#ifndef YOCTO_SPADE_FIELDS_SETUP_INCLUDED
#define YOCTO_SPADE_FIELDS_SETUP_INCLUDED 1

#include "yocto/spade/field-info.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/memory/pooled.hpp"
#include "yocto/spade/array-db.hpp"

namespace yocto
{
 
    namespace spade
    {
     
        template <typename LAYOUT>
        class fields_setup
        {
        public:
            typedef field_info<LAYOUT> field_type;
            
            explicit fields_setup( size_t n ) : fields(n,as_capacity) {}
            virtual ~fields_setup() throw() {}
            
            //! append a new need for field...
            void append(const char                           *name,
                        const std::type_info                 &spec,
                        const typename field_type::array_ctor ctor,
                        const typename field_type::array_dtor dtor)
            {
                const field_type f(name,spec,ctor,dtor);
                for( size_t i=fields.size();i>0;--i)
                {
                    if( f.name == fields[i].name )
                        throw exception("spade.fields(multiple '%s')",f.name.c_str());
                }
                fields.push_back( f );
            }
            
            //! current #fields
            inline size_t size() const throw() { return fields.size(); }
            
            //! release memory
            inline void   clear() throw() { fields.release(); }
            
            //! create all fields with same layout into database
            inline void create( const LAYOUT &L, array_db &db )
            {
                for( size_t i=1; i <= fields.size(); ++i )
                {
                    const field_type &f = fields[i];
                    linear *info = 0;
                    void   *addr = f.ctor(L,&info);
                    try
                    {
                        varray::ptr vp( new varray(f.name, f.spec, addr, info, f.dtor) );
                        addr = 0;
                        db.insert(vp);
                    }
                    catch(...)
                    {
                        if( addr ) f.dtor(addr);
                        throw;
                    }
                }
            }
            
        private:
            vector< field_info<LAYOUT>, memory::pooled::allocator > fields;
            YOCTO_DISABLE_COPY_AND_ASSIGN(fields_setup);
        };

#define Y_SPADE_FIELD(F,NAME,ARRAY) do { (F).append(NAME,typeid(ARRAY), ARRAY::ctor, ARRAY::dtor); } while(false)
        
    }
}


#endif
