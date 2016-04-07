#ifndef YOCTO_LINGUA_PATTERN_INCLUDED
#define YOCTO_LINGUA_PATTERN_INCLUDED 1

#include "yocto/lingua/source.hpp"
#include "yocto/code/fourcc.hpp"
#include "yocto/ios/ostream.hpp"

namespace yocto
{
    namespace lingua
    {

#define YOCTO_LINGUA_PATTERN_MATCH_ARGS token &tkn, source &src
#define YOCTO_LINGUA_PATTERN_IS(CLASS) assert(0==self); self = static_cast<CLASS *>(this)

        class pattern : public object
        {
        public:
            pattern        *next;
            pattern        *prev;
            const uint32_t  uuid;
            void           *self;

            virtual ~pattern() throw();

            //__________________________________________________________________
            //
            // virtual interface
            //__________________________________________________________________
            virtual pattern *clone() const = 0;                                 //!< cloning
            virtual bool     match(YOCTO_LINGUA_PATTERN_MATCH_ARGS) const = 0;  //!< matching
            virtual void     __sav(ios::ostream &) const = 0;                   //!< output data without uuid
            virtual void     __viz(ios::ostream &) const = 0;                   //!< output graphviz

            //__________________________________________________________________
            //
            // non virtual interface
            //__________________________________________________________________
            void   save(ios::ostream &) const; //!< write uuid and call __out
            string bin() const;
            string b64() const;
            void graphviz(const string &filename) const;

            static pattern *simplify(pattern *p) throw();
            static pattern *load(ios::istream &);


        protected:
            explicit pattern(const uint32_t) throw();
            pattern(const pattern &other) throw();

        private:
            YOCTO_DISABLE_ASSIGN(pattern);

        };


        typedef core::list_of_cloneable<pattern> p_list;
    }
}

#endif

