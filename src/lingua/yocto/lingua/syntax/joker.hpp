#ifndef YOCTO_LINGUA_SYNTAX_JOKER_INCLUDED
#define YOCTO_LINGUA_SYNTAX_JOKER_INCLUDED 1

#include "yocto/lingua/syntax/rule.hpp"

namespace yocto
{
    namespace lingua
    {
        namespace syntax
        {
            class joker : public rule
            {
            public:
                virtual ~joker() throw();

            protected:
                explicit joker(const string  &id,
                               const uint32_t t,
                               const rule    *r);

                const rule *jk;

            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(joker);
            };

        }

    }
}


namespace yocto
{
    namespace lingua
    {
        namespace syntax
        {

            class optional : public joker
            {
            public:
                static const uint32_t UUID = YOCTO_FOURCC('[', '?', ']', ' ');
                explicit optional(const string &id,const rule *r);
                virtual ~optional() throw();
                
                //______________________________________________________________
                //
                // virtual interface
                //______________________________________________________________
                virtual void viz(ios::ostream &fp) const;
                virtual bool admit(YOCTO_LINGUA_SYNTAX_RULE_ADMIT_ARGS) const;

            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(optional);
            };
        }

    }
}

namespace yocto
{
    namespace lingua
    {

        namespace syntax
        {
            class at_least : public joker
            {
            public:
                static const uint32_t UUID = YOCTO_FOURCC('[', '>', '=', ']');
                explicit at_least(const string &id, const rule *r, const size_t n);
                virtual ~at_least() throw();
                
                const size_t nmin;

                //______________________________________________________________
                //
                // virtual interface
                //______________________________________________________________
                virtual void viz(ios::ostream &fp) const;
                virtual bool admit(YOCTO_LINGUA_SYNTAX_RULE_ADMIT_ARGS) const;
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(at_least);
            };
        }
    }
}




#endif
