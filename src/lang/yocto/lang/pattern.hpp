#ifndef YOCTO_LANG_PATTERN_INCLUDED
#define YOCTO_LANG_PATTERN_INCLUDED 1

#include "yocto/lang/source.hpp"
#include "yocto/core/meta-list.hpp"
#include "yocto/code/fourcc.hpp"


namespace yocto
{
    namespace lang
    {
        
        class pattern : public token
        {
        public:
            virtual ~pattern() throw();
            
            //! clone interface
            virtual pattern * clone() const = 0;
            
            //! true is a match is possible
            /**
             must left the source untouched otherwise !
             */
            virtual bool      match( source &src, ios::istream & ) = 0;
            
            //! clear all internal data
            /**
             default is this->clear
             */
            virtual void      reset() throw();
            
            pattern       *next;
            pattern       *prev;
            const uint32_t type; //!< UUID
            void          *data; //!< to handle private data
            
        protected:
            explicit pattern( const uint32_t user_type ) throw();
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(pattern);
        };
        
#define YOCTO_LANG_PATTERN_API() \
virtual pattern *clone() const
        
        typedef core::meta_list<pattern> p_list;
        
    }
}

#endif

