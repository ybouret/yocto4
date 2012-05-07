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
        
        //! virtual array
        /**
         smart pointee on one array address,
         with automatic memory allocation
         */
        class varray : public object, public counted
        {
        public:
            //! default constructor
            /**
             \param array_name array identifier
             \param array_spec array class type
             \param array_addr handle to the array
             \param array_info memory requirements for the array
             \param array_kill array destructor wrapper
             */
            explicit varray(const string         &array_name, 
                            const type_spec      &array_spec,
                            void *                array_addr,
                            linear_base          *array_info,
                            void                (*array_kill)(void *));
            
            //! default destructor
            virtual ~varray() throw();
            
            //! for array::ptr
            const string & key() const throw();
            
            //! recover the original array
            template <typename ARRAY>
            inline ARRAY &as()
            {
                const type_spec required( typeid( ARRAY ) );
                check_specs( spec, required);
                return *(ARRAY *)( addr );
            }
            
            template <typename ARRAY>
            inline const ARRAY &as() const
            {
                const type_spec required( typeid( ARRAY ) );
                check_specs( spec, required);
                return *(ARRAY *)( addr );
            }
            
            typedef intrusive_ptr<string,varray> ptr; //!< smart pointer for the database
            typedef set<string,varray::ptr>      db;  //!< array database
            
            const string    name; //!< array unique name
            const type_spec spec; //!< array class type
            
            linear_base *handle() throw();
            
        private:
            void             *addr;
            void            (*kill)(void *);
            linear_base      *info;
            const data_block  data;
            static void check_specs( const type_spec &self, const type_spec &required);
            
            YOCTO_DISABLE_COPY_AND_ASSIGN(varray);
        };
        
        
        //! database of virtual arrays
        class array_db 
        {
        public:
            explicit array_db() throw();
            explicit array_db( size_t n);
            virtual ~array_db() throw();
            
            //! take care of addr
            void operator()(const string         &name, 
                            const type_spec      &spec, 
                            void                 *addr,
                            linear_base          *info,
                            void                (*kill)( void *)
                            );
            
            varray       &operator[]( const string &name );
            const varray &operator[]( const string &name ) const;
            
            varray       &operator[]( const char *id );
            const varray &operator[]( const char *id ) const;
            
            
        private:
            varray::db arrays;
            YOCTO_DISABLE_COPY_AND_ASSIGN(array_db);
        };
        
    }
    
}


#endif
