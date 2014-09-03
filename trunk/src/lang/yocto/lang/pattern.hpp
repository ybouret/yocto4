#ifndef YOCTO_LANG_PATTERN_INCLUDED
#define YOCTO_LANG_PATTERN_INCLUDED 1

#include "yocto/lang/source.hpp"
#include "yocto/core/meta-list.hpp"
#include "yocto/code/fourcc.hpp"
#include "yocto/ios/ostream.hpp"

namespace yocto
{
    namespace lang
    {
        
        class pattern : public token
        {
        public:
            virtual ~pattern() throw();
            
            pattern       *next;
            pattern       *prev;
            const uint32_t type; //!< UUID
            void          *self; //!< class corresponding to type
            
            //! clone interface
            virtual pattern * clone() const = 0;
            
            //! GraphViz interface
            virtual void viz( ios::ostream & ) const = 0;
            
            //! save interface
            virtual void save( ios::ostream & ) const = 0;
            
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
            
           
            //! output a readable GraphViz label
            static void outviz( const char C, ios::ostream &fp );
            
            //! save a directed graph of the pattern
            void graphviz( const string &filename ) const;
            
            //! save a directed graph of the pattern
            void graphviz( const char   *fn ) const;

            //! refactor a pattern, default is do nothing
            virtual void refactor() throw();
            
            //! refactor and optimize recursively the structure of a pattern
            static pattern *optimize(pattern *p) throw();
            
            //! not thread safe
            static const char *fourcc_of(uint32_t tag) throw();
            
            //! not thread safe
            const char *fourcc() const throw();
            
            //! using the save function;
            string to_binary() const;
            
            //! reloading a pattern
            static pattern *load(ios::istream &);
            
            
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

