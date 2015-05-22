#ifndef YOCTO_LANG_PATTERN_JOKER_INCLUDED
#define YOCTO_LANG_PATTERN_JOKER_INCLUDED 1

#include "yocto/lang/pattern.hpp"
#include "yocto/code/fourcc.hpp"

namespace yocto
{
    namespace lang
    {

        class joker : public pattern
        {
        public:
            virtual ~joker() throw();

        protected:
            explicit joker(uint32_t ID, pattern *p) throw();
            pattern *jk;

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(joker);
        };

        //! '?'
        class optional : public joker
        {
        public:
            static const uint32_t UUID = YOCTO_FOURCC('[', '?', '?', ']');
            virtual ~optional() throw();

            static  pattern *create(pattern *p);
            virtual pattern *clone() const;
            virtual bool     match(Y_LANG_PATTERN_MATCH_ARGS) const;
            virtual void     viz( ios::ostream & ) const;
            virtual bool     accept_empty() const throw();
            virtual void     save( ios::ostream &fp ) const;

        private:
            optional(pattern *p) throw();

            YOCTO_DISABLE_COPY_AND_ASSIGN(optional);
        };


        //! '>='
        class at_least : public joker
        {
        public:
            static const size_t UUID = YOCTO_FOURCC('[', '>', '=', ']');
            virtual ~at_least() throw();
            const size_t count;

            static  pattern *create(pattern *p, size_t n);
            virtual bool     match(Y_LANG_PATTERN_MATCH_ARGS) const;
            virtual pattern *clone() const;
            virtual void     viz( ios::ostream & ) const;
            virtual bool     accept_empty() const throw();
            virtual void     save( ios::ostream &fp ) const;

        private:
            at_least(pattern *p,size_t n) throw();
            YOCTO_DISABLE_COPY_AND_ASSIGN(at_least);
        };

        inline pattern *zero_or_more( pattern *p ) { return at_least::create(p,0); }
        inline pattern *one_or_more(  pattern *p ) { return at_least::create(p,1); }


        class counting : public joker
        {
        public:
            static const size_t UUID = YOCTO_FOURCC('[', '-', '-', ']');
            const size_t lower;
            const size_t upper;
            
            virtual ~counting() throw();
            static  pattern *create(pattern *p, size_t n, size_t m);
            virtual bool     match(Y_LANG_PATTERN_MATCH_ARGS) const;
            virtual pattern *clone() const;
            virtual void     viz( ios::ostream & ) const;
            virtual bool     accept_empty() const throw();
            virtual void     save( ios::ostream &fp ) const;

        private:
            explicit counting(pattern *p, size_t n, size_t m) throw();
            YOCTO_DISABLE_COPY_AND_ASSIGN(counting);
        };

    }
    
}

#endif

