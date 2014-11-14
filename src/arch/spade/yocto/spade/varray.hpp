#ifndef YOCTO_SPADE_VARRAY_INCLUDED
#define YOCTO_SPADE_VARRAY_INCLUDED 1

#include "yocto/type/spec.hpp"
#include "yocto/spade/data-block.hpp"
#include "yocto/ptr/intr.hpp"
#include "yocto/counted.hpp"
#include "yocto/string.hpp"

namespace yocto
{
    namespace spade
    {
                
        //! virual array
        /**
         generic array handling
         */
        class varray : public object, public counted
        {
        public:
            //! default constructor
            /**
             \param array_name array identifier
             \param array_spec array class type
             \param array_held type held by array
             \param array_addr address of the array
             \param array_info memory requirements for the array
             \param array_dtor array destructor wrapper
             \warning string copy may throw
             */
            explicit varray(const string         &array_name,
                            const type_spec      &array_spec,
                            const type_spec      &array_held,
                            void *                array_addr,
                            linear               *array_info,
                            void                (*array_dtor)(void *));
            
            //! default destructor
            virtual ~varray() throw();
            
            
            typedef intr_ptr<string,varray> ptr;
            const string & key() const throw();
            
            const string    name; //!< array name
            const type_spec spec; //!< array typeid
            const type_spec held; //!< typeid of type in array
            
            linear &       handle() throw();
            const linear & handle() const throw();
            
            template <typename ARRAY>
            inline ARRAY &as()
            {
                const type_spec requested = typeid(ARRAY);
                check_spec(requested);
                return *(ARRAY*)addr;
            }
            
            template <typename ARRAY>
            inline const ARRAY &as() const
            {
                const type_spec requested = typeid(ARRAY);
                check_spec(requested);
                return *(ARRAY*)addr;
            }

            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(varray);
            void   *        addr;          //!< array[1|2|3]D address
            linear *        info;          //!< associated linear info
            void           (*dtor)(void*); //!< to destroy addr
            const data_block data;
            void check_spec( const type_spec &requested ) const;
            
        };
        
    }
}

#endif
