#include "yocto/lang/pattern.hpp"
#include "yocto/code/fourcc.hpp"


namespace yocto
{
    namespace lang
    {

        class logical : public pattern
        {
        public:
            p_list operands;

            virtual ~logical() throw();
            void     append( pattern *p ) throw();

            static logical *among(const string &);
            static logical *among(const char   *);

            static logical *equal(const string &);
            static logical *equal(const char   *);

            static pattern *simplify( logical *p ) throw();

            virtual const void *content() const throw(); //!< return &operands
            virtual void        save( ios::ostream &fp ) const;

        protected:
            logical(const uint32_t) throw();
            logical(const logical &other);

            void vizops( ios::ostream &fp ) const;
            void save_ops( ios::istream &fp ) const;
            
        private:
            YOCTO_DISABLE_ASSIGN(logical);
        };


        class AND : public logical
        {
        public:
            static const uint32_t UUID = YOCTO_FOURCC('<', '&', '&' , '>');
            virtual ~AND() throw();

            virtual pattern *clone() const;
            virtual bool     match(Y_LANG_PATTERN_MATCH_ARGS) const;
            static  logical *create();
            virtual void     viz( ios::ostream & ) const;
            virtual bool     accept_empty() const throw();

        private:
            AND() throw();
            AND(const AND &);
            
            YOCTO_DISABLE_ASSIGN(AND);
        };

        class OR : public logical
        {
        public:
            static const uint32_t UUID = YOCTO_FOURCC('<', '|', '|' , '>');
            virtual ~OR() throw();

            virtual pattern *clone() const;
            virtual bool     match(Y_LANG_PATTERN_MATCH_ARGS) const;
            static  logical *create();
            virtual void     viz( ios::ostream & ) const;
            virtual bool     accept_empty() const throw();

        private:
            OR() throw();
            OR(const OR &);

            YOCTO_DISABLE_ASSIGN(OR);
        };


        class NOT : public logical
        {
        public:
            static const uint32_t UUID = YOCTO_FOURCC('<', '!', '!' , '>');
            virtual ~NOT() throw();

            virtual pattern *clone() const;
            virtual bool     match(Y_LANG_PATTERN_MATCH_ARGS) const;
            static  logical *create();
            virtual void     viz( ios::ostream & ) const;
            virtual bool     accept_empty() const throw();

        private:
            NOT() throw();
            NOT(const NOT &);

            YOCTO_DISABLE_ASSIGN(NOT);
        };


    }

}
