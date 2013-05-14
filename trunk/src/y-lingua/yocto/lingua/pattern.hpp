#ifndef YOCTO_LINGUA_PATTERN_INCLUDED
#define YOCTO_LINGUA_PATTERN_INCLUDED 1

#include "yocto/lingua/source.hpp"
#include "yocto/ios/ostream.hpp"
#include "yocto/ios/istream.hpp"

#include "yocto/core/meta-list.hpp"

namespace yocto
{
    namespace lingua
    {
        
        //! a cloneable pattern
        class pattern : public token
        {
        public:
            const uint32_t type; //!< for identifier
            pattern       *prev; //!< for list
            pattern       *next; //!< for list
            void          *data; //!< access to special data
            
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
            
            //! save a binary representation
            virtual void save( ios::ostream &fp ) const = 0;
            
            //! human readable name
            string  hr_name() const;
            
            //! binary content
            string content() const;
            
            //! based on binary representations
            friend bool operator==( const pattern &lhs, const pattern &rhs );
            friend bool operator!=( const pattern &lhs, const pattern &rhs );
            
            //! loading from a binary representation
            static pattern *load( ios::istream &fp);
            
            //! optimization: default is do nothing
            virtual void optimize() throw();
            
        protected:
            explicit pattern(uint32_t t) throw();
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(pattern);
        };
        
        typedef core::meta_list<pattern> p_list;
    }
}

#endif
