#ifndef YOCTO_LINGUA_PATTERN_JOKER_INCLUDED
#define YOCTO_LINGUA_PATTERN_JOKER_INCLUDED 1

#include "yocto/lingua/pattern.hpp"

namespace yocto
{
    namespace lingua
    {

        class joker : public pattern
        {
        public:
            virtual ~joker() throw();

        protected:
            joker(const uint32_t t, pattern *p) throw();
            joker(const joker&);

            pattern *motif;


        private:
            YOCTO_DISABLE_ASSIGN(joker);

        };
    }

}

namespace yocto
{
    namespace lingua
    {
        class optional : public joker
        {
        public:
            static const uint32_t UUID = YOCTO_FOURCC(' ',' ','?',' ');
            virtual ~optional() throw();

            //__________________________________________________________________
            //
            // virtual interface
            //__________________________________________________________________
            virtual bool     match(YOCTO_LINGUA_PATTERN_MATCH_ARGS) const;
            virtual pattern *clone() const;
            virtual void     __viz(ios::ostream &) const;
            virtual void     __out(ios::ostream &) const;

            //__________________________________________________________________
            //
            // non virtual interface
            //__________________________________________________________________
            static optional *create(pattern *p);

        protected:
            explicit optional(pattern *p) throw();
            optional(const optional &other);


        private:
            YOCTO_DISABLE_ASSIGN(optional);
        };
    }
}

namespace yocto
{
    namespace lingua
    {
        class at_least : public joker
        {
        public:
            static const uint32_t UUID = YOCTO_FOURCC(' ','>','=',' ');
            virtual ~at_least() throw();

            const size_t nmin;

            //__________________________________________________________________
            //
            // virtual interface
            //__________________________________________________________________
            virtual bool     match(YOCTO_LINGUA_PATTERN_MATCH_ARGS) const;
            virtual pattern *clone() const;
            virtual void     __viz(ios::ostream &) const;
            virtual void     __out(ios::ostream &) const;

            //__________________________________________________________________
            //
            // non virtual interface
            //__________________________________________________________________
            static at_least *create(pattern *p,const size_t n);

        protected:
            explicit at_least(pattern *p,const size_t n) throw();
            at_least(const at_least &other);


        private:
            YOCTO_DISABLE_ASSIGN(at_least);
        };

        pattern *zero_or_more(pattern *);
        pattern *one_or_more(pattern *);
    }
}



namespace yocto
{
    namespace lingua
    {
        class counting : public joker
        {
        public:
            static const uint32_t UUID = YOCTO_FOURCC('{',' ',' ','}');
            virtual ~counting() throw();

            const size_t nmin;
            const size_t nmax;
            
            //__________________________________________________________________
            //
            // virtual interface
            //__________________________________________________________________
            virtual bool     match(YOCTO_LINGUA_PATTERN_MATCH_ARGS) const;
            virtual pattern *clone() const;
            virtual void     __viz(ios::ostream &) const;
            virtual void     __out(ios::ostream &) const;

            //__________________________________________________________________
            //
            // non virtual interface
            //__________________________________________________________________
            static counting *create(pattern *p,const size_t,const size_t);

        protected:
            explicit counting(pattern *p,const size_t,const size_t) throw();
            counting(const counting &other);


        private:
            YOCTO_DISABLE_ASSIGN(counting);
        };
    }
}





#endif

