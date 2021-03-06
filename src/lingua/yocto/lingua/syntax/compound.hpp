#ifndef YOCTO_LINGUA_SYNTAX_COMPOUND_INCLUDED
#define YOCTO_LINGUA_SYNTAX_COMPOUND_INCLUDED 1

#include "yocto/lingua/syntax/rule.hpp"

namespace yocto
{
    namespace lingua
    {
        namespace syntax
        {

            class compound : public rule
            {
            public:
                meta_list members;

                virtual ~compound() throw();

                compound & operator<<( const rule &);

            protected:
                explicit compound(const string &id,const uint32_t t);
                void subviz( ios::ostream &fp ) const;

            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(compound);
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

            class alternate : public compound
            {
            public:
                static const uint32_t UUID = YOCTO_FOURCC('[', '|', '|', ']');
                virtual ~alternate() throw();
                explicit alternate(const string &id);

                inline alternate & operator|=(const rule &r)
                {
                    (*this) << r;
                    return *this;
                }

                //______________________________________________________________
                //
                // virtual interface
                //______________________________________________________________
                virtual void viz(ios::ostream &fp) const;
                virtual bool admit(YOCTO_LINGUA_SYNTAX_RULE_ADMIT_ARGS) const;
                virtual bool admit_empty() const throw();

            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(alternate);
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

            class aggregate : public compound
            {
            public:
                static const uint32_t UUID = YOCTO_FOURCC('[', '&', '&', ']');
                virtual ~aggregate() throw();
                explicit aggregate(const string &id);

                inline aggregate & operator+=(const rule &r)
                {
                    (*this) << r;
                    return *this;
                }

                //______________________________________________________________
                //
                // virtual interface
                //______________________________________________________________
                virtual void viz(ios::ostream &fp) const;
                virtual bool admit(YOCTO_LINGUA_SYNTAX_RULE_ADMIT_ARGS) const;
                virtual bool admit_empty() const throw();

            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(aggregate);
            };
            
        }
    }
}



#endif
