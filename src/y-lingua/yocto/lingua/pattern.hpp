#ifndef YOCTO_LINGUA_PATTERN_INCLUDED
#define YOCTO_LINGUA_PATTERN_INCLUDED 1

#include "yocto/lingua/source.hpp"
#include "yocto/ios/ostream.hpp"
#include "yocto/core/meta-list.hpp"

namespace yocto
{
    namespace lingua
    {
        
        //! a cloneable pattern
        class pattern : public token
        {
        public:
            pattern *prev;
            pattern *next;
            
            virtual ~pattern() throw();
            
            //! default this->clear()
            virtual void reset() throw();
            
            //! clone interface
            virtual pattern *clone() const = 0;
            
            //! true if accept the source
            /**
             \param src the source to be tested
             the pattern must be empty before call.
             if the source doesn't match, then
             it must be restored, and the pattern left empty
             */
            virtual bool accept( source &src ) = 0;
            
            virtual void save( ios::ostream &fp ) const = 0;
            
            string  make_name() const;
            
        protected:
            explicit pattern() throw();
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(pattern);
        };
        
        typedef core::meta_list<pattern> p_list;
    }
}

#endif
