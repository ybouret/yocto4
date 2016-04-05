#ifndef YOCTO_LINGUA_PATTERN_BASIC_INCLUDED
#define YOCTO_LINGUA_PATTERN_BASIC_INCLUDED 1

#include "yocto/lingua/pattern.hpp"

namespace yocto
{
    namespace lingua
    {

        class any1 : public pattern
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
            virtual void     __out(ios::ostream &) const;

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
        class single : public pattern
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
            virtual void     __out(ios::ostream &) const;

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
        class range : public pattern
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
            virtual void     __out(ios::ostream &) const;

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

