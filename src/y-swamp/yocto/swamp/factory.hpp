#ifndef YOCTO_SWAMP_FACTORY_INCLUDED
#define YOCTO_SWAMP_FACTORY_INCLUDED 1

#include "yocto/swamp/arrays.hpp"
#include "yocto/exception.hpp"

namespace yocto 
{
    namespace swamp
    {
        template <typename LAYOUT>
        class factory 
        {
        public:
            explicit factory() throw() : sheds()  {}
            virtual ~factory() throw() {}
            
            typedef void  * (*array_ctor)( const LAYOUT & , linear_base **);
            typedef void    (*array_dtor)(void *);
            
            inline void record( const std::type_info &which, array_ctor ctor, array_dtor dtor )
            {
                const shed param( which, ctor, dtor );
                if( ! sheds.insert( param ) )
                    throw exception("swamp::factory(multiple '%s')", which.name() );
            }
            
            template <typename ARRAY>
            inline void use()
            {
                record( typeid( ARRAY ), ARRAY::ctor, ARRAY::dtor );
            }
            
            void produce( const string &name, const LAYOUT &L, const std::type_info &which, array_db &db )
            {
                const shed * param = sheds.search( which );
                if( !param )
                    throw exception("swam::factory(can't produce '%s')", which.name() );
                linear_base *info = NULL;
                void        *addr = param->ctor( L, &info );
                db.record(name, which, addr, info, param->dtor);
            }
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(factory);
            class shed
            {
            public:
                const   type_spec  spec;
                const   array_ctor ctor;
                const   array_dtor  dtor;
                
                inline shed(const std::type_info  &which, 
                            const array_ctor       _ctor,
                            const array_dtor       _dtor ) throw() :
                spec( which ),
                ctor( _ctor ),
                dtor( _dtor )
                {
                    assert( ctor != NULL ); assert( dtor != NULL );
                }
                inline ~shed() throw() {}
                inline shed( const shed &other ) throw() :
                spec( other.spec ), ctor(other.ctor), dtor(other.dtor) {}
                
                const type_spec &key() const throw() { return spec; }
                
            private:
                YOCTO_DISABLE_ASSIGN(shed);
            };
            set<type_spec,shed> sheds;
        };
        
    }
    
}

#endif
