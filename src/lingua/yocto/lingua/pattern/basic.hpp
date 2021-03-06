#ifndef YOCTO_LINGUA_PATTERN_BASIC_INCLUDED
#define YOCTO_LINGUA_PATTERN_BASIC_INCLUDED 1

#include "yocto/lingua/pattern.hpp"

namespace yocto
{
    namespace lingua
    {

        //! base class for accepting one char...
        class one_char : public pattern
        {
        public:
            virtual ~one_char() throw();

            virtual  bool match_empty() const;

        protected:
            one_char( const uint32_t t ) throw();
            one_char(const one_char &other) throw();


        private:
            YOCTO_DISABLE_ASSIGN(one_char);
        };

    }

}


namespace yocto
{
    namespace lingua
    {

        class any1 : public one_char
        {
        public:
            static const uint32_t UUID = YOCTO_FOURCC('A','N','Y','1');
            virtual ~any1() throw();

            //__________________________________________________________________
            //
            // virtual interface
            //__________________________________________________________________
            virtual bool     match(YOCTO_LINGUA_PATTERN_MATCH_ARGS) const;
            virtual pattern *clone() const;
            virtual void     __sav(ios::ostream &) const;
            virtual void     __viz(ios::ostream &) const;

            //__________________________________________________________________
            //
            // Non virtual interface
            //__________________________________________________________________
            static any1 *create();

        public:
            explicit any1() throw();

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(any1);
        };

    }

}

namespace yocto
{
    namespace lingua
    {
        class single : public one_char
        {
        public:
            static const uint32_t UUID = YOCTO_FOURCC('S', 'N', 'G', 'L');
            virtual ~single() throw();
            const uint8_t code;

            //__________________________________________________________________
            //
            // virtual interface
            //__________________________________________________________________
            virtual bool     match(YOCTO_LINGUA_PATTERN_MATCH_ARGS) const;
            virtual pattern *clone() const;
            virtual void     __sav(ios::ostream &) const;
            virtual void     __viz(ios::ostream &) const;

            //__________________________________________________________________
            //
            // Non virtual interface
            //__________________________________________________________________
            static single *create(const char c);

            
        protected:
            explicit single(const char c) throw();
            single(const single &other) throw();

        private:
            YOCTO_DISABLE_ASSIGN(single);
        };
    }
}

namespace yocto
{
    namespace lingua
    {
        class range : public one_char
        {
        public:
            static const uint32_t UUID = YOCTO_FOURCC('R', 'N', 'G', 'E');
            virtual ~range() throw();
            const uint8_t lower;
            const uint8_t upper;

            //__________________________________________________________________
            //
            // virtual interface
            //__________________________________________________________________
            virtual bool     match(YOCTO_LINGUA_PATTERN_MATCH_ARGS) const;
            virtual pattern *clone() const;
            virtual void     __sav(ios::ostream &) const;
            virtual void     __viz(ios::ostream &) const;

            //__________________________________________________________________
            //
            // Non virtual interface
            //__________________________________________________________________
            static range *create(const uint8_t lo,const uint8_t up);


        protected:
            explicit range(const uint8_t lo, const uint8_t up) throw();
            range(const range &other) throw();

        private:
            YOCTO_DISABLE_ASSIGN(range);
        };
    }
}




#endif

