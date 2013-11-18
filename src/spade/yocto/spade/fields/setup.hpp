#ifndef YOCTO_SPADE_FIELDS_SETUP_INCLUDED
#define YOCTO_SPADE_FIELDS_SETUP_INCLUDED 1

#include "yocto/spade/fields/info.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/memory/pooled.hpp"
#include "yocto/spade/array-db.hpp"
#include "yocto/spade/linear-handles.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    
    namespace spade
    {
        
        template <typename LAYOUT>
        class fields_setup
        {
        public:
            typedef field_info<LAYOUT> field_type;
            static const size_t DEFAULT_CAPACITY = 8;
            const size_t               max_interleaved_size;
            
            //! prepare fields
            /**
             \param chunk maximum size (in bytes) of items simultaneously sent
             */
            explicit fields_setup( size_t chunk ) :
            max_interleaved_size(chunk),
            fields(DEFAULT_CAPACITY,as_capacity)
            {
                
            }
            
            virtual ~fields_setup() throw() {}
            
            //! append a new need for field...
            void append(const char                           *name,
                        const std::type_info                 &spec,
                        const std::type_info                 &held,
                        const typename field_type::array_ctor ctor,
                        const typename field_type::array_dtor dtor)
            {
                const field_type f(name,spec,held,ctor,dtor);
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
            inline void create(const LAYOUT &L, array_db &db) const
            {
                for( size_t i=1; i <= fields.size(); ++i )
                {
                    const field_type &f = fields[i];
                    linear *info = 0;
                    void   *addr = f.ctor(L,&info);
                    try
                    {
                        varray::ptr vp( new varray(f.name, f.spec, f.held, addr, info, f.dtor) );
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
        
#define Y_SPADE_FIELD(F,NAME,ARRAY)   do { (F).append(NAME,typeid(ARRAY), typeid(ARRAY::type), ARRAY::ctor, ARRAY::dtor); } while(false)
#define Y_SPADE_FIELD_T(F,NAME,ARRAY) do { (F).append(NAME,typeid(ARRAY), typeid(typename ARRAY::type), ARRAY::ctor, ARRAY::dtor); } while(false)
        
    }
}


#endif
