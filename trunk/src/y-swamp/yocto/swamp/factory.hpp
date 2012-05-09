#ifndef YOCTO_SWAMP_FACTORY_INCLUDED
#define YOCTO_SWAMP_FACTORY_INCLUDED 1

#include "yocto/swamp/arrays.hpp"
#include "yocto/exception.hpp"

namespace yocto 
{
    namespace swamp
    {
        //! factory of any array, for a given dimension
        template <typename LAYOUT>
        class factory 
        {
        public:
            explicit factory() throw() : sheds()  {}
            explicit factory( size_t n ) : sheds( n, as_capacity ) {}
            virtual ~factory() throw() {}
            
            typedef void  * (*array_ctor)( const LAYOUT & , linear_base **);
            typedef void    (*array_dtor)(void *);
            
                       
            //! permissive recording of an array with its content
            inline void record( const type_spec  &array_spec, const type_spec &array_held, array_ctor ctor, array_dtor dtor )
            {
                const shed param( array_spec, array_held, ctor, dtor );
                (void) sheds.insert( param );
            }
            
            //! permissive recording of std::type_info for the array and its content
            inline void record( const std::type_info &array_spec, const std::type_info &array_held, array_ctor ctor, array_dtor dtor )
            {
                const type_spec spec( array_spec );
                const type_spec held( array_held );
                record(spec,held,ctor,dtor);
            }

            //! templated recording of type ARRAY
            template <typename ARRAY>
            inline void use() 
            {
                record( typeid( ARRAY ), typeid(typename ARRAY::type), ARRAY::ctor, ARRAY::dtor ); 
            }
            
            
            //! create and register an array 
            inline void produce( const string &name, const LAYOUT &L, const type_spec &spec, array_db &db )
            {
                const shed * param = sheds.search( spec );
                if( !param )
                    throw exception("swamp::factory(can't produce '%s')", spec.name() );
                linear_base *info = NULL;
                void        *addr = param->ctor( L, &info );
                db(name, spec, param->held, addr, info, param->dtor);
            }
            
            inline void produce( const string &name, const LAYOUT &L, const std::type_info &array_spec,  array_db &db )
            {
                const type_spec spec(array_spec);
                produce(name,L,spec,db);
            }
            
            //! make with auto-registering
            template <typename ARRAY>
            inline void make( const string &name, const LAYOUT &L, array_db &db )
            {
                const std::type_info &kind = typeid(ARRAY);
                const type_spec       spec(kind);
                if( !sheds.search(spec) )
                {
                    use<ARRAY>();
                }
                produce( name, L, spec, db );
            }
            
            template <typename ARRAY>
            inline void make( const char *id, const LAYOUT &L, array_db &db )
            {
                const string name(id);
                make<ARRAY>( name, L, db );
            }
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(factory);
           
            
            class shed
            {
            public:
                const   type_spec  spec;
                const   type_spec  held;
                const   array_ctor ctor;
                const   array_dtor dtor;
                
                inline shed(const type_spec       &array_spec,
                            const type_spec       &array_held,
                            const array_ctor       _ctor,
                            const array_dtor       _dtor ) throw() :
                spec( array_spec ),
                held( array_held ),
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
