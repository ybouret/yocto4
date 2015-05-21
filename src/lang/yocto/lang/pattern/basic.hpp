#ifndef YOCTO_LANG_PATTERN_BASIC_INCLUDED
#define YOCTO_LANG_PATTERN_BASIC_INCLUDED 1

#include "yocto/lang/pattern.hpp"
#include "yocto/code/fourcc.hpp"

namespace yocto
{
    namespace lang
    {

        //______________________________________________________________________
        //
        //! one char
        //______________________________________________________________________

        class one_char : public pattern
        {
        public:
            virtual ~one_char() throw();

            virtual bool match(Y_LANG_PATTERN_MATCH_ARGS) const;
            virtual bool accept_empty() const throw();

        protected:
            one_char(uint32_t ID) throw();
            one_char(const one_char &) throw();

        private:
            YOCTO_DISABLE_ASSIGN(one_char);
            virtual bool is_valid(code_t) const throw() = 0;
        };

        //______________________________________________________________________
        //
        //! any1 char
        //______________________________________________________________________
        class any1 : public one_char
        {
        public:
            static const uint32_t UUID = YOCTO_FOURCC('A','N','Y','1');
            virtual ~any1() throw();

            virtual pattern *clone() const;
            static  pattern *create();
            virtual void     viz( ios::ostream & ) const;
            
        private:
            any1() throw();
            YOCTO_DISABLE_COPY_AND_ASSIGN(any1);

            virtual bool is_valid(code_t) const throw(); //! true
        };


        //______________________________________________________________________
        //
        //! single char
        //______________________________________________________________________
        class single : public one_char
        {
        public:
            static const uint32_t UUID = YOCTO_FOURCC('S','N','G','L');
            const        code_t   code;

            virtual ~single() throw();

            virtual pattern *clone() const;
            static  pattern *create(code_t C);
            virtual void     viz( ios::ostream & ) const;

            virtual const void *content() const throw();

        private:
            single(code_t C) throw();
            single(const single &) throw();
            YOCTO_DISABLE_ASSIGN(single);

            virtual bool is_valid(code_t) const throw();
        };


        //______________________________________________________________________
        //
        //! range of char
        //______________________________________________________________________
        class range : public one_char
        {
        public:
            static const uint32_t UUID = YOCTO_FOURCC('R','N','G','E');
            const        code_t   lower;
            const        code_t   upper;

            virtual ~range() throw();

            virtual pattern *clone() const;
            static  pattern *create(code_t lo, code_t hi);
            virtual void     viz( ios::ostream & ) const;

        private:
            range(code_t lo, code_t hi) throw();
            range(const range &) throw();
            YOCTO_DISABLE_ASSIGN(range);

            virtual bool is_valid(code_t) const throw();  
        };

        
    }



}

#endif
