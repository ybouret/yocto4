#ifndef YOCTO_LINGUA_PATTERN_LOGIC_INCLUDED
#define YOCTO_LINGUA_PATTERN_LOGIC_INCLUDED 1

#include "yocto/lingua/pattern.hpp"

namespace yocto
{
    namespace lingua
    {

        class logical : public pattern
        {
        public:
            p_list operands;

            //__________________________________________________________________
            //
            // virtual interface
            //__________________________________________________________________
            virtual ~logical() throw();
            virtual void __sav(ios::ostream &fp) const;

            //__________________________________________________________________
            //
            // non virtual interface
            //__________________________________________________________________
            void add( pattern *p ) throw();

            static logical *among(const string &str);
            static logical *among(const char   *str);
            static logical *equal(const string &str);
            static logical *equal(const char   *str);

            
        protected:
            explicit logical(const uint32_t t) throw();
            logical(const logical &other);
            virtual void __subviz(ios::ostream &fp) const;

        private:
            YOCTO_DISABLE_ASSIGN(logical);
        };

    }
}

namespace yocto
{
    namespace lingua
    {

        class AND : public logical
        {
        public:
            static const uint32_t UUID = YOCTO_FOURCC('<', '&', '&', '>');

            //__________________________________________________________________
            //
            // virtual interface
            //__________________________________________________________________
            virtual ~AND() throw();
            virtual pattern *clone() const;
            virtual bool     match(YOCTO_LINGUA_PATTERN_MATCH_ARGS) const;
            virtual void     __viz(ios::ostream &) const;
            virtual bool     match_empty() const;

            static AND *create();

        protected:
            explicit AND() throw();
            AND(const AND &other);

        private:
            YOCTO_DISABLE_ASSIGN(AND);
        };
        
    }

}


namespace yocto
{
    namespace lingua
    {

        class OR : public logical
        {
        public:
            static const uint32_t UUID = YOCTO_FOURCC('<', '|', '|', '>');

            //__________________________________________________________________
            //
            // virtual interface
            //__________________________________________________________________
            virtual ~OR() throw();
            virtual pattern *clone() const;
            virtual bool     match(YOCTO_LINGUA_PATTERN_MATCH_ARGS) const;
            virtual void     __viz(ios::ostream &) const;
            virtual bool     match_empty() const;

            static OR *create();

        protected:
            explicit OR() throw();
            OR(const OR &other);

        private:
            YOCTO_DISABLE_ASSIGN(OR);
        };
        
    }
    
}


namespace yocto
{
    namespace lingua
    {

        class NONE : public logical
        {
        public:
            static const uint32_t UUID = YOCTO_FOURCC('<', '!', '!', '>');

            //__________________________________________________________________
            //
            // virtual interface
            //__________________________________________________________________
            virtual ~NONE() throw();
            virtual pattern *clone() const;
            virtual bool     match(YOCTO_LINGUA_PATTERN_MATCH_ARGS) const;
            virtual void     __viz(ios::ostream &) const;
            virtual bool     match_empty() const;

            static NONE *create();

        protected:
            explicit NONE() throw();
            NONE(const NONE &other);

        private:
            YOCTO_DISABLE_ASSIGN(NONE);
        };
        
    }
    
}




#endif
