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
            virtual void     __out(ios::ostream &) const;

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
            static const uint32_t UUID = YOCTO_FOURCC(' ',' ',' ','?');
            virtual ~optional() throw();

            //__________________________________________________________________
            //
            // virtual interface
            //__________________________________________________________________
            virtual bool     match(YOCTO_LINGUA_PATTERN_MATCH_ARGS) const;
            virtual pattern *clone() const;
            virtual void     __viz(ios::ostream &) const;

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
        class zero_or_more : public joker
        {
        public:
            static const uint32_t UUID = YOCTO_FOURCC(' ',' ',' ','*');
            virtual ~zero_or_more() throw();

            //__________________________________________________________________
            //
            // virtual interface
            //__________________________________________________________________
            virtual bool     match(YOCTO_LINGUA_PATTERN_MATCH_ARGS) const;
            virtual pattern *clone() const;
            virtual void     __viz(ios::ostream &) const;

            //__________________________________________________________________
            //
            // non virtual interface
            //__________________________________________________________________
            static zero_or_more *create(pattern *p);

        protected:
            explicit zero_or_more(pattern *p) throw();
            zero_or_more(const zero_or_more &other);


        private:
            YOCTO_DISABLE_ASSIGN(zero_or_more);
        };
    }
}


namespace yocto
{
    namespace lingua
    {
        class one_or_more : public joker
        {
        public:
            static const uint32_t UUID = YOCTO_FOURCC(' ',' ',' ','+');
            virtual ~one_or_more() throw();

            //__________________________________________________________________
            //
            // virtual interface
            //__________________________________________________________________
            virtual bool     match(YOCTO_LINGUA_PATTERN_MATCH_ARGS) const;
            virtual pattern *clone() const;
            virtual void     __viz(ios::ostream &) const;

            //__________________________________________________________________
            //
            // non virtual interface
            //__________________________________________________________________
            static one_or_more *create(pattern *p);

        protected:
            explicit one_or_more(pattern *p) throw();
            one_or_more(const one_or_more &other);


        private:
            YOCTO_DISABLE_ASSIGN(one_or_more);
        };
    }
}








#endif

