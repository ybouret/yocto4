#ifndef YOCTO_LANG_PATTERN_INCLUDED
#define YOCTO_LANG_PATTERN_INCLUDED 1

#include "yocto/lang/source.hpp"
#include "yocto/counted-object.hpp"
#include "yocto/ios/ostream.hpp"

namespace yocto
{
    namespace lang
    {

#define Y_LANG_PATTERN_MATCH_ARGS token &tkn, source &src, ios::istream &fp

        //! base class to match a source of t_char
        class pattern : public counted_object
        {
        public:
            pattern       *next;
            pattern       *prev;
            const uint32_t uuid;
            virtual ~pattern() throw();

            //! clone the behavior
            virtual pattern *clone() const = 0;

            //! matching function
            /** 
             if matching, content is append to tkn,
             must be left untouched otherwise !
             */
            virtual bool        match( Y_LANG_PATTERN_MATCH_ARGS ) const = 0;
            
            //! internal content, default is NULL
            virtual const void *content() const throw();

            //! accept an empty token ?
            virtual bool  accept_empty() const throw() = 0;

            //! write a graphviz representation
            virtual void  viz( ios::ostream & ) const = 0;

            //! graphviz helper
            static  void  encode( code_t code, ios::ostream &fp);
            static  void  encode( const token &, ios::ostream &fp);

            //! graphviz helper
            static  void  vizlink( ios::ostream &fp, const pattern *src, const pattern *tgt);
            
            //! output to a .dot file
            void graphviz(const string &filename) const;

        protected:
            explicit pattern(uint32_t ID) throw();
            pattern(const pattern &) throw();

        private:
            YOCTO_DISABLE_ASSIGN(pattern);
        };

        //! a list of pattern
        class p_list : public core::list_of_cpp<pattern>
        {
        public:
            explicit p_list() throw();
            virtual ~p_list() throw();
            p_list(const p_list &);
            p_list & operator=(const p_list &);
        };

    }
}


#endif

