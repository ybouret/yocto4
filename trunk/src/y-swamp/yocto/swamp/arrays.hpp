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
        
        class array : public object, public counted
        {
        public:
            explicit array(const string         &array_name, 
                           const std::type_info &array_spec,
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
            
            virtual ~array() throw()
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
            
            typedef intrusive_ptr<string,array> ptr;
            typedef set<string,array::ptr>      db;
            
        private:
            void             *addr;
            void            (*kill)(void *);
            const data_block  data;
            
            YOCTO_DISABLE_COPY_AND_ASSIGN(array);
        };
        
        class array_db 
        {
        public:
            explicit array_db() throw();
            virtual ~array_db() throw();
            
            //! take care of addr
            void record(const string         &name, 
                        const std::type_info &which, 
                        void                 *addr,
                        linear_base          *info,
                        void                (*kill)( void *)
                        );
            
            array       &operator[]( const string &name );
            const array &operator[]( const string &name ) const;
            
        private:
            array::db arrays;
            YOCTO_DISABLE_COPY_AND_ASSIGN(array_db);
        };
        
    }
    
}


#endif
