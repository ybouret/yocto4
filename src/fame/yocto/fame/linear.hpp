#ifndef YOCTO_FAME_LINEAR_INCLUDED
#define YOCTO_FAME_LINEAR_INCLUDED 1

#include "yocto/fame/types.hpp"
#include "yocto/fame/types-io.hpp"

#include "yocto/counted.hpp"
#include "yocto/type/args.hpp"
#include "yocto/string.hpp"
#include "yocto/ptr/intr.hpp"
#include <typeinfo>

namespace yocto
{
    namespace fame
    {
#if defined(_MSC_VER)
        // 'this' in ctor
#pragma warning ( disable : 4355 )
#endif
        class linear_space : public counted
        {
        public:
            //__________________________________________________________________
            //
            // data
            //__________________________________________________________________
            const string name;
            const size_t bytes; //!< linear bytes
            const size_t itmsz; //!< one item size
            
            //__________________________________________________________________
            //
            // virtual interface
            //__________________________________________________________________
            virtual ~linear_space() throw();
            
            virtual const std::type_info &get_typeid() const throw() = 0; //!< typeid  of array
            virtual void *                get_handle() const throw() = 0; //!< address of array
            
            //__________________________________________________________________
            //
            // non-virtual interface
            //__________________________________________________________________
            const string &key() const throw();  //!< the key for tables
            
            template <typename ARRAY>
            inline ARRAY & as()
            {
                check_typeid( typeid(ARRAY) );
                return *(ARRAY *) get_handle();
            }
            
            template <typename ARRAY>
            inline const ARRAY & as() const
            {
                check_typeid( typeid(ARRAY) );
                return *(ARRAY *) get_handle();
            }
            
            string make_id( unit_t idx ) const;
            
            void *       data() throw();
            const void * data() const throw();
            
            
        protected:
            size_t buflen;
            void  *buffer;
            explicit linear_space(const string &user_name,
                                  const size_t  num_bytes,
                                  const size_t  item_size,
                                  void         *user_data);
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(linear_space);
            void check_typeid(const std::type_info&aid) const;
            
        };
        
        typedef intr_ptr<string,linear_space> linear_ptr;
        typedef linear_space                 *linear_handle;
        
        
        template <typename T,typename LAYOUT>
        class linear : public LAYOUT, public linear_space
        {
        public:
            YOCTO_ARGUMENTS_DECL_T;
            inline virtual ~linear() throw() {}
            type *entry;
            
            inline void ld( param_type value)
            {
                for(size_t i=0;i<this->items;++i)
                {
                    entry[i] = value;
                }
            }
            
            type & operator()(unit_t offset) throw()
            {
                assert(offset>=0);
                assert(offset<this->items);
                return entry[offset];
            }
            
            const_type & operator()(unit_t offset) const throw()
            {
                assert(offset>=0);
                assert(offset<this->items);
                return entry[offset];
            }
            
        protected:
            inline explicit linear(const string &user_name,
                                   const LAYOUT &L,
                                   void         *user_data ) :
            LAYOUT(L),
            linear_space(user_name,
                         this->items * sizeof(T),
                         sizeof(T),
                         user_data),
            entry( (type*)buffer )
            {
                assert(entry);
            }
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(linear);
        };
        
    }
}


#endif
