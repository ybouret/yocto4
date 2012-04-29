#ifndef YOCTO_SWAMP_ARRAYS_INCLUDED
#define YOCTO_SWAMP_ARRAYS_INCLUDED 1

#include "yocto/type-spec.hpp"
#include "yocto/intrusive-ptr.hpp"
#include "yocto/swamp/data-block.hpp"
#include "yocto/associative/set.hpp"

namespace yocto 
{
    namespace swamp
    {
        
        class varray : public object, public counted
        {
        public:
            explicit varray(const string         &array_name, 
                            const type_spec      &array_spec,
                            void *                array_addr,
                            linear_base          *array_info,
                            void                (*array_kill)(void *)) :
            name(  array_name ),
            spec(  array_spec ),
            addr(  array_addr ),
            kill(  array_kill ),
            data( *array_info )
            {
                assert( addr != NULL );
                assert( kill != NULL );
            }
            
            virtual ~varray() throw()
            {
                kill( addr );
            }
            
            const string    name;
            const type_spec spec;
            
            const string & key() const throw() { return name; }
            
            template <typename ARRAY>
            ARRAY &as()
            {
                const type_spec sp( typeid( ARRAY ) );
                assert( sp == spec );
                return *(ARRAY *)( addr );
            }
            
            typedef intrusive_ptr<string,varray> ptr;
            typedef set<string,varray::ptr>      db;
            
        private:
            void             *addr;
            void            (*kill)(void *);
            const data_block  data;
            
            YOCTO_DISABLE_COPY_AND_ASSIGN(varray);
        };
        
        class array_db 
        {
        public:
            explicit array_db() throw();
            virtual ~array_db() throw();
            
            //! take care of addr
            void append(const string         &name, 
                        const type_spec      &spec, 
                        void                 *addr,
                        linear_base          *info,
                        void                (*kill)( void *)
                        );
            
            varray       &operator[]( const string &name );
            const varray &operator[]( const string &name ) const;
            
        private:
            varray::db arrays;
            YOCTO_DISABLE_COPY_AND_ASSIGN(array_db);
        };
        
    }
    
}


#endif
