#ifndef YOCTO_SWAMP_FIELDS_INCLUDED 
#define YOCTO_SWAMP_FIELDS_INCLUDED 1

#include "yocto/swamp/factory.hpp"


namespace yocto 
{
    namespace swamp 
    {
        
        //! information to create an array of a given dimension
        template <typename LAYOUT>
        class field_info 
        {
        public:
            typedef typename factory<LAYOUT>::array_ctor array_ctor;
            typedef typename factory<LAYOUT>::array_dtor array_dtor;
            
            inline field_info(const char           * _name,
                              const std::type_info & _spec,
                              const std::type_info & _held,
                              const bool             _async,
                              const array_ctor       _ctor,
                              const array_dtor       _dtor) :
            name(  _name  ),
            spec(  _spec  ),
            held(  _held  ),
            async( _async ),
            ctor(  _ctor  ),
            dtor(  _dtor  )
            {
            }
            
            inline field_info( const field_info &other ) :
            name(  other.name  ),
            spec(  other.spec  ),
            held(  other.held  ),
            async( other.async ),
            ctor(  other.ctor  ),
            dtor(  other.dtor  )
            {

            }
            
            inline ~field_info() throw() {}
            
            const string     name;  //!< variable name
            const type_spec  spec;  //!< field type
            const type_spec  held;  //!< associated field content
            const bool       async; //!< exchanged ?
            const array_ctor ctor;  //!< the constructor
            const array_dtor dtor;  //!< the destructor
            
            inline const string & key() const throw() { return name; }
            typedef set<string,field_info<LAYOUT> > db;
            
        private:
            YOCTO_DISABLE_ASSIGN(field_info);
        };
        
        
        //! store information to create some fields in a dataspace
        template <typename LAYOUT>
        class fields_setup : public  field_info<LAYOUT>::db
        {
        public:
            typedef typename field_info<LAYOUT>::db db_type;
            explicit fields_setup() throw() : db_type() {}
            virtual ~fields_setup() throw() {}
            explicit fields_setup(size_t n) : db_type(n,as_capacity) {}
            
            template <typename ARRAY>
            void add( const char *name, bool async )
            {
                const field_info<LAYOUT> f( name, typeid(ARRAY), typeid( typename ARRAY::type), async, ARRAY::ctor, ARRAY::dtor );
                if( ! this->insert(f) )
                    throw exception("field_layout%uD( multiple '%s')", unsigned(LAYOUT::DIMENSIONS), f.name.c_str() );
            }
            
            typedef typename db_type::const_iterator iterator;
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(fields_setup);
        };
        
#define Y_SWAMP_DECL_VAR(F,NAME,TYPE) (F).add<TYPE>( NAME, true  )
#define Y_SWAMP_DECL_AUX(F,NAME,TYPE) (F).add<TYPE>( NAME, false )
        
        
    }
}

#endif
